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
  Controller(ArgParser *_args, RayTracer *_raytracer);
  ~Controller();

	const Controller &operator=(const Controller &rhs);
	Controller(const Controller &rhs);
  
  void PartialRender( int processor_rank, int num_processor );
  void FullRender();
  void SetCamera();

	void Output(Image &out);

private:

  // various variables
  ArgParser *args;
  //Mesh *mesh;
  RayTracer *raytracer;
  //Vec3f direction, position, orientation;
  Vec3f position, lowerLeft, xAxis, yAxis;
	Color *output;
	uintmax_t pixelcount;
	
  
  // Callback functions for Render
  Ray GetCameraRay(double x, double y);
  Vec3f TraceRay(double x, double y);
  Color DrawPixel(int x, int y);
};

#endif

// vim: ts=2:sw=2
