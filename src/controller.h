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

  Controller() : args(NULL), mesh(NULL), raytracer(NULL), image(NULL) {}
  Controller(ArgParser *_args, Mesh *_mesh, RayTracer *_raytracer, Image *_image);
  ~Controller() {}
  
  void PartialRender( int processor_rank, int num_processor );
  void FullRender();

private:

  // various variables
  ArgParser *args;
  Mesh *mesh;
  RayTracer *raytracer;
  Image *image;
  Vec3f direction, position, orientation;

  // Callback functions for Render
  void SetCamera();
  Ray GetCameraRay(Vec2f point);
  Vec3f TraceRay(double x, double y);
  Color DrawPixel(double x, double y);
};

#endif
