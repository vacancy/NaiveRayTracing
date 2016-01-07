/**
 * File   : common
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:31
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_COMMON_H
#define RAYTRACE_COMMON_H

#include <cassert>
#include <cstring>
#include <cmath>
#include <utility>
#include <vector>
#include <string>
#include <iostream>

namespace diorama {

const double pi = 3.14159265358979323;
const double eps = 1e-6;
const double bigeps = 1e-4;
const double inf = 1e20;

typedef unsigned char uint8;
typedef unsigned int  uint32;
typedef float  float32;
typedef double float64;

inline double sqr(const double x) {
    return x * x;
}

typedef std::pair<double, double> Pair;

inline double clamp(double x) {
    if (x < 0) return 0;
    else if (x > 1) return 1;
    else return x;
}

inline double clamp_int(double x) {
    //return int(clamp(x) * 255. + 0.5);
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

} // End namespace diorama

#endif //RAYTRACE_COMMON_H
