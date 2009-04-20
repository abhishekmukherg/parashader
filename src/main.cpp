#include <stdio.h>

#include "argparser.h"
#include "mesh.h"
#include "raytracer.h"

// =========================================
// =========================================

int main(int argc, char *argv[]) {
  // deterministic (repeatable) randomness
  srand(37);
  // "real" randomness
  //srand((unsigned)time(0));

  ArgParser *args = new ArgParser(argc, argv);

  Mesh *mesh = new Mesh();
  mesh->Load(args->input_file,args);
  RayTracer *raytracer = new RayTracer(mesh,args);

  // well it never returns from the GLCanvas loop...
  delete raytracer;
  delete mesh;
  delete args;
  return 0;
}

// =========================================
// =========================================
