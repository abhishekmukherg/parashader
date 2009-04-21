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
  ArgParser *args = new ArgParser(argc, argv);
  if ( !args->input_file) {
    printf ( "ERROR! No input file provided.\n" );
    status++;
  }
  if ( !args->output_file ) {
    printf ( "ERROR! No output file provided.\n" );
    status++;
  }
  if ( status ) return 0;

  //initialize pointers
  Mesh *mesh = new Mesh();
  mesh->Load(args->input_file,args);

  RayTracer *raytracer = new RayTracer(mesh,args);

  Image *image = new Image();
  image->Allocate( args->width, args->height );

  Controller *controller = new Controller( args, raytracer, image );
  
  //Testing: doing full render of the image
  controller->FullRender();
  if ( args->output_file )
    status = (int) image->Save( std::string(args->output_file) );

  // well it never returns from the GLCanvas loop...
  delete controller;
  delete raytracer;
  delete image;
  delete mesh;
  delete args;
  return status;
}

// =========================================
// =========================================
