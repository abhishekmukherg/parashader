#include <stdio.h>

#include "argparser.h"
#include "mesh.h"
#include "raytracer.h"
#include "image.h"
#include "controller.h"

// =========================================
// =========================================

int main(int argc, char *argv[]) {
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

  Controller controller( &args, &raytracer );
  controller.FullRender();

  Image image;
  image.Allocate( args.width, args.height );
  controller.Output(image);
  if ( args.output_file )
    status = (int) image.Save( std::string(args.output_file) );

  // well it never returns from the GLCanvas loop...
  return status;
}

// =========================================
// =========================================
