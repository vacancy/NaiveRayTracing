/**
 * File   : common
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-07 12:31
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_COMMON_H
#define RAYTRACE_COMMON_H

#include <cassert>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <utility>
#include <vector>
#include <string>
#include <iostream>

namespace diorama {

const double pi = 3.14159265358979323;
const double inv_pi = 0.318309886;
const double eps = 1e-6;
const double bigeps = 1e-4;
const double inf = 1e20;

typedef unsigned char uint8;
typedef unsigned int  uint32;
typedef float  float32;
typedef double float64;

using std::max;
using std::min;
using std::pair;

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

template <typename T>
inline T abs(const T &a) {
    return a < 0 ? -a : a;
}

template <int times>
inline double expand_pow(double base) {
    double res = base;
#pragma unroll
    for (int i = 1; i < times; ++i)
        res = res * base;
    return res;
}

inline double int_pow(double base1, int times) {
    if (times == 1)  return base1;
    if (times == 2)  return base1 * base1;
    if (times == 3)  return base1 * base1 * base1;
    double base2 = base1 * base1;
    if (times == 4)  return base2 * base2;
    if (times == 5)  return base2 * base2 * base1;
    if (times == 6)  return base2 * base2 * base2;
    double base4 = base2 * base2;
    if (times == 7)  return base4 * base2 * base1;
    if (times == 8)  return base4 * base4;
    if (times == 9)  return base4 * base4 * base1;
    if (times == 10) return base4 * base4 * base2;

    for (int i = 4; i < times; ++i) {
        base4 *= base1;
    }
    return base4;
}

} // End namespace diorama

#endif //RAYTRACE_COMMON_H
