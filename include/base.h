/**
 * File   : base.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-15 15:51:20
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_VECTOR_H_
#define _INCLUDE_VECTOR_H_

#include <cmath>
#include <iostream>

namespace rt {

const double PI = 3.14159265358979323;
const double eps = 1e-6;

inline double sqr(const double &x) {
	return x*x;
}

class Vector {
public:

	double x, y, z;
	Vector(void) {}
	Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
	//Vector(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

	inline double len(void) {
		return sqrt(x * x + y * y + z * z);
	}
	inline double len2(void) {
		return (x * x + y * y + z * z);
	}
	inline Vector norm(void) const {
		double len_inv = 1.0 / sqrt(x * x + y * y + z * z);
		return Vector(x*len_inv, y*len_inv, z*len_inv);
	}

	static const Vector Zero;
	static const Vector XAxis;
	static const Vector YAxis;
	static const Vector ZAxis;
};

// {{{ Begin vector operations
inline double dot(const Vector &op0, const Vector &op1) {
	return op0.x*op1.x + op0.y*op1.y + op0.z*op1.z;
}
inline Vector cross(const Vector &op0, const Vector &op1) {
	return Vector(
		op0.y * op1.z - op0.z * op1.y, 
		op0.z * op1.x - op0.x * op1.z,
		op0.x * op1.y - op0.y * op1.x
	);
}
inline Vector operator +(const Vector &op0, const Vector &op1) {
	return Vector(op0.x + op1.x, op0.y + op1.y, op0.z + op1.z);
}
inline Vector operator -(const Vector &op0, const Vector &op1) {
	return Vector(op0.x - op1.x, op0.y - op1.y, op0.z - op1.z);
}
inline Vector operator +(const Vector &op0, const double &op1) {
	return Vector(op0.x + op1, op0.y + op1, op0.z + op1);
}
inline Vector operator -(const Vector &op0, const double &op1) {
	return Vector(op0.x - op1, op0.y - op1, op0.z - op1);
}
inline Vector operator -(const Vector &op0) {
	return Vector(-op0.x, -op0.y, -op0.z);
}
inline Vector operator *(const Vector &op0, const Vector &op1) {
	return Vector(op0.x * op1.x, op0.y * op1.y, op0.z * op1.z);
}
inline Vector operator *(const Vector &op0, const double &op1) {
	return Vector(op0.x * op1, op0.y * op1, op0.z * op1);
}
inline Vector operator *(const double &op0, const Vector &op1) {
	return Vector(op1.x * op0, op1.y * op0, op1.z * op0);
}
inline Vector operator /(const Vector &op0, const double &op1) {
	return Vector(op0.x / op1, op0.y / op1, op0.z / op1);
}
// }}} End vector operations

inline double clamp(double x) { 
	if (x < 0) return 0;
	else if (x > 1) return 1;
	else return x;
}

inline double clamp_int(double x) {
	return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}
std::ostream &operator <<(std::ostream &os, const Vector &vec);

} // end namespace rt

#endif
