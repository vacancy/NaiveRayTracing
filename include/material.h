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

enum REFLECT_T {
	REFL_DIFF,
	REFL_SPEC,
	REFL_REFR
};

class Material {

};

class Phong : public Material {
public:
	REFLECT_T reflect_type;
	Vector emission, color, c_color;
	double color_max;
	double shininess, reflectiveness;

	Phong(void) {}
	Phong(REFLECT_T refl_type, const Vector &c, const Vector &e)
		 : reflect_type(refl_type), color(c), emission(e) {

		color_max = std::max(color.x, std::max(color.y, color.z));
		c_color = color / color_max;
	}
};

} // end namespace rt

#endif
