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

    inline virtual Ray generate(double x, double y, RandomStream *rng) = 0;
};

class PerspectiveCamera : public Camera {
public:
    double fov, fov_scale;

    PerspectiveCamera(void) { }

    PerspectiveCamera(const Vector &e, const Vector &f, const Vector &u, double fov_) {
        eye = e, front = f, up = u, right = cross(f, u);
        fov = fov_, fov_scale = tan(fov_ * 0.5 * pi / 180) * 2;
    }

    inline virtual Ray generate(double x, double y, RandomStream *rng) {
        Vector r = right * ((x - 0.5) * fov_scale);
        Vector u = up * ((y - 0.5) * fov_scale);
        return Ray(eye, (front + r + u).norm());
    }
};

class DoFCamera : public Camera {
public:
    double fov, fov_scale, lens_r, focus_distance;

    DoFCamera(void) { }

    DoFCamera(const Vector &e, const Vector &f, const Vector &u, double fov_, double lens_r_, double focus_distance_) {
        eye = e, front = f, up = u, right = cross(f, u);
        fov = fov_, fov_scale = tan(fov_ * 0.5 * pi / 180) * 2;
        lens_r = lens_r_, focus_distance = focus_distance_;
    }

    inline virtual Ray generate(double x, double y, RandomStream *rng) {
        Vector r = right * ((x - 0.5) * fov_scale);
        Vector u = up * ((y - 0.5) * fov_scale);
        Ray center_ray = Ray(eye, (front + r + u).norm());
        Ray disk_ray = rng->sample_disk(eye, lens_r, front);
//        disk_ray.direct = center_ray.direct;
        Vector look_at = center_ray.origin + center_ray.direct * focus_distance / dot(center_ray.direct, front);
        disk_ray.direct = (look_at - disk_ray.origin).norm();
        return disk_ray;
    }
};

} // End namespace diorama

#endif //RAYTRACE_CAMERA_H
