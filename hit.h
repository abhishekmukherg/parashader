#ifndef _HIT_H
#define _HIT_H

#include <limits>
#include <float.h>
#include <ostream>
#include "vectors.h"
#include "ray.h"

class Material;
class Face;

using std::ostream;

// ====================================================================
// ====================================================================

class Hit {
  
public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit() { 
    t = FLT_MAX;
    material = NULL;
    normal = Vec3f(0,0,0); 
    texture_s = 0;
    texture_t = 0;
  }
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    normal = h.normal; 
    texture_s = h.texture_s;
    texture_t = h.texture_t;
  }
  ~Hit() {}

  // ACCESSORS
  double getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getNormal() const { return normal; }
  double get_s() const { return texture_s; }
  double get_t() const { return texture_t; }

  // MODIFIER
  void set(double _t, Material *m, Vec3f n) {
    t = _t; material = m; normal = n; 
    texture_s = 0; texture_t = 0; }

  void setTextureCoords(double t_s, double t_t) {
    texture_s = t_s; texture_t = t_t; 
  }

private: 

  // REPRESENTATION
  double t;
  Material *material;
  Vec3f normal;
  double texture_s, texture_t;
};

inline ostream &operator<<(ostream &os, const Hit &h) {
  os << "Hit <" <<h.getT()<<", "<<h.getNormal()<<">";
  return os;
}
// ====================================================================
// ====================================================================

#endif
