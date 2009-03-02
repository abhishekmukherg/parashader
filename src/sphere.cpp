#include "sphere.h"

#include <limits>

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

bool Sphere::intersect(const Ray &r, Hit &h) const
{
	const Vec3f &dir = r.getDirection();
	const Vec3f ori(r.getOrigin() - center);

	const double a = dir.Dot3(dir);
	const double b = 2 * dir.Dot3(ori);
	const double c = ori.Dot3(ori) - radius * radius;
	
	const double d = b * b - 4 * a * c;

	if (d < std::numeric_limits<double>::epsilon())
		return false;
	
	assert(fabs(d) > std::numeric_limits<double>::epsilon());

	const double t0 = (-b + d) / (2 * a);
	const double t1 = (-b - d) / (2 * a);
	assert(t0 >= t1);

	const double tf = (t1 >= 0) ? t1 : t0;
	if (tf < std::numeric_limits<double>::epsilon() || tf >= h.getT())
		return false;

	const Vec3f point(r.pointAtParameter(tf));
	const Vec3f normal(point - center);
	assert(normal.Length() > 0);
	h.set(tf, material, normal);
	h.setTextureCoords(0, 0);
	/* FIXME: missing setTextureCoordinates */
	return true;
} 

// ====================================================================
// ====================================================================
