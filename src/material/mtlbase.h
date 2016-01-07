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

namespace diorama {

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

} // End namespace diorama

#endif //RAYTRACE_MTLBASE_H
