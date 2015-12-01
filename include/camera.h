/**
 * File   : camera.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 12:04:19
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_CAMERA_H_
#define _INCLUDE_CAMERA_H_

#include "object.h"
#include <iostream>

namespace rt {

using namespace std;

class Camera {
public:
	Vector eye;
	Vector front, up, right;

	Camera(void) {}
	inline virtual Ray generate(double x, double y) = 0;
};

class PerspectiveCamera : public Camera {
public:
	double fov, fov_scale;

	PerspectiveCamera(void) {}
	PerspectiveCamera(const Vector &e, const Vector &f, const Vector &u, double fov_) {
		eye = e;
		front = f;
		up = u; 
		right = cross(f, u);
		fov = fov_;
		fov_scale = tan(fov_ * 0.5 * PI / 180) * 2;
	}
	inline virtual Ray generate(double x, double y) {
		Vector r = right * ((x - 0.5) * fov_scale);
		Vector u = up * ((y - 0.5) * fov_scale);
		return Ray(eye, (front + r + u).norm());
	}
};

} // end namespace rt

#endif
