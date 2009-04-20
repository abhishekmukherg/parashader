#include "controller.h"

// ========================================================
// Initialize all appropriate OpenGL variables, set
// callback functions, and start the main event loop.
// This function will not return but can be terminated
// by calling 'exit(0)'
// ========================================================

// sets the camera position based on args
void Controller::SetCamera() {
}

// trace a ray through pixel (i,j) of the image and return the color
Vec3f Controller::TraceRay(double i, double j) {
  // compute and set the pixel color
  int max_d = max2(args->width,args->height);
  double x = (i+0.5-args->width/2.0)/double(max_d)+0.5;
  double y = (j+0.5-args->height/2.0)/double(max_d)+0.5;

  Ray r = camera->generateRay(Vec2f(x,y));
  Hit hit;
  Vec3f color = raytracer->TraceRay(r,hit,args->num_bounces);
  //RayTree::SetMainSegment(r,0,hit.getT());
  return color;
}

// Scan through the image from the lower left corner across each row
// and then up to the top right.  Initially the image is sampled very
// coarsely.  Increment the static variables that track the progress
// through the scans
int Controller::DrawPixel() {
  if (raytracing_x > args->width) {
    raytracing_x = raytracing_skip/2;
    raytracing_y += raytracing_skip;
  }
  if (raytracing_y > args->height) {
    if (raytracing_skip == 1) return 0;
    raytracing_skip = raytracing_skip / 2;
    if (raytracing_skip % 2 == 0) raytracing_skip++;
    assert (raytracing_skip >= 1);
    raytracing_x = raytracing_skip/2;
    raytracing_y = raytracing_skip/2;
    glEnd();
    glPointSize(raytracing_skip);
    glBegin(GL_POINTS);
  }

  // compute the color and position of intersection
  Vec3f color = TraceRay(raytracing_x, raytracing_y);
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

/*
  if( args->camera_position == args->camera_direction ) {
    Vec3f point_of_interest = mesh->getBoundingBox()->getCenter();
    double max_dim = mesh->getBoundingBox()->maxDim();
    Vec3f camera_position = point_of_interest + Vec3f(0,0,4*max_dim);
    camera = new PerspectiveCamera(camera_position, point_of_interest, args->camera_orientation, 20 * M_PI/180.0);
  } else {
    camera = new PerspectiveCamera(args->camera_position, args->camera_direction, args->camera_orientation, 20 * M_PI/180.0);
  }
  */

/*
  case 'r':  case 'R':
    // animate raytracing of the scene
    args->raytracing_animation = !args->raytracing_animation;
    if (args->raytracing_animation) {
      raytracing_skip = max2(args->width,args->height) / 10;
      if (raytracing_skip % 2 == 0) raytracing_skip++;
      assert (raytracing_skip >= 1);
      raytracing_x = raytracing_skip/2;
      raytracing_y = raytracing_skip/2;
      display(); // clear out any old rendering
      printf ("raytracing animation started, press 'R' to stop\n");
    } else
      printf ("raytracing animation stopped, press 'R' to start\n");
    break;
    */

    /*
    if (args->radiosity_animation) {
    double undistributed = radiosity->Iterate();
    if (undistributed < 0.001) {
      args->radiosity_animation = false;
      printf ("undistributed < 0.001, animation stopped\n");
    }
    Render();
  }
  if (args->raytracing_animation) {
    // draw 100 pixels and then refresh the screen and handle any user input
    for (int i = 0; i < 100; i++) {
      if (!DrawPixel()) {
	args->raytracing_animation = false;
	break;
      }
    }
  }
    */

void Controller::Render() {
}

