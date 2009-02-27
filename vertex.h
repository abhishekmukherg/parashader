#ifndef _VERTEX_H
#define _VERTEX_H

#include <assert.h>
#include "vectors.h"

class Vertex;

// ==========================================================

class Vertex {

public:

  // ========================
  // CONSTRUCTOR & DESTRUCTOR
  Vertex(int i, const Vec3f &pos) : position(pos) { index = i; s = 0; t = 0; }
  virtual ~Vertex() { }
  
  // =========
  // ACCESSORS
  int getIndex() const { return index; }
  double x() const { return position.x(); }
  double y() const { return position.y(); }
  double z() const { return position.z(); }
  const Vec3f& get() const { return position; }
  double get_s() const { return s; }
  double get_t() const { return t; }

  // =========
  // MODIFIERS
  void set(Vec3f v) { position = v; }
  void set(double x, double y, double z) { position.Set(x,y,z); }
  void setTextureCoordinates(double _s, double _t) { s = _s; t = _t; }

private:

  // don't use these constructors
  Vertex() { assert(0); }
  Vertex& operator=(const Vertex&) { assert(0); exit(0); }
  Vertex(const Vertex&) { assert(0); }
  
  // ==============
  // REPRESENTATION
  Vec3f position;

  // texture coordinates
  // NOTE: arguably these should be stored at the faces of the mesh
  // rather than the vertices
  double s,t;

  // this is the index from the original .obj file.
  // technically not part of the half-edge data structure
  int index;  

  // NOTE: the vertices don't know anything about adjacency.  In some
  // versions of this data structure they have a pointer to one of
  // their incoming edges.  However, this data is very complicated to
  // maintain during mesh manipulation.

};

// ==========================================================

#endif

