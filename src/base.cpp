/**
 * File   : base.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-15 15:51:20
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/base.h"

namespace rt {

const Vector Vector::Zero(0, 0, 0);
const Vector Vector::XAxis(1, 0, 0);
const Vector Vector::YAxis(0, 1, 0);
const Vector Vector::ZAxis(0, 0, 1);

// {{{ Begin output operations
std::ostream &operator<<(std::ostream &os, const Vector &vec) {
    os << "Vector(" << vec.x << " " << vec.y << " " << vec.z << ")";
    return os;
}
// }}} End output Operations

}; // end namespace rt
