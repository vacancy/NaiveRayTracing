/**
 * File   : object.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 10:44:30
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/object.h"
#include <iostream>

using std::vector;
using std::swap;
using std::cout;
using std::cerr;
using std::endl;

namespace rt {

const Intersection Intersection::null = Intersection();

Intersection Scene::intersect(const Ray &ray) {
    Intersection res = Intersection::null;
    for (Object *obj : objects) {
        Intersection tmp = obj->intersect(ray);
        if (tmp.object != NULL) {
            if (res.object == NULL || res.distance > tmp.distance) {
                res = tmp;
            }
        }
    }
    return res;
}

// {{{ Begin triangle intersection
inline static bool _sameside(const Vector &a, const Vector &b, const Vector &c, const Vector &p) {
    Vector ab = b - a, ac = c - a, ap = p - a;
    Vector v1 = cross(ab, ac);
    Vector v2 = cross(ab, ap);
    return dot(v1, v2) >= 0;
}

inline static bool _in_triangle(const Vector &p, const Triangle &t) {
    return _sameside(t.a, t.b, t.c, p) &&
           _sameside(t.b, t.c, t.a, p) &&
           _sameside(t.c, t.a, t.b, p);
}

Intersection Triangle::intersect(const Ray &ray) {
    double t = -(d + dot(norm, ray.origin)) / (dot(norm, ray.direct) + eps);
    if (t > bigeps) {
        Vector point = ray.get(t);
        bool inter = _in_triangle(point, *this);
        if (inter) {
            Intersection res;
            res.object = static_cast<Object *>(this);
            res.distance = t;
            res.position = point;
            res.norm = norm;
            return res;
        }
    }
    return Intersection::null;
}

Intersection TriangleMesh::intersect(const Ray &ray) {
    if (!_bbox.intersect(ray)) {
        return Intersection::null;
    }

    Intersection res = Intersection::null;
    for (Triangle *triangle : _triangles) {
        Intersection tmp = triangle->intersect(ray);
        if (tmp.object != NULL) {
            if (res.object == NULL || res.distance > tmp.distance)
                res = tmp;
        }
    }

    return res;
}

// }}} End triangle intersection

Intersection Sphere::intersect(const Ray &ray) {
    Vector v = center - ray.origin;
    double b = dot(v, ray.direct);
    double det = b * b - v.len2() + sqrrad;
    if (det > 0) {
        double sdet = sqrt(det);
        double distance = 0;
        if (b - sdet > bigeps)
            distance = b - sdet;
        else if (b + sdet > bigeps)
            distance = b + sdet;
        if (distance > bigeps) {
            Intersection res;
            res.object = static_cast<Object *>(this);
            res.distance = distance;
            res.position = ray.get(distance);
            res.norm = (res.position - center).norm();
            return res;
        }
    }

    return Intersection::null;
}

Intersection Plane::intersect(const Ray &ray) {
    double a = dot(norm, ray.direct);
    if (a >= -eps)
        return Intersection::null;

    double b = dot(norm, ray.origin - point);
    Intersection res;
    res.object = static_cast<Object *>(this);
    res.distance = -b / a;
    res.position = ray.get(res.distance);
    res.norm = norm;
    return res;
}

// {{{ Begin output operations
std::ostream &operator<<(std::ostream &os, const Triangle &tri) {
    os << "Triangle("
    << "(" << tri.a.x << " " << tri.a.y << " " << tri.a.z << ")" << ", "
    << "(" << tri.b.x << " " << tri.b.y << " " << tri.b.z << ")" << ", "
    << "(" << tri.c.x << " " << tri.c.y << " " << tri.c.z << ")"
    << ")";
    return os;
}
// }}} End output operations

} // end namespace rt
