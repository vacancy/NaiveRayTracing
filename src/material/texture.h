/**
 * File   : texture
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-11 12:58
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_TEXTURE_H_H
#define RAYTRACE_TEXTURE_H_H

#include "../core/vector3.h"

namespace diorama {

template <typename T>
class Texture {
public:
    typedef T value_t;

    virtual T get(const Vector &pos, const Vector &norm) const = 0;
};

template <typename T>
class ConstantTexture : public Texture<T> {
public:
    typedef T value_t;

    ConstantTexture(const value_t &c) : _const(c) {

    }
    virtual T get(const Vector &pos, const Vector &norm) const override {
        return _const;
    }

private:
    T _const;
};

typedef Texture<Vector> VectorTexture;
typedef Texture<float>  FloatTexture;

} // End namespace diorama

#endif //RAYTRACE_TEXTURE_H_H
