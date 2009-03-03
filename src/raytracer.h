#ifndef _RAY_TRACER_
#define _RAY_TRACER_

#include <assert.h>
#include "ray.h"
#include "hit.h"

#include <vector>

class Mesh;
class ArgParser;
class Radiosity;

// ====================================================================
// ====================================================================

class RayTracer
{

public:

        // CONSTRUCTOR & DESTRUCTOR
        RayTracer(Mesh *m, ArgParser *a) {
                mesh = m;
                args = a;
        }
        ~RayTracer() {}
        void setRadiosity(Radiosity *r) {
                radiosity = r;
        }

        // casts a single ray through the scene geometry and finds the closest hit
        bool CastRay(const Ray &ray, Hit &h, bool use_sphere_patches) const;

        // does the recursive work
        Vec3f TraceRay(const Ray &ray, Hit &hit, int bounce_count = 0) const;

private:

	Vec3f shadows(const Ray &ray, const Hit &hit, const Material *m) const;
	Vec3f shadow(const Vec3f &point, const Vec3f &pointOnLight, const Face *f, const Material *m, const Ray &ray, const Hit &hit) const;

        RayTracer() {
                assert(0);    // don't use this
        }

        // REPRESENTATION
        Mesh *mesh;
        ArgParser *args;
        Radiosity *radiosity;
};

// ====================================================================
// ====================================================================

#endif
