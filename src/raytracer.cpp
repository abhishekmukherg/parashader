#include "raytracer.h"
#include "mesh.h"
#include "face.h"
#include "sphere.h"
#include "ray.h"

#include <cmath>

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

        Vec3f answer(args->background_color_linear);

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
                        answer += shadows(ray, hit);

                        // Reflections
                        Vec3f reflectiveColor = m->getReflectiveColor();
                        double roughness = m->getRoughness();
                        if (bounce_count > 0 && reflectiveColor.Length() > MIN_COLOR_LEN) {
                        	answer += reflectiveColor * reflections(ray, hit, bounce_count, roughness);
                        }
                }
        }

        return answer;
}

Vec3f RayTracer::reflections(const Ray &ray, const Hit &hit, int bounce_count, double roughness) const
{
	if (bounce_count <= 0)
		return Vec3f(0, 0, 0);
	const Vec3f point = ray.pointAtParameter(hit.getT());

	/* Get mirror direction */
	const Vec3f orig_dir = ray.getDirection();
	Vec3f norm = hit.getNormal();
	norm.Normalize();
	const Vec3f new_dir = orig_dir - 2 * orig_dir.Dot3(norm) * norm;

	const Ray new_ray(point, new_dir);
	Vec3f rand_vec(0, 0, 0);
	double answerx = 0, answery = 0, answerz = 0;
	/* sphere projection, what if the center of the sphere misses the
	 * object?
	 */

#pragma omp parallel shared(answerx,answery,answerz)
	{
#pragma omp for private(rand_vec) reduction(+:answerx,answery,answerz)
		for (int i = 0; i <= args->num_glossy_samples; ++i) {
			/* Getting gloss ray */
			Ray start_ray(new_ray.getOrigin(),
					new_ray.getDirection() + rand_vec);
			const Vec3f answer(reflection(start_ray, bounce_count - 1));

			answerx += answer.x();
			answery += answer.y();
			answerz += answer.z();
			rand_vec = Vec3f(roughness * (static_cast<double>(rand()) / RAND_MAX),
				roughness * (static_cast<double>(rand()) / RAND_MAX),
				roughness * (static_cast<double>(rand()) / RAND_MAX));
		}
	}
	Vec3f answer = Vec3f(answerx, answery, answerz);
	answer *= static_cast<double>(1)/(args->num_glossy_samples + 1);
	return answer;
}

Vec3f RayTracer::reflection(const Ray &start_ray, int bounce_count) const
{
	Ray ray(start_ray);
	Hit h;
	Vec3f answer = TraceRay(ray, h, bounce_count);
	while (h.getT() < SURFACE_EPSILON) {
		ray = Ray(ray.pointAtParameter(SURFACE_EPSILON),
				ray.getDirection());
		answer = TraceRay(ray, h, bounce_count);
	}
	return answer;
}

Vec3f RayTracer::shadows(const Ray &ray, const Hit &hit) const
{
	Vec3f answer(0, 0, 0);

	const int num_lights = mesh->getLights().size();
	if (args->num_shadow_samples == 0) {
		for (int i = 0; i < num_lights; ++i) {

			const Face *f = mesh->getLights()[i];
			Vec3f pointOnLight = f->computeCentroid();
			const Vec3f point(ray.pointAtParameter(hit.getT()));
			Vec3f dirToLight = pointOnLight - point;
			dirToLight.Normalize();
			const Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();
			return hit.getMaterial()->Shade(ray,hit,dirToLight,lightColor,args);
		}
	}

	// ----------------------------------------------
	// add contributions from each light that is not in shadow

	for (int i = 0; i < num_lights; i++) {
		const Face *f = mesh->getLights()[i];
		Vec3f pointOnLight = f->computeCentroid();
		const Vec3f point(ray.pointAtParameter(hit.getT()));

		double answerx = 0, answery = 0, answerz = 0;
#pragma omp parallel shared(answerx,answery,answerz)
		{
#pragma omp for private(pointOnLight) reduction(+:answerx,answery,answerz)
			for (int s = 0; s <= args->num_shadow_samples; ++s) {
				const Vec3f sh = shadow(point, pointOnLight, f, ray, hit);
				answerx += sh.x();
				answery += sh.y();
				answerz += sh.z();
				pointOnLight = f->RandomPoint();
			}
		}
		answer += Vec3f(answerx, answery, answerz);
	}
	answer *= static_cast<double>(1) / ((args->num_shadow_samples + 1) * num_lights);
	return answer;
}

Vec3f RayTracer::shadow(const Vec3f &point,
			const Vec3f &pointOnLight,
			const Face *f,
			const Ray &ray,
			const Hit &hit) const
{
        const Vec3f normal(hit.getNormal());
        const Material *m = hit.getMaterial();

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
#pragma omp critical
		if (hLight.getT() == FLT_MAX || hLight.getMaterial() != f->getMaterial()) {
			return Vec3f(0, 0, 0);
		}

		const Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();
		return m->Shade(ray,hit,dirToLight,lightColor,args);
	}
	return Vec3f(0, 0, 0);
}
