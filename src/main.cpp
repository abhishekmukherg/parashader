#include <stdio.h>

#include "argparser.h"
#include "mesh.h"
#include "raytracer.h"

// =========================================
// =========================================

int main(int argc, char *argv[]) {
  int status = 0;
  
  // deterministic (repeatable) randomness
  srand(37);
  // "real" randomness
  //srand((unsigned)time(0));

  ArgParser *args = new ArgParser(argc, argv);

  Mesh *mesh = new Mesh();
  mesh->Load(args->input_file,args);

  RayTracer *raytracer = new RayTracer(mesh,args);

  Image *image = new Image();
  image->Allocate( args->width, args->height );

  Controller *controller = new Controller( args, mesh, raytracer, image );
  
  //Testing: doing full render of the image
  controller->FullRender();
  if( args->output_file ) {
    if( !image->Save( std::string(output_file) ) ) status = 1
  }

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
