#include "material.h"
#include "utils.h"

Material::~Material() {
}


const Vec3f Material::getDiffuseColor(double s, double t) const {
  if (!hasTextureMap()) return diffuseColor; 

  // this is just using nearest neighbor and could be improved to
  // bilinear interpolation, etc.
  int i = int(s * image.Width()) % image.Width();
  int j = int(t * image.Height()) % image.Height();
  if (i < 0) i += image.Width();
  if (j < 0) j += image.Height();
  assert (i >= 0 && i < image.Width());
  assert (j >= 0 && j < image.Height());
  Color c = image.GetPixel(i,j);

  // we assume the texture is stored in sRGB and convert to linear for
  // computation.  It will be converted back to sRGB before display.
  double r = srgb_to_linear(c.r/255.0);
  double g = srgb_to_linear(c.g/255.0);
  double b = srgb_to_linear(c.b/255.0);

  return Vec3f(r,g,b);
}

void Material::ComputeAverageTextureColor() {
  assert (hasTextureMap());
  double r = 0;
  double g = 0;
  double b = 0;
  for (int i = 0; i < image.Width(); i++) {
    for (int j = 0; j < image.Height(); j++) {
      Color c = image.GetPixel(i,j);
       r += srgb_to_linear(c.r/255.0);
       g += srgb_to_linear(c.g/255.0);
       b += srgb_to_linear(c.b/255.0);
    }
  }
  int count = image.Width() * image.Height();
  r /= double(count);
  g /= double(count);
  b /= double(count);
  diffuseColor = Vec3f(r,g,b);
}


// PHONG LOCAL ILLUMINATION

// this function should be called to compute the light contributed by
// a particular light source to the intersection point.  Note that
// this function does not calculate any global effects (e.g., shadows). 

Vec3f Material::Shade(const Ray &ray, const Hit &hit, 
                      const Vec3f &dirToLight, 
                      const Vec3f &lightColor, ArgParser *args) const {
  
  Vec3f point = ray.pointAtParameter(hit.getT());
  Vec3f n = hit.getNormal();
  Vec3f e = ray.getDirection()*-1.0f;
  Vec3f l = dirToLight;
  
  Vec3f answer = Vec3f(0,0,0);

  // emitted component
  // -----------------
  answer += 0.2*getEmittedColor();

  // diffuse component
  // -----------------
  double dot_nl = n.Dot3(l);
  if (dot_nl < 0) dot_nl = 0;
  answer += lightColor * getDiffuseColor(hit.get_s(),hit.get_t()) * dot_nl;

  // specular component (Phong)
  // ------------------
  // make up reasonable values for other Phong parameters
  Vec3f specularColor = 0.8*reflectiveColor;
  double exponent = 5/(roughness+0.01);
  // compute ideal reflection angle
  Vec3f r = (l*-1.0f) + n * (2 * dot_nl);
  r.Normalize();
  double dot_er = e.Dot3(r);
  if (dot_er < 0) dot_er = 0;
  answer += lightColor*specularColor*pow(dot_er,exponent)* dot_nl;

  return answer;

}
