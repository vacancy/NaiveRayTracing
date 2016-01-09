/**
 * File   : random
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:39
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_RANDOM_H
#define RAYTRACE_RANDOM_H

#include "vector3.h"
#include "ray.h"

namespace diorama {

class RandomStream {
public:
    virtual double get() = 0;

    inline Vector sample_hemisphere(const Vector &norm) {
        double r1 = 2 * pi * get();
        double r2 = get();
        double r2s = sqrt(r2);

        Vector w = norm;
        Vector wo = w.x < -0.1 || w.x > 0.1 ? Vector::YAxis : Vector::XAxis;
        Vector u = cross(wo, w).norm();
        Vector v = cross(w, u);
        return (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
    }

    inline Ray sample_sphere(const Vector &center, double r) {
        double s = 2.0 * get() - 1.0;
        double c = sqrt(1.0 - s * s);
        double p = 2.0 * pi * get();
        double x = c * cos(p);
        double y = c * sin(p);
        double z = s;

        Ray result;
        result.direct = Vector(x, y, z);
        result.origin = r * result.direct + center;
        return result;
    }

    inline Ray sample_triangle(const Vector &a, const Vector &b, const Vector &c, const Vector &norm) {
        double r1 = get(), r2 = get();
        if (r1 + r2 >= 1.0) {
            r1 = 1.0 - r1;
            r2 = 1.0 - r2;
        }

        Ray result;
        result.origin = a +  r1 * (b - a) + r2 * (c - a);
        result.direct = norm;
        return result;
    }

};

class LCGStream : public RandomStream {
public:
    LCGStream(unsigned s = 12345) {
        seed = s;
    }

    virtual inline double get() {
        seed = 214013u * seed + 2531011u;
        return seed * (1.0 / 4294967296.0);
    }

protected:
    unsigned seed;
};

}

#endif //RAYTRACE_RANDOM_H
