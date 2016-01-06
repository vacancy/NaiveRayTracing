/**
 * File   : material.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 10:44:30
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_MATERIAL_H_
#define _INCLUDE_MATERIAL_H_

#include "base.h"
#include <cmath>
#include <vector>
#include <algorithm>

namespace rt {

class Material {

};

class Phong : public Material {
public:
    Vector emission, color, c_color;
    double color_max;
    double k_diff, k_spec;
    double beta = 1.5;
    int  n_spec;
    bool have_refr;

    Phong(void) { }

    Phong(const Vector &c, const Vector &e, double k_diff, double k_spec=0, int n_spec=1, bool have_refr=false)
            : k_diff(k_diff), k_spec(k_spec), n_spec(n_spec), color(c), emission(e), have_refr(have_refr) {

        color_max = std::max(color.x, std::max(color.y, color.z));
        c_color = color / color_max;
    }
};

} // end namespace rt

#endif
