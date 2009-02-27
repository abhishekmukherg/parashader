#include "sphere.h"
#include "material.h"
#include "argparser.h"
#include "utils.h"

// Included files for OpenGL Rendering
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

// ====================================================================
// ====================================================================

bool Sphere::intersect(const Ray &r, Hit &h) const {




  // ==========================================
  // ASSIGNMENT:  IMPLEMENT SPHERE INTERSECTION
  // ==========================================

  // plug the explicit ray equation into the implict sphere equation and solve



  // fix this: return true if the sphere was intersected, and update
  // the hit data structure to contain the value of t for the ray at
  // the intersection point, the material, and the normal
  return false;


} 

// ====================================================================
// ====================================================================
