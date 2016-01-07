/**
 * File   : objdetail
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 13:15
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_OBJDETAIL_H
#define RAYTRACE_OBJDETAIL_H

#include "objbase.h"

namespace diorama {

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

std::ostream &operator<<(std::ostream &os, const Triangle &tri);

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

} // End namespace diorama

#endif //RAYTRACE_OBJDETAIL_H
