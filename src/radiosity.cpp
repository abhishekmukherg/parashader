#include "radiosity.h"
#include "mesh.h"
#include "face.h"
#include "raytracer.h"
#include "glCanvas.h"

#include <cmath>
#ifndef M_PI
#warning Using more imprecise version of PI
static const double M_PI = 3.1415;
#endif

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

static const int NUM_RAYS = 4;
static const double FACE_EPSILON = 0.000001;

void CollectFacesWithVertex(Vertex *have, Face *f, vector<Face*> &faces);

// ================================================================
// CONSTRUCTOR & DESTRUCTOR
// ================================================================
Radiosity::Radiosity(Mesh *m, ArgParser *a) {
  mesh = m;
  args = a;
  num_faces = -1;
  formfactors = NULL;
  area = NULL;
  undistributed = NULL;
  absorbed = NULL;
  radiance = NULL;
  max_undistributed_patch = -1;
  total_area = -1;
  Reset();
}

Radiosity::~Radiosity() {
  Cleanup();
}

void Radiosity::Cleanup() {
  delete [] formfactors;
  delete [] area;
  delete [] undistributed;
  delete [] absorbed;
  delete [] radiance;
  num_faces = -1;
  formfactors = NULL;
  area = NULL;
  undistributed = NULL;
  absorbed = NULL;
  radiance = NULL;
  max_undistributed_patch = -1;
  total_area = -1;
}

void Radiosity::Reset() {
  delete [] area;
  delete [] undistributed;
  delete [] absorbed;
  delete [] radiance;

  // create and fill the data structures
  num_faces = mesh->numFaces();
  area = new double[num_faces];
  undistributed = new Vec3f[num_faces];
  absorbed = new Vec3f[num_faces];
  radiance = new Vec3f[num_faces];
  for (int i = 0; i < num_faces; i++) {
    Face *f = mesh->getFace(i);
    f->setRadiosityPatchIndex(i);
    setArea(i,f->getArea());
    Vec3f emit = f->getMaterial()->getEmittedColor();
    setUndistributed(i,emit);
    setAbsorbed(i,Vec3f(0,0,0));
    setRadiance(i,emit);
  }

  // find the patch with the most undistributed energy
  findMaxUndistributed();
}


void Radiosity::findMaxUndistributed() {
  // find the patch with the most undistributed energy
  // don't forget that the patches may have different sizes!
  max_undistributed_patch = -1;
  total_undistributed = 0;
  total_area = 0;
  double max = -1;
  for (int i = 0; i < num_faces; i++) {
    double m = getUndistributed(i).Length() * getArea(i);
    total_undistributed += m;
    total_area += getArea(i);
    if (max < m) {
      max = m;
      max_undistributed_patch = i;
    }
  }
  assert (max_undistributed_patch >= 0 && max_undistributed_patch < num_faces);
}


void Radiosity::ComputeFormFactors()
{
	assert(formfactors == NULL);
	assert(num_faces > 0);
	formfactors = new double[num_faces * num_faces];

	std::cout << "Calculating form factors" << std::endl;

#pragma omp parallel for
	for (int i = 0; i < num_faces; ++i) {
		const Face *f_i(mesh->getFace(i));
#pragma omp parallel for
		for (int j = 0; j < num_faces; ++j) {
			if (i == j) {
				setFormFactor(i, j, 0);
				continue;
			}
			const Face *f_j(mesh->getFace(j));
			setFormFactor(i, j, form_factor(f_i, f_j));
		}
	}
	std::cout << "Done calculating form factors" << std::endl;
}

double Radiosity::form_factor(const Face *f_i, const Face *f_j) const
{
	double value = 0;
	const Vec3f n_i = f_i->computeNormal();
	const Vec3f n_j = f_j->computeNormal();
	for (int i = 0; i < NUM_RAYS; ++i) {
		/* Get sample points */
		const Vec3f p_i(f_i->RandomPoint());
		const Vec3f p_j(f_j->RandomPoint());

		/* Connecting vector */
		Vec3f p_ji(p_i - p_j);
		p_ji.Normalize();
		const Vec3f p_ij(p_ji * -1);
		/* Angle between vector and normals of faces */
		const double ctheta_i(p_ij.CosAngleBetween(n_i));
		const double ctheta_j(p_ji.CosAngleBetween(n_j));
		/* Obtruse angles -> visibility is 0 */
		if (ctheta_i < 0 || ctheta_j < 0)
			continue;

		Hit h;
		/* Make sure the view is not obstructed */
		const Ray ray(p_i + (p_ij * FACE_EPSILON), p_ij);
		const bool intersect = raytracer->CastRay(ray, h, false);

		if (intersect && h.getMaterial() == f_j->getMaterial()) {
			const double len = (p_j - p_i).Length();
			value += (ctheta_i * ctheta_j) / (len * len);
		}
	}
	value = value * getArea(f_i->getRadiosityPatchIndex()) / (NUM_RAYS * M_PI);
	assert(value >= 0);
	return value;
}

