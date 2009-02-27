#include <stdio.h>

#include "argparser.h"
#include "glCanvas.h"
#include "mesh.h"
#include "radiosity.h"
#include "raytracer.h"

// Included files for OpenGL Rendering
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

// =========================================
// =========================================

int main(int argc, char *argv[]) {
  // deterministic (repeatable) randomness
  srand(37);
  // "real" randomness
  //srand((unsigned)time(0));

  ArgParser *args = new ArgParser(argc, argv);
  glutInit(&argc, argv);

  Mesh *mesh = new Mesh();
  mesh->Load(args->input_file,args);
  RayTracer *raytracer = new RayTracer(mesh,args);
  Radiosity *radiosity = new Radiosity(mesh,args);
  raytracer->setRadiosity(radiosity);
  radiosity->setRayTracer(raytracer);

  GLCanvas glcanvas;
  glcanvas.initialize(args,mesh,raytracer,radiosity); 

  // well it never returns from the GLCanvas loop...
  delete args;
  return 0;
}

// =========================================
// =========================================
