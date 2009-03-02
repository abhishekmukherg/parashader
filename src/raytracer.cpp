#include "raytracer.h"
#include "material.h"
#include "vectors.h"
#include "argparser.h"
#include "raytree.h"
#include "utils.h"
#include "mesh.h"
#include "face.h"
#include "sphere.h"


// casts a single ray through the scene geometry and finds the closest hit
bool RayTracer::CastRay(Ray &ray, Hit &h, bool use_sphere_patches) const
{
        bool answer = false;

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
Vec3f RayTracer::TraceRay(Ray &ray, Hit &hit, int bounce_count) const
{

        hit = Hit();
        bool intersect = CastRay(ray,hit,false);

        Vec3f answer = args->background_color_linear;

        if (intersect == true) {
                Material *m = hit.getMaterial();
                assert (m != NULL);

                // rays coming from the light source are set to white, don't bother to ray trace further.
                if (m->getEmittedColor().Length() > 0.001) {
                        answer = Vec3f(1,1,1);
                }

                else {

                        Vec3f normal = hit.getNormal();
                        Vec3f point = ray.pointAtParameter(hit.getT());

                        // ----------------------------------------------
                        // ambient light
                        answer = args->ambient_light_linear *
                                 m->getDiffuseColor(hit.get_s(),hit.get_t());

                        // ----------------------------------------------
                        // add contributions from each light that is not in shadow
                        int num_lights = mesh->getLights().size();
                        for (int i = 0; i < num_lights; i++) {


                                // =============================
                                // ASSIGNMENT:  ADD SHADOW LOGIC
                                // =============================

                                Face *f = mesh->getLights()[i];
                                Vec3f pointOnLight = f->computeCentroid();
                                Vec3f dirToLight = pointOnLight - point;
                                dirToLight.Normalize();
                                if (normal.Dot3(dirToLight) > 0) {
                                        Vec3f lightColor = 0.2 * f->getMaterial()->getEmittedColor() * f->getArea();
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
