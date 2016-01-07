/**
 * File   : lightbase
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 20:19
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_LIGHTBASE_H
#define RAYTRACE_LIGHTBASE_H

#include "../object/objbase.h"
#include "../object/objdetail.h"

namespace diorama {

class Light {
public:
    virtual Ray sample(void);
private:

};

class SphereLight : Light {
public:
    SphereLight(const Vector &position, double r, const Vector &emission)
            : _position(position), _radius(r), _emission(emission) {

    }

    SphereLight(const Sphere &sphere, const Vector &emission)
            : _position(sphere.center), _radius(sphere.radius), _emission(emission) {

    }

    virtual Ray sample(void) {

    }


private:
    Vector _position;
    Vector _emission;
    double _radius;
};

} // End namespace diorama

#endif //RAYTRACE_LIGHTBASE_H
