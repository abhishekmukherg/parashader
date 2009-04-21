#include "controller.h"

// ========================================================
// Initialize all appropriate variables, and ray-trace certain
// pixels
// ========================================================


// Constructor
Controller::Controller(ArgParser *_args, Mesh *_mesh, RayTracer *_raytracer, Image *_image) :
  args(_args), mesh(_mesh), raytracer(_raytracer), image(_image) { SetCamera(); }


// sets the camera position based on args
void Controller::SetCamera() {
  //setup camera variables
  orientation = args->camera_orientation;
  
  //if camera position and direction are at the same spot,
  //set camera to default.  Use "direction" as point-of-interest
  if( args->camera_position == args->camera_direction ) {
    direction = mesh->getBoundingBox()->getCenter();
    double max_dim = mesh->getBoundingBox()->maxDim();
    position = direction + Vec3f(0,0,4*max_dim);
  } else {
    direction = args->camera_direction;
    position = args->camera_position;
  }
  
  //get the actual camera direction
  direction -= position;
  direction.Normalize();
}


// sets the camera position based on args
Ray Controller::GetCameraRay(Vec2f point) {
  //M_PI?
  double angle = 20 * M_PI/180.0
  
  //Copy & paste from camera.cpp
  Vec3f screenCenter = position + direction;
  Vec3f horizontal, screen_up;
  Vec3f::Cross3(horizontal, direction, orientation);
  Vec3f::Cross3(screen_up, horizontal, direction);
  double screenHeight = tan(angle/2.0);
  Vec3f xAxis = horizontal * 2 * screenHeight;
  Vec3f yAxis = screen_up * 2 * screenHeight;
  Vec3f lowerLeft = screenCenter - (screen_up * screenHeight) - (horizontal * screenHeight);
  Vec3f screenPoint = lowerLeft + xAxis*point.x() + yAxis*point.y();
  Vec3f dir = screenPoint - position;
  dir.Normalize();
  return Ray(position,dir);
}


// trace a ray through pixel (x,y) of the image and return the color
Vec3f Controller::TraceRay(double x, double y) {
  // compute and set the pixel color
  Ray r = camera->GetCameraRay(Vec2f(x,y));
  Hit hit;
  Vec3f color = raytracer->TraceRay(r,hit,args->num_bounces);
  return color;
}


// Scan through the image from the lower left corner across each row
// and then up to the top right.  Initially the image is sampled very
// coarsely.  Increment the static variables that track the progress
// through the scans
Color Controller::DrawPixel(double x, double y) {
  // compute the color and position of intersection
  Vec3f color = TraceRay(x, y);
  double r = linear_to_srgb(color.x()) * 255;
  double g = linear_to_srgb(color.y()) * 255;
  double b = linear_to_srgb(color.z()) * 255;
  int red = (int) r;
  int green = (int) g;
  int blue = (int) b;
  Color toFill = Color(red, green, blue);
  int xFill = (int) x;
  int yFill = (int) y;
  image->SetPixel( xFill, yFill, toFill );
  return toFill;
}


//Creates a full image
void Controller::FullRender() {
  int width = image->Width(), height = image->Height();
  double x, y;
  for( int i = 0; i < width; ++i ) {
    for( int j = 0; j < height; ++j ) {
      x = (double) i;
      y = (double) j;
      DrawPixel( x, y );
    }
  }
}


//Creates a partial image for MPI
void Controller::PartialRender( int processor_rank, int num_processor ) {
  int width = image->Width();
  int total_pixels = width * image->Height();
  int work_unit = total_pixels / num_processor;
  int end, start = work_unit * processor_rank;
  double x, y;
  if( processor_rank == num_processor - 1 )
    end = total_pixels;
  else
    end = work_unit + start;
  for( int i = start; i < end; ++i ) {
    x = (double) ( i % width );
    y = (double) ( i / width );
    DrawPixel( x, y );
  }
}

