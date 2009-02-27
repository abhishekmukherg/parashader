#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "vectors.h"
#include "material.h"
#include <vector>

class Face;
class ArgParser;

// ====================================================================
// ====================================================================

class Sphere {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Sphere(const Vec3f &c, double r, Material *m) {
    center = c; radius = r; material = m;
    assert (radius >= 0); }
  ~Sphere() {}

  bool intersect(const Ray &r, Hit &h) const;
  Material* getMaterial() const { return material; }

private:

  Sphere() { assert (0); } // don't use this

  // REPRESENTATION
  Vec3f center;
  double radius;
  Material *material;

};

// ====================================================================
// ====================================================================

#endif
