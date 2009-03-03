#include <cmath>

#include "raytracer.h"
#include "mesh.h"
#include "face.h"
#include "sphere.h"
#include "raytree.h"
#include "ray.h"

static double const SURFACE_EPSILON = 0.000000001;
static double const MIN_COLOR_LEN = 0.00000001;

// casts a single ray through the scene geometry and finds the closest hit
bool RayTracer::CastRay(const Ray &ray, Hit &h, bool use_sphere_patches) const
{
        bool answer = false;
        h.clear();

        // intersect each of the quads
        for (int i = 0; i < mesh->numQuadFaces(); i++) {
                Face *f = mesh->getFace(i);
                if (f->intersect(ray,h,args->intersect_backfacing)) answer = true;
        }

        // intersect each of the spheres (either the patches, or the original spheres)
        if (use_sphere_patches) {
                for (int i = mesh->numQuadFaces(); i < mesh->numFaces(); i++) {
                        Face *f = mesh->getFace(i);
                        if (f->intersect(ray,h,args->intersect_backfacing)) answer = true;
                }
        } else {
                const vector<Sphere*> &spheres = mesh->getSpheres();
                for (unsigned int i = 0; i < spheres.size(); i++) {
                        if (spheres[i]->intersect(ray,h)) answer = true;
                }
        }
        return answer;
}

// does the recursive (shadow rays & recursive/glossy rays) work
Vec3f RayTracer::TraceRay(const Ray &ray, Hit &hit, int bounce_count) const
{
        hit = Hit();
        bool intersect = CastRay(ray,hit,false);

        Vec3f answer = args->background_color_linear;
        const double STD_EPS = std::numeric_limits<double>::epsilon();

        if (intersect == true) {
                const Material *m = hit.getMaterial();
                assert (m != NULL);

                // rays coming from the light source are set to white, don't bother to ray trace further.
                if (m->getEmittedColor().Length() > 0.001) {
                        answer = Vec3f(1,1,1);
                } else {
                        // ambient light
                        answer = args->ambient_light_linear *
                                 m->getDiffuseColor(hit.get_s(),hit.get_t());
                        // Shadows
                        answer += shadows(ray, hit, m);

                        // Reflections
                        Vec3f reflectiveColor = m->getReflectiveColor();
                        double roughness = m->getRoughness();
                        if (bounce_count > 0 && reflectiveColor.Length() > MIN_COLOR_LEN) {
                        	answer += reflectiveColor * reflections(ray, hit, bounce_count);
                        }
                }
        }

        return answer;
}

Vec3f RayTracer::reflections(const Ray &ray, const Hit &hit, int bounce_count) const
{
	if (bounce_count <= 0)
		return Vec3f(0, 0, 0);
	const Vec3f point = ray.pointAtParameter(hit.getT());
	const Vec3f orig_dir = ray.getDirection();
	Vec3f norm = hit.getNormal();
	norm.Normalize();
	const Vec3f new_dir = orig_dir - 2 * orig_dir.Dot3(norm) * norm;

	Ray new_ray(point, new_dir);
	Hit new_hit;
	/* Non glossy reflections */
	Vec3f answer = TraceRay(new_ray, new_hit, bounce_count - 1);
	while (new_hit.getT() < SURFACE_EPSILON) {
		new_ray = Ray(new_ray.pointAtParameter(SURFACE_EPSILON),
				new_ray.getDirection());
		answer = TraceRay(new_ray, new_hit, bounce_count - 1);
	}
	RayTree::AddReflectedSegment(new_ray, 0, new_hit.getT());
	/* Glossy reflections */
	for (int i = 1; i < args->num_glossy_samples; ++i) {
		const Vec3f rand_vec(0.5 * (static_cast<double>(rand()) / RAND_MAX),
				0.5 * (static_cast<double>(rand()) / RAND_MAX),
				0.5 * (static_cast<double>(rand()) / RAND_MAX));
		Ray gloss_ray(new_ray.getOrigin(),
				new_ray.getDirection() + rand_vec);
		Hit gloss_hit;
		/* Compute reflection */
		Vec3f gloss_answer = TraceRay(gloss_ray, gloss_hit, bounce_count - 1);
		while (gloss_hit.getT() < SURFACE_EPSILON) {
			gloss_ray = Ray(gloss_ray.pointAtParameter(SURFACE_EPSILON),
					gloss_ray.getDirection());
			gloss_answer = TraceRay(gloss_ray,
					gloss_hit,
					bounce_count - 1);
		}
		answer += gloss_answer;
		RayTree::AddReflectedSegment(gloss_ray, 0, gloss_hit.getT());
	}
	answer *= static_cast<double>(1)/(args->num_glossy_samples + 1);
	return answer;
}

Vec3f RayTracer::shadows(const Ray &ray, const Hit &hit, const Material *m) const
{
	Vec3f answer(0,0,0);
	// ----------------------------------------------
	// add contributions from each light that is not in shadow
	const int num_lights = mesh->getLights().size();
	for (int i = 0; i < num_lights; i++) {
		const Face *f = mesh->getLights()[i];
		const Vec3f pointOnLight = f->computeCentroid();

	        const Vec3f point(ray.pointAtParameter(hit.getT()));
		answer += shadow(point, pointOnLight, f, m, ray, hit);
	}
	return answer;
}

Vec3f RayTracer::shadow(const Vec3f &point,
			const Vec3f &pointOnLight,
			const Face *f,
			const Material *m,
			const Ray &ray,
			const Hit &hit) const
{
        const Vec3f normal(hit.getNormal());

	Vec3f dirToLight = pointOnLight - point;
	dirToLight.Normalize();
	/* If dot product < 0, surface is not facing light */
	if (normal.Dot3(dirToLight) > 0) {
		Ray rayToLight(point, dirToLight);
		Hit hLight;
		bool blocked = CastRay(rayToLight, hLight, false);
		while (std::fabs(hLight.getT()) < SURFACE_EPSILON &&
				std::fabs((pointOnLight - point).Length()) > SURFACE_EPSILON) {
			rayToLight = Ray(rayToLight.pointAtParameter(SURFACE_EPSILON),
					dirToLight);
			blocked = CastRay(rayToLight, hLight, false);
		}
		RayTree::AddShadowSegment(rayToLight, 0, hLight.getT());
		if (hLight.getT() == FLT_MAX || hLight.getMaterial() != f->getMaterial()) {
			return Vec3f(0, 0, 0);
		}

		const Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();
		return m->Shade(ray,hit,dirToLight,lightColor,args);
	}
	return Vec3f(0, 0, 0);
}
