#include "controller.h"

// ========================================================
// Initialize all appropriate variables, and ray-trace certain
// pixels
// ========================================================

// Constructor
Controller::Controller(ArgParser *_args, RayTracer *_raytracer, Image *_image) :
  args(_args), raytracer(_raytracer), image(_image) { SetCamera(); }


// sets the camera position based on args
void Controller::SetCamera() {
  //if camera position and direction are at the same spot,
  //set camera to default.  Use "direction" as point-of-interest
  Vec3f direction;
  if( args->camera_position == args->camera_direction ) {
    const BoundingBox *coord = raytracer->getMesh()->getBoundingBox();
    direction = coord->getCenter();
    position = direction + Vec3f( 0, 0, 4 * coord->maxDim() );
  } else {
    direction = args->camera_direction;
    position = args->camera_position;
  }
  
  //get the actual camera direction
  direction -= position;
  direction.Normalize();

  //Copy & paste from camera.cpp
  Vec3f horizontal, screenUp;
  Vec3f::Cross3(horizontal, direction, args->camera_orientation);
  Vec3f::Cross3(screenUp, horizontal, direction);
  double screenHeight = tan(20 * M_PI / 180.0 / 2.0);
  horizontal *= screenHeight;
  screenUp *= screenHeight;
  
  xAxis = horizontal * 2;
  yAxis = screenUp * 2;
  lowerLeft = position + direction - horizontal - screenUp;
}


// sets the camera position based on args
Ray Controller::GetCameraRay(double x, double y) {
  Vec3f dir = lowerLeft + xAxis * x + yAxis * y - position;
  dir.Normalize();
  
  //need dir, position
  return Ray( position, dir );
}


// trace a ray through pixel (x,y) of the image and return the color
Vec3f Controller::TraceRay(double x, double y) {
  //translate pixel coordinates to screen coordinates
  double xFill = x / double(args->width);
  double yFill = y / double(args->height);
  
  // compute and set the pixel color
  Ray r = GetCameraRay( xFill, yFill );
  Hit hit;
  Vec3f color = raytracer->TraceRay( r, hit, args->num_bounces );
  return color;
}


// Scan through the image from the lower left corner across each row
// and then up to the top right.  Initially the image is sampled very
// coarsely.  Increment the static variables that track the progress
// through the scans
Color Controller::DrawPixel(int x, int y) {
  // compute the color and position of intersection
  Vec3f color = TraceRay( double(x), double(y) );
  double r = linear_to_srgb(color.x()) * 255.0;
  double g = linear_to_srgb(color.y()) * 255.0;
  double b = linear_to_srgb(color.z()) * 255.0;
  
  //Taro's random extensions
  if( args->gray_scale ) {
    //gray-scaling algorithm from
    //http://www.mathworks.com/support/solutions/data/1-1ASCU.html
    r = g = b = r * 0.299 + g * 0.587 + b * 0.114;
  }
  
  //Make sure the r g b values are less than 255
  if( r > 255 ) r = 255;
  if( g > 255 ) g = 255;
  if( b > 255 ) b = 255;
  
  //color image
  Color toFill = Color( (int) r, (int) g, (int) b);
  image->SetPixel( x, y, toFill );
  return toFill;
}


//Creates a full image
void Controller::FullRender() {
  int width = image->Width(), height = image->Height();
  for( int i = 0; i < width; ++i ) {
    for( int j = 0; j < height; ++j ) {
      DrawPixel( i, j );
    }
  }
}


//Creates a partial image for MPI
void Controller::PartialRender( int processor_rank, int num_processor ) {
  int width = image->Width();
  int total_pixels = width * image->Height();
  int work_unit = total_pixels / num_processor;
  int end, start = work_unit * processor_rank;
  int x, y;
  if( processor_rank == num_processor - 1 )
    end = total_pixels;
  else
    end = work_unit + start;
  for( int i = start; i < end; ++i ) {
    x = i % width;
    y = i / width;
    DrawPixel( x, y );
  }
}

