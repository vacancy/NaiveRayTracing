/**
 * File   : object.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 10:44:30
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_OBJECT_H_
#define _INCLUDE_OBJECT_H_

#include <cmath>
#include <vector>
#include <algorithm>

#include "base.h"
#include "material.h"

namespace rt {

class Intersection;
class Ray;
class Object;
class Scene;

class Ray {
public:
	Vector origin;
	Vector direct;

	Ray(void) {}
	Ray(const Vector &o, const Vector &d): origin(o), direct(d) {}
	Ray(double ox, double oy, double oz,
		double dx, double dy, double dz) {

		origin.x = ox, origin.y = oy, origin.z = oz;
		direct.x = dx, direct.y = dy, direct.z = dz;
	}

	inline Vector get(double t) const {
		return origin + direct * t;
	}
};

class Intersection {
public:
	Object *object;
	double distance;
	Vector position;
	Vector norm;

	Intersection(void) : object(NULL), distance(0) {}
	const static Intersection null;
};

class Scene {
public:
	std::vector<Object *> objects;
	Scene(void) {}

	inline Scene *add(Object *obj) {
		objects.push_back(obj);
		return this;
	}
	Intersection intersect(const Ray &ray);
};

class Object {
public:
	Material *material;

	virtual Intersection intersect(const Ray &ray) { return Intersection::null; }
	inline Object *set_material(Material *m) {
		material = m;
		return this;
	}
	inline Object *add_to_scene(Scene *s) {
		s->add(this);
		return this;
	}
};

class Triangle : public Object {
public:

	Vector a, b, c, norm, nnorm, center;
	double minx, maxx, miny, maxy, minz, maxz, d;

	Triangle(void) {}
	Triangle(const Vector &a_, const Vector &b_, const Vector &c_) : a(a_), b(b_), c(c_) {
		initialize();
	}
	Triangle(
		double ax, double ay, double az,
		double bx, double by, double bz,
		double cx, double cy, double cz
	) {
		a.x = ax, a.y = ay, a.z = az;
		b.x = bx, b.y = by, b.z = bz;
		c.x = cx, c.y = cy, c.z = cz;
		initialize();
	}
	inline void initialize(void) {
		#define tri_eps 1e-4
		norm = cross(b-a, c-a).norm();
		nnorm = -norm;
		center = (a + b + c) / 3;
		d = -dot(norm, a);
		minx = std::min(a.x, std::min(b.x, c.x))-tri_eps,
		maxx = std::max(a.x, std::max(b.x, c.x))+tri_eps;
		miny = std::min(a.y, std::min(b.y, c.y))-tri_eps,
		maxy = std::max(a.y, std::max(b.y, c.y))+tri_eps;
		minz = std::min(a.z, std::min(b.z, c.z))-tri_eps,
		maxz = std::max(a.z, std::max(b.z, c.z))+tri_eps;
		#undef tri_eps
	}

	Intersection intersect(const Ray &ray);
};

class Sphere : public Object {
public:
	Vector center;
	double radius;
	double sqrrad;

	Sphere(void) {}
	Sphere(const Vector &o, double r) : center(o), radius(r) {
		sqrrad = r*r;
	}
	Sphere(double x, double y, double z, double r) {
		center.x = x, center.y = y, center.z = z;
		radius = r, sqrrad = r*r;
	}

	Intersection intersect(const Ray &ray);
};

class Plane : public Object {
public:
	Vector point, norm;
	double d;

	Plane(void) {}
	Plane(const Vector &p, const Vector &n) : point(p), norm(n) {
		initialize();
	}
	inline void initialize(void) {
		d = -dot(point, norm);
	}

	Intersection intersect(const Ray &ray);
};

std::ostream &operator <<(std::ostream &os, const Triangle &tri);

} // end namespace rt

#endif
