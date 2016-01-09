/**
 * File   : canvas
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:36
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_CANVAS_H
#define RAYTRACE_CANVAS_H

#include "common.h"
#include <opencv2/core/core.hpp>

namespace diorama {

class Canvas {
public:
    uint8 *mat;
    int h, w, c;
    int st0, st1, size;

    Canvas(void) { }

    Canvas(int h_, int w_, int c_) : h(h_), w(w_), c(c_) {
        mat = new uint8[h_ * w_ * c_];
        st1 = c, st0 = c * w, size = c * w * h;
        memset(mat, 0, size * sizeof(uint8));
    }

    inline uint8 &get(int x, int y, int c = 0) {
        return mat[x * st0 + y * st1 + c];
    }

    inline void set(int x, int y, int c, uint8 v) {
        mat[x * st0 + y * st1 + c] = v;
    }

    void show(const std::string &, bool wait = true);

    void write(const std::string &);

protected:
    cv::Mat _to_mat(void);
};

} // End namespace diorama

#endif //RAYTRACE_CANVAS_H_H
