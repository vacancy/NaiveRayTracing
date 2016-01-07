/**
 * File   : camera
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:46
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_CAMERA_H
#define RAYTRACE_CAMERA_H

#include "../core/common.h"
#include "../core/vector3.h"
#include "../core/ray.h"
#include "../object/objbase.h"

namespace diorama {

class Camera {
public:
    Vector eye;
    Vector front, up, right;

    Camera(void) { }

    inline virtual Ray generate(double x, double y) = 0;
};

class PerspectiveCamera : public Camera {
public:
    double fov, fov_scale;

    PerspectiveCamera(void) { }

    PerspectiveCamera(const Vector &e, const Vector &f, const Vector &u, double fov_) {
        eye = e;
        front = f;
        up = u;
        right = cross(f, u);
        fov = fov_;
        fov_scale = tan(fov_ * 0.5 * pi / 180) * 2;
    }

    inline virtual Ray generate(double x, double y) {
        Vector r = right * ((x - 0.5) * fov_scale);
        Vector u = up * ((y - 0.5) * fov_scale);
        return Ray(eye, (front + r + u).norm());
    }
};

} // End namespace diorama

#endif //RAYTRACE_CAMERA_H
