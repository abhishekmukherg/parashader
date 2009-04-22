#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include <stdlib.h>
#include <assert.h>
#include <ostream>
#include "vectors.h"
#include "image.h"
#include "raytracer.h"
#include "argparser.h"
#include "mesh.h"

class Controller {

public:
  Controller(ArgParser *_args, RayTracer *_raytracer, int rank=0, int npes=1);
  ~Controller();

	const Controller &operator=(const Controller &rhs);
	Controller(const Controller &rhs);
  
  void PartialRender( int processor_rank, int num_processor );
  void FullRender();
  void SetCamera();
  void SetExtensions();

	void Output(Image &out);
	void Output(std::ostream &out);
	const unsigned char *GetOutputStream() const;

private:

  // various variables
  ArgParser *args;
  RayTracer *raytracer;
  Vec3f position, lowerLeft, xAxis, yAxis;
	Color *output;
	uintmax_t pixelcount;
	int processor_count;
	int curr_proc;
	
  
  // Callback functions for Render
  Ray GetCameraRay(double x, double y);
  Vec3f TraceRay(double x, double y);
  Color DrawPixel(int x, int y);
  
  //Taro's random extensions
  double splitRed, splitGreen, splitBlue,
    redWeight, greenWeight, blueWeight;
  void NPR(double &red, double &green, double &blue);
  void GrayScale(double &red, double &green, double &blue);
};

#endif

// vim: ts=2:sw=2
