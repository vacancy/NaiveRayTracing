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

#include "base.h"
#include "material.h"

#include <cmath>
#include <vector>
#include <algorithm>

namespace rt {

class Ray;

class Intersection;

class BoundingBox;

class Object;

class Scene;

class Ray {
public:
    Vector origin;
    Vector direct;

    Ray(void) { }

    Ray(const Vector &o, const Vector &d) : origin(o), direct(d) { }

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

    Intersection(void) : object(NULL), distance(0) { }

    const static Intersection null;
};

class Scene {
public:
    std::vector<Object *> objects;

    Scene(void) { }

    inline Scene *add(Object *obj) {
        objects.push_back(obj);
        return this;
    }

    Intersection intersect(const Ray &ray);
};

class BoundingBox {
public:
    Vector vmin, vmax;

    BoundingBox(void) { }

    BoundingBox(Vector vmin, Vector vmax) : vmin(vmin), vmax(vmax) {
        initialize();
    }

    inline void initialize(void) {

    }

    inline Pair get_near_far(const Ray &ray) {
        Vector result_min = vmin - ray.origin;
        Vector result_max = vmax - ray.origin;
        result_min = result_min / ray.direct;
        result_max = result_max / ray.direct;
        for (int i = 0; i < 3; ++i)
            if (result_min[i] > result_max[i])
                std::swap(result_min[i], result_max[i]);
        return Pair(result_min.max(), result_max.min());
    }

    inline bool intersect(const Ray &ray) {
        Pair near_far = get_near_far(ray);
        return !(near_far.first > near_far.second || near_far.second < 0);
    }
};

class Object {
public:
    Material *material;

    Object(void) : material(NULL) {

    }

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
    double d;

    Triangle(void) { }

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
        norm = cross(b - a, c - a).norm();
        nnorm = -norm;
        center = (a + b + c) / 3;
        d = -dot(norm, a);
    }

    virtual Intersection intersect(const Ray &ray);
};

class ObjKDTree;

class ObjReader;

class TriangleMesh : public Object {
    friend class ObjKDTree;

    friend class ObjReader;

public:
    typedef std::vector<Triangle *> TriangleVector;
    typedef std::vector<Vector> VertexVector;

    TriangleMesh(void) { }

    TriangleMesh(TriangleVector triangles, BoundingBox bbox)
            : _triangles(triangles), _bbox(bbox) {

    }

    TriangleMesh(TriangleVector triangles, VertexVector vertexes)
            : _triangles(triangles) {

        initialize(vertexes);
    }

    inline void initialize(VertexVector vertexes) {
        Vector vmin(inf, inf, inf), vmax(-inf, -inf, -inf);
        for (auto v : vertexes) {
            if (v.x < vmin.x) vmin.x = v.x;
            if (v.y < vmin.y) vmin.y = v.y;
            if (v.z < vmin.z) vmin.z = v.z;

            if (v.x > vmax.x) vmax.x = v.x;
            if (v.y > vmax.y) vmax.y = v.y;
            if (v.z > vmax.z) vmax.z = v.z;
        }
        _bbox = BoundingBox(vmin, vmax);
    }

    virtual Intersection intersect(const Ray &ray);

private:
    std::vector<Triangle *> _triangles;
    BoundingBox _bbox;
};

class Sphere : public Object {
public:
    Vector center;
    double radius;
    double sqrrad;

    Sphere(void) { }

    Sphere(const Vector &o, double r) : center(o), radius(r) {
        sqrrad = r * r;
    }

    Sphere(double x, double y, double z, double r) {
        center.x = x, center.y = y, center.z = z;
        radius = r, sqrrad = r * r;
    }

    virtual Intersection intersect(const Ray &ray);
};

class Plane : public Object {
public:
    Vector point, norm;
    double d;

    Plane(void) { }

    Plane(const Vector &p, const Vector &n) : point(p), norm(n) {
        initialize();
    }

    inline void initialize(void) {
        d = -dot(point, norm);
    }

    virtual Intersection intersect(const Ray &ray);
};

std::ostream &operator<<(std::ostream &os, const Triangle &tri);

} // end namespace rt

#endif
