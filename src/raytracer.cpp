#include <cmath>

#include "raytracer.h"
#include "mesh.h"
#include "face.h"
#include "sphere.h"
#include "raytree.h"
#include "ray.h"

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
                        const Vec3f normal(hit.getNormal());
                        const Vec3f point(ray.pointAtParameter(hit.getT()));

                        // ----------------------------------------------
                        // ambient light

                        answer = args->ambient_light_linear *
                                 m->getDiffuseColor(hit.get_s(),hit.get_t());

                        // ----------------------------------------------
                        // add contributions from each light that is not in shadow
                        const int num_lights = mesh->getLights().size();
                        for (int i = 0; i < num_lights; i++) {
                                const Face *f = mesh->getLights()[i];
                                const Vec3f pointOnLight = f->computeCentroid();
                                Vec3f dirToLight = pointOnLight - point;
                                dirToLight.Normalize();
                                /* If dot product < 0, surface is not facing light */
                                if (normal.Dot3(dirToLight) > 0) {
					Ray rayToLight(point, dirToLight);
					Hit hLight;
					bool blocked = CastRay(rayToLight, hLight, false);
					while (std::fabs(hLight.getT()) < 0.0000001 &&
							std::fabs((pointOnLight - point).Length()) > STD_EPS) {
						rayToLight = Ray(rayToLight.pointAtParameter(0.00000001),
								dirToLight);
						blocked = CastRay(rayToLight, hLight, false);
					}
					RayTree::AddShadowSegment(rayToLight, 0, hLight.getT());
					if (hLight.getT() == FLT_MAX || hLight.getMaterial() != f->getMaterial()) {
						continue;
					}

                                        const Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();
                                        answer += m->Shade(ray,hit,dirToLight,lightColor,args);
                                }

                        }

                        // ----------------------------------------------
                        // add contribution from reflection, if the surface is shiny
                        Vec3f reflectiveColor = m->getReflectiveColor();
                        double roughness = m->getRoughness();


                        // ==========================================
                        // ASSIGNMENT:  ADD REFLECTIVE & GLOSSY LOGIC
                        // ==========================================


                }
        }

        return answer;
}
