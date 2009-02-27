#ifndef __ARG_PARSER_H__
#define __ARG_PARSER_H__

#include "string.h"
#include "stdio.h"
#include "assert.h"
#include "stdlib.h"

#include "vectors.h"
#include "utils.h"

enum RENDER_MODE { RENDER_MATERIALS, RENDER_RADIANCE, RENDER_FORM_FACTORS, RENDER_LIGHTS, RENDER_UNDISTRIBUTED, RENDER_ABSORBED };
#define NUM_RENDER_MODES 6

class ArgParser {

public:

  ArgParser() { DefaultValues(); }

  ArgParser(int argc, char *argv[]) {
    DefaultValues();

    for (int i = 1; i < argc; i++) {
      if (!strcmp(argv[i],"-input")) {
	i++; assert (i < argc); 
	input_file = argv[i];
      } else if (!strcmp(argv[i],"-size")) {
	i++; assert (i < argc); 
	width = atoi(argv[i]);
	i++; assert (i < argc); 
         height = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-sphere_rasterization")) {
	i++; assert (i < argc); 
	sphere_horiz = atoi(argv[i]);
         if (sphere_horiz % 2 == 1) sphere_horiz++; 
	i++; assert (i < argc); 
	sphere_vert = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-num_bounces")) {
	i++; assert (i < argc); 
	num_bounces = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-num_shadow_samples")) {
	i++; assert (i < argc); 
	num_shadow_samples = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-num_glossy_samples")) {
	i++; assert (i < argc); 
	num_glossy_samples = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-num_form_factor_samples")) {
	i++; assert (i < argc); 
	num_form_factor_samples = atoi(argv[i]);
      } else if (!strcmp(argv[i],"-background_color")) {
         i++; assert (i < argc);
         double r = atof(argv[i]);
         i++; assert (i < argc);
         double g = atof(argv[i]);
         i++; assert (i < argc);
         double b = atof(argv[i]);
	 // background color input is assumed to be sRGB
         background_color = Vec3f(r,g,b);
         background_color_linear = 
	   Vec3f(srgb_to_linear(r),srgb_to_linear(g),srgb_to_linear(b));
      } else if (!strcmp(argv[i],"-ambient")) {
	i++; assert (i < argc);
         double r = atof(argv[i]);
         i++; assert (i < argc);
         double g = atof(argv[i]);
         i++; assert (i < argc);
         double b = atof(argv[i]);
	 // ambient color input is assumed to be sRGB
         ambient_light = Vec3f(r,g,b);
         ambient_light_linear = 
	   Vec3f(srgb_to_linear(r),srgb_to_linear(g),srgb_to_linear(b));
      } else {
	printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
	assert(0);
      }
    }
  }

  void DefaultValues() {
    input_file = NULL;
    width = 100;
    height = 100;
    wireframe = false;
    interpolate = false;
    render_mode = RENDER_MATERIALS;
    raytracing_animation = false;
    radiosity_animation = false;
    num_bounces = 0;
    num_shadow_samples = 0;
    num_glossy_samples = 0;
    num_form_factor_samples = 1;
    sphere_horiz = 8;
    sphere_vert = 6;
    intersect_backfacing = false;
    background_color = Vec3f(1,1,1);
    background_color_linear = Vec3f(1,1,1);
    ambient_light = Vec3f(0.2,0.2,0.2);
    ambient_light_linear = 
      Vec3f(srgb_to_linear(ambient_light.r()),
	    srgb_to_linear(ambient_light.g()),
	    srgb_to_linear(ambient_light.b()));
  }

  // ==============
  // REPRESENTATION
  // all public! (no accessors)

  char *input_file;
  int width;
  int height;
  bool wireframe;
  bool interpolate;
  enum RENDER_MODE render_mode;
  bool raytracing_animation;
  bool radiosity_animation;

  Vec3f ambient_light;
  Vec3f ambient_light_linear;
  Vec3f background_color;
  Vec3f background_color_linear;

  int num_bounces;
  int num_shadow_samples;
  int num_glossy_samples;

  int num_form_factor_samples;

  int sphere_horiz;
  int sphere_vert;

  bool intersect_backfacing;

};

#endif
