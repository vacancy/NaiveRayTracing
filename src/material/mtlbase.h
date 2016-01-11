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

#include "bsdf.h"
#include "../core/common.h"
#include "../core/vector3.h"
#include "../core/ray.h"
#include "../core/random.h"

namespace diorama {

class Material {
public:
    virtual BSDF *get_bsdf(const Vector &pos, const Vector &norm) const = 0;
};

class SimpleMaterial : public Material {
public:
    SimpleMaterial(BSDF *const bsdf = NULL) : _bsdf(bsdf) {

    }
    virtual BSDF *get_bsdf(const Vector &pos, const Vector &norm) const override {
        return _bsdf;
    }
    virtual void set_bsdf(BSDF *const bsdf) {
        _bsdf = bsdf;
    }
private:
    BSDF *_bsdf;
};

class Light {
public:
    virtual Vector get_emission(const Vector &pos, const Vector &direct) const = 0;
};

class SimpleLight : public Light {
public:
    SimpleLight(const Vector &emission = Vector::Zero) : _emission(emission) {

    }
    virtual inline Vector get_emission(const Vector &pos, const Vector &direct) const {
        return _emission;
    }
private:
    Vector _emission;
};

} // End namespace diorama

#endif //RAYTRACE_MTLBASE_H
