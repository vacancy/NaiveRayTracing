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

    inline double area() {
        return abs(cross(c - a, b - a).len()) / 2;
    }

    virtual Intersection intersect(const Ray &ray);
    virtual void sample(RandomStream *rng, Ray &ray, double &pdf);
};

class Quad : public Object {
public:
    Triangle tri1, tri2;
    double area1, area2;

    Quad(void) { }

    Quad(const Vector &a, const Vector &b, const Vector &c, const Vector &d) {
        tri1 = Triangle(a, b, c);
        tri2 = Triangle(a, c, d);
        initialize();
    }

    inline void initialize(void) {
        area1 = tri1.area();
        area2 = tri2.area();
    }

    virtual Intersection intersect(const Ray &ray) {
        Intersection inter = tri1.intersect(ray);
        if (inter.object != NULL) {
            inter.object = static_cast<Object *>(this);
            return inter;
        }
        inter = tri2.intersect(ray);
        if (inter.object != NULL)
            inter.object = static_cast<Object *>(this);
        return inter;
    }
    virtual void sample(RandomStream *rng, Ray &ray, double &pdf) {
        if (rng->get() < area1 / (area1 + area2)) tri1.sample(rng, ray, pdf);
        else tri2.sample(rng, ray, pdf);
    }
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

    inline double area() {
        return sqrrad * 4 * pi;
    }

    virtual Intersection intersect(const Ray &ray);
    virtual void sample(RandomStream *rng, Ray &ray, double &pdf);
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
    virtual void sample(RandomStream *rng, Ray &ray, double &pdf) {

    }
};

} // End namespace diorama

#endif //RAYTRACE_OBJDETAIL_H
