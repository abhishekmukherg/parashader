#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <assert.h>
#include <string>
using std::string;

#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include "image.h"

class ArgParser;

// ====================================================================
// ====================================================================
// A simple Phong-like material with roughness (for glossy rendering)

class Material {

public:

  Material(const string &texture_file,
	   const Vec3f &d_color,
           const Vec3f &r_color,
           const Vec3f &e_color,
           double rough) {
    textureFile = texture_file;
    if (textureFile != "") {
      image.Load(textureFile);
      ComputeAverageTextureColor();
    } else {
      diffuseColor = d_color;
    }
    reflectiveColor = r_color;
    emittedColor = e_color;
    roughness = rough;
    // need to initialize texture_id after glut has started
  }
  
  ~Material();

  // ACCESSORS
  const Vec3f& getDiffuseColor() const { return diffuseColor; }
  const Vec3f getDiffuseColor(double s, double t) const;
  const Vec3f& getReflectiveColor() const { return reflectiveColor; }
  const Vec3f& getEmittedColor() const { return emittedColor; }  
  double getRoughness() const { return roughness; }
  bool hasTextureMap() const { return (textureFile != ""); }

  // SHADE
  // compute the contribution to local illumination at this point for
  // a particular light source
  Vec3f Shade
  (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, 
   const Vec3f &lightColor, ArgParser *args) const;
  
protected:

  void ComputeAverageTextureColor();

  // REPRESENTATION
  Vec3f diffuseColor;
  Vec3f reflectiveColor;
  Vec3f emittedColor;
  double roughness;

  string textureFile;
  Image image;
};

// ====================================================================
// ====================================================================

#endif

