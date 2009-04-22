#include <stdio.h>

#include "argparser.h"
#include "mesh.h"
#include "raytracer.h"
#include "image.h"
#include "controller.h"
#include <mpi.h>

const int COMBINE_RANK = 0;

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
	uintmax_t outsize = (args.width * args.height) * 3;
	if (nrank == COMBINE_RANK)
		combined_output = new unsigned char[outsize];

	const unsigned char *mydata = controller.GetOutputStream();
	MPI::COMM_WORLD.Gather(
			mydata,
			((args.width * args.height)/npes) * 3, MPI::UNSIGNED_CHAR,
			(combined_output),
			outsize, MPI::UNSIGNED_CHAR,
			COMBINE_RANK);

	delete [] mydata;

	if (nrank == COMBINE_RANK) {
		printf("got data");
		Image image;
		image.Allocate( args.width, args.height );
		controller.Output(image);
		if ( args.output_file )
    status = (int) image.Save( std::string(args.output_file) );
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