// ================================================================
// ================================================================

double Radiosity::Iterate()
{
	if (formfactors == NULL)
		ComputeFormFactors();
	assert(formfactors != NULL);

	const int max_und = max_undistributed_patch;
	const Vec3f face_rad(getRadiance(max_und));
	for (int i = 0; i < num_faces; ++i) {
		const Vec3f new_r(getMesh()->getFace(i)->getMaterial()->getDiffuseColor() * getFormFactor(max_und, i) * getRadiance(max_und)
				+ getMesh()->getFace(i)->getMaterial()->getEmittedColor());
		setRadiance(i, new_r);
		setAbsorbed(i, getAbsorbed(i) + new_r);
	}
	setUndistributed(max_und, Vec3f(0,0,0));
	findMaxUndistributed();


	std::cout << "[";
	std::cout << "<" << undistributed[0].x() << "," << undistributed[0].y() <<
				"," << undistributed[0].z() << ">";
	for (int i = 1; i < num_faces; ++i) {
		std::cout << ", <" << undistributed[i].x() << "," << undistributed[i].y() <<
			"," << undistributed[i].z() << ">";
	}
	std::cout << "]" << std::endl;

	// fix this: return the total light yet undistributed
	// (so we can decide when the solution has sufficiently converged)
	return total_undistributed;

}

// =======================================================================
// PAINT
// =======================================================================

Vec3f Radiosity::whichVisualization(enum RENDER_MODE mode, Face *f, int i) {
  assert (mesh->getFace(i) == f);
  assert (i >= 0 && i < num_faces);
  if (mode == RENDER_LIGHTS) {
    return f->getMaterial()->getEmittedColor();
  } else if (mode == RENDER_UNDISTRIBUTED) {
    return getUndistributed(i);
  } else if (mode == RENDER_ABSORBED) {
    return getAbsorbed(i);
  } else if (mode == RENDER_RADIANCE) {
    return getRadiance(i);
  } else if (mode == RENDER_FORM_FACTORS) {
    if (formfactors == NULL) ComputeFormFactors();
    double scale = 0.2 * total_area/getArea(i);
    double factor = scale * getFormFactor(max_undistributed_patch,i);
    return Vec3f(factor,factor,factor);
  } else {
    assert(0);
  }
  exit(0);
}


// for interpolation
void CollectFacesWithVertex(Vertex *have, Face *f, vector<Face*> &faces) {
  for (unsigned int i = 0; i < faces.size(); i++) {
    if (faces[i] == f) return;
  }
  if (have != (*f)[0] && have != (*f)[1] && have != (*f)[2] && have != (*f)[3]) return;
  faces.push_back(f);
  for (int i = 0; i < 4; i++) {
    Edge *ea = f->getEdge()->getOpposite();
    Edge *eb = f->getEdge()->getNext()->getOpposite();
    Edge *ec = f->getEdge()->getNext()->getNext()->getOpposite();
    Edge *ed = f->getEdge()->getNext()->getNext()->getNext()->getOpposite();
    if (ea != NULL) CollectFacesWithVertex(have,ea->getFace(),faces);
    if (eb != NULL) CollectFacesWithVertex(have,eb->getFace(),faces);
    if (ec != NULL) CollectFacesWithVertex(have,ec->getFace(),faces);
    if (ed != NULL) CollectFacesWithVertex(have,ed->getFace(),faces);
  }
}

void Radiosity::insertColor(Vec3f v) {
  double r = linear_to_srgb(v.x());
  double g = linear_to_srgb(v.y());
  double b = linear_to_srgb(v.z());
  glColor3f(r,g,b);
}

void Radiosity::insertInterpolatedColor(int index, Face *f, Vertex *v) {
  vector<Face*> faces;
  CollectFacesWithVertex(v,f,faces);
  double total = 0;
  Vec3f color = Vec3f(0,0,0);
  Vec3f normal = f->computeNormal();
  for (unsigned int i = 0; i < faces.size(); i++) {
    Vec3f normal2 = faces[i]->computeNormal();
    double area = faces[i]->getArea();
    if (normal.Dot3(normal2) < 0.5) continue;
    assert (area > 0);
    total += area;
    color += area * whichVisualization(RENDER_RADIANCE,faces[i],faces[i]->getRadiosityPatchIndex());
  }
  assert (total > 0);
  color /= total;
  insertColor(color);
}

