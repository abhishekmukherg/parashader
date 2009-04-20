#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <stdlib.h>
#include <assert.h>
#include "vectors.h"
#include "image.h"
#include "raytracer.h"
#include "argparser.h"

class Controller {

public:

  Controller(ArgParser *_args, Mesh *_mesh, RayTracer *_raytracer,
    Image *_image, int _processor_rank, int _num_processor) : args(_args),
    mesh(_mesh), raytracer(_raytracer), image(_image),
    proc_rank(_processor_rank), num_proc(_num_processor) {}
  
  ~Controller() {}
  
  void Render();

private:

  // various variables
  ArgParser *args;
  Mesh *mesh;
  RayTracer *raytracer;
  Image *image;
  Vec3f direction, position, orientation;
  int proc_rank;
  int num_proc;

  // Callback functions for Render
  Ray SetCamera();
  int DrawPixel();
  Vec3f TraceRay(double i, double j);
};

#endif