#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <stdlib.h>
#include <assert.h>
#include "vectors.h"
#include "image.h"
#include "raytracer.h"
#include "argparser.h"
#include "mesh.h"

class Controller {

public:

  Controller() : args(NULL), raytracer(NULL), image(NULL) {}
  Controller(ArgParser *_args, RayTracer *_raytracer, Image *_image);
  ~Controller() {}
  
  void PartialRender( int processor_rank, int num_processor );
  void FullRender();
  void SetCamera();

private:

  // various variables
  ArgParser *args;
  RayTracer *raytracer;
  Image *image;
  Vec3f position, lowerLeft, xAxis, yAxis;
  
  // Callback functions for Render
  Ray GetCameraRay(double x, double y);
  Vec3f TraceRay(double x, double y);
  Color DrawPixel(int x, int y);
  
  //Taro's random extensions
  void NPR(double &red, double &green, double &blue);
  void GrayScale(double &red, double &green, double &blue);
};

#endif