void Radiosity::Paint(ArgParser *args) {

  // this offset prevents "z-fighting" bewteen the edges and faces
  // the edges will always win.
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.1,4.0);

  if (args->render_mode == RENDER_MATERIALS) {
    // draw the faces with OpenGL lighting, just to understand the geometry
    // (the GL light has nothing to do with the surfaces that emit light!)
    for ( int i = 0; i < num_faces; i++) {
      Face *f = mesh->getFace(i);
      Material *m = f->getMaterial();

      Vec3f a = (*f)[0]->get();
      Vec3f b = (*f)[1]->get();
      Vec3f c = (*f)[2]->get();
      Vec3f d = (*f)[3]->get();
      Vec3f normal = f->computeNormal();
      glNormal3f(normal.x(),normal.y(),normal.z());

      if (!m->hasTextureMap()) {
	Vec3f color = m->getDiffuseColor();
	insertColor(color);
	glBegin (GL_QUADS);
	glVertex3f(a.x(),a.y(),a.z());
	glVertex3f(b.x(),b.y(),b.z());
	glVertex3f(c.x(),c.y(),c.z());
	glVertex3f(d.x(),d.y(),d.z());
	glEnd();
      } else {
	glEnable(GL_TEXTURE_2D);
	glColor3f(1,1,1);
	glBindTexture(GL_TEXTURE_2D,m->getTextureID());
	glBegin (GL_QUADS);
	glTexCoord2d((*f)[0]->get_s(),(*f)[0]->get_t());
	glVertex3f(a.x(),a.y(),a.z());
	glTexCoord2d((*f)[1]->get_s(),(*f)[1]->get_t());
	glVertex3f(b.x(),b.y(),b.z());
	glTexCoord2d((*f)[2]->get_s(),(*f)[2]->get_t());
	glVertex3f(c.x(),c.y(),c.z());
	glTexCoord2d((*f)[3]->get_s(),(*f)[3]->get_t());
	glVertex3f(d.x(),d.y(),d.z());
	glEnd();
	glDisable(GL_TEXTURE_2D);
      }
    }
  } else if (args->render_mode == RENDER_RADIANCE && args->interpolate == true) {
    // interpolate the radiance values with neighboring faces having the same normal
    glDisable(GL_LIGHTING);
    glBegin (GL_QUADS);
    for ( int i = 0; i < num_faces; i++) {
      Face *f = mesh->getFace(i);
      Vec3f a = (*f)[0]->get();
      Vec3f b = (*f)[1]->get();
      Vec3f c = (*f)[2]->get();
      Vec3f d = (*f)[3]->get();
      insertInterpolatedColor(i,f,(*f)[0]);
      glVertex3f(a.x(),a.y(),a.z());
      insertInterpolatedColor(i,f,(*f)[1]);
      glVertex3f(b.x(),b.y(),b.z());
      insertInterpolatedColor(i,f,(*f)[2]);
      glVertex3f(c.x(),c.y(),c.z());
      insertInterpolatedColor(i,f,(*f)[3]);
      glVertex3f(d.x(),d.y(),d.z());
    }
    glEnd();
    glEnable(GL_LIGHTING);
  } else {
    // for all other visualizations, just render the patch in a uniform color
    glDisable(GL_LIGHTING);
    glBegin (GL_QUADS);
    for ( int i = 0; i < num_faces; i++) {
      Face *f = mesh->getFace(i);
      Vec3f color = whichVisualization(args->render_mode,f,i);
      insertColor(color);
      Vec3f a = (*f)[0]->get();
      Vec3f b = (*f)[1]->get();
      Vec3f c = (*f)[2]->get();
      Vec3f d = (*f)[3]->get();
      glVertex3f(a.x(),a.y(),a.z());
      glVertex3f(b.x(),b.y(),b.z());
      glVertex3f(c.x(),c.y(),c.z());
      glVertex3f(d.x(),d.y(),d.z());
    }
    glEnd();
    glEnable(GL_LIGHTING);
  }

  if (args->render_mode == RENDER_FORM_FACTORS) {
    // render the form factors of the patch with the most undistributed light
    glDisable(GL_LIGHTING);
    glColor3f(1,0,0);
    Face *t = mesh->getFace(max_undistributed_patch);
    glLineWidth(3);
    glBegin(GL_LINES);
    Vec3f a = (*t)[0]->get();
    Vec3f b = (*t)[1]->get();
    Vec3f c = (*t)[2]->get();
    Vec3f d = (*t)[3]->get();
    glVertex3f(a.x(),a.y(),a.z());
    glVertex3f(b.x(),b.y(),b.z());
    glVertex3f(b.x(),b.y(),b.z());
    glVertex3f(c.x(),c.y(),c.z());
    glVertex3f(c.x(),c.y(),c.z());
    glVertex3f(d.x(),d.y(),d.z());
    glVertex3f(d.x(),d.y(),d.z());
    glVertex3f(a.x(),a.y(),a.z());
    glEnd();
    glEnable(GL_LIGHTING);
  }

  glDisable(GL_POLYGON_OFFSET_FILL);
  HandleGLError();

  if (args->wireframe) {
    mesh->PaintWireframe();
  }
}

