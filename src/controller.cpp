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
// TODO: convert that doubles to simple ints
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
// TODO: convert that doubles to simple ints
Color Controller::DrawPixel(double x, double y) {
  // compute the color and position of intersection
  Vec3f color = TraceRay(x, y);
  double r = linear_to_srgb(color.x());
  double g = linear_to_srgb(color.y());
  double b = linear_to_srgb(color.z());
  glColor3f(r,g,b);
  double x = 2 * (raytracing_x/double(args->width)) - 1;
  double y = 2 * (raytracing_y/double(args->height)) - 1;
  glVertex3f(x,y,-1);
  raytracing_x += raytracing_skip;
  return 1;
}


void Controller::PartialRender( int processor_rank, int num_processor ) {
}


void Controller::FullRender() {
}

