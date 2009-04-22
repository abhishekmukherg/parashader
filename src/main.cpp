#include <stdio.h>

#include "argparser.h"
#include "mesh.h"
#include "raytracer.h"
#include "image.h"
#include "controller.h"
#include <mpi.h>

static const int COMBINE_RANK = 0;

void deinterlace(const int npes, const unsigned char *data, Image &output);

void deinterlace(const int npes, const unsigned char *data, Image &output)
{
  const int width = output.Width();
  const int count = (width * output.Height()) * 3;
  const int stride = (count / 3) / npes;
  for (int i = 0; i < count; i += 3) {
    const int data_index = i / 3;
    const int pixel_index = npes * (data_index % stride)
      + (data_index / stride);
    const int y = pixel_index / width;
    const int x = pixel_index % width;
    assert(i+2 < count);
    output.SetPixel(y, x, Color(data[i], data[i + 1], data[i + 2]));
  }
}

// =========================================
// =========================================

int main(int argc, char *argv[]) {
  MPI::Init(argc, argv);
  int status = 0;

  // deterministic (repeatable) randomness
  srand(37);
  // "real" randomness
  //srand((unsigned)time(0));

  //initialize parameters
  ArgParser args(argc, argv);
  if ( !args.input_file) {
    printf ( "ERROR! No input file provided.\n" );
    status++;
  }
  if ( !args.output_file ) {
    printf ( "ERROR! No output file provided.\n" );
    status++;
  }
  if ( status ) return 0;

  Mesh mesh;
  mesh.Load(args.input_file,&args);
  RayTracer raytracer(&mesh,&args);

  const int nrank = MPI::COMM_WORLD.Get_rank();
  const int npes = MPI::COMM_WORLD.Get_size();
  Controller controller( &args, &raytracer, nrank, npes );
  controller.FullRender();

  unsigned char *combined_output;
  uintmax_t pixelcount = args.width * args.height;
  uintmax_t outsize = pixelcount * 3;
  if (nrank == COMBINE_RANK)
    combined_output = new unsigned char[outsize + 1];

  const unsigned char *mydata = controller.GetOutputStream();
  const int mysize = ((args.width * args.height) / npes) * 3;
  MPI::COMM_WORLD.Gather(
      mydata, mysize, MPI::UNSIGNED_CHAR,
      combined_output, mysize, MPI::UNSIGNED_CHAR,
      COMBINE_RANK);

  delete [] mydata;

  if (nrank == COMBINE_RANK) {
    Image image;
    image.Allocate( args.width, args.height );
    deinterlace(npes, combined_output, image);
    if ( args.output_file )
      status = (int) image.Save( std::string(args.output_file) );
    delete[] combined_output;
  } else {
    status = 0;
  }

  // well it never returns from the GLCanvas loop...
  MPI::Finalize();
  return status;
}

// =========================================
// =========================================
// vim: ts=2:sw=2
