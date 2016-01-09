/**
 * File   : mtlbase
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:44
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_MTLBASE_H
#define RAYTRACE_MTLBASE_H

#include "../core/common.h"
#include "../core/vector3.h"
#include "../core/ray.h"
#include "../core/random.h"

namespace diorama {

class Material {
public:
    Material(const Vector &color, const Vector &emission) : color(color), emission(emission) {
        max_color = color.max();
        normed_color = color / max_color;
    }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                       Ray &out, double &pdf) = 0;

    Vector color, emission, normed_color;
    double max_color;
};

} // End namespace diorama

#endif //RAYTRACE_MTLBASE_H
