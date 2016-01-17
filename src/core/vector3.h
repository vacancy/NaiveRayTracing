/**
 * File   : vector3
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-07 12:33
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_VECTOR3_H
#define RAYTRACE_VECTOR3_H

#include "common.h"

namespace diorama {

class Vector {
public:
    typedef double value_t;

    union {
        struct {
            double x, y, z;
        };
        struct {
            double _values[3];
        };
    };

    Vector(void) { }

    Vector(double x_, double y_, double z_) : x(x_), y(y_), z(z_) { }

    inline double &operator[](int index) {
        return _values[index];
    }

    inline const double &operator[](int index) const {
        return _values[index];
    }

    inline double len(void) {
        return sqrt(x * x + y * y + z * z);
    }

    inline double l2(void) {
        return (x * x + y * y + z * z);
    }

    inline Vector norm(void) const {
        double len_inv = 1.0 / sqrt(x * x + y * y + z * z);
        return Vector(x * len_inv, y * len_inv, z * len_inv);
    }

    inline double mean(void) const {
        return (x + y + z) / 3;
    }

    inline double min(void) const {
        return (x < y ? (x < z ? x : z) : (y < z ? y : z));
    }

    inline double max(void) const {
        return (x > y ? (x > z ? x : z) : (y > z ? y : z));
    }

    static const Vector Zero;
    static const Vector XAxis;
    static const Vector YAxis;
    static const Vector ZAxis;
};

inline Vector operator+(const Vector &op0, const Vector &op1) {
    return Vector(op0.x + op1.x, op0.y + op1.y, op0.z + op1.z);
}

inline Vector operator-(const Vector &op0, const Vector &op1) {
    return Vector(op0.x - op1.x, op0.y - op1.y, op0.z - op1.z);
}

inline Vector &operator+=(Vector &op0, const Vector &op1) {
    op0.x += op1.x, op0.y += op1.y, op0.z += op1.z;
    return op0;
}

inline Vector &operator-=(Vector &op0, const Vector &op1) {
    op0.x -= op1.x, op0.y -= op1.y, op0.z -= op1.z;
    return op0;
}

inline Vector operator+(const Vector &op0, const double &op1) {
    return Vector(op0.x + op1, op0.y + op1, op0.z + op1);
}

inline Vector operator-(const Vector &op0, const double &op1) {
    return Vector(op0.x - op1, op0.y - op1, op0.z - op1);
}

inline Vector operator-(const Vector &op0) {
    return Vector(-op0.x, -op0.y, -op0.z);
}

inline Vector operator*(const Vector &op0, const Vector &op1) {
    return Vector(op0.x * op1.x, op0.y * op1.y, op0.z * op1.z);
}

inline Vector operator*(const Vector &op0, const double &op1) {
    return Vector(op0.x * op1, op0.y * op1, op0.z * op1);
}

inline Vector operator*(const double &op0, const Vector &op1) {
    return Vector(op1.x * op0, op1.y * op0, op1.z * op0);
}

inline Vector operator/(const Vector &op0, const double &op1) {
    return Vector(op0.x / op1, op0.y / op1, op0.z / op1);
}

inline Vector operator/(const Vector &op0, const Vector &op1) {
    return Vector(op0.x / op1.x, op0.y / op1.y, op0.z / op1.z);
}

inline double dot(const Vector &op0, const Vector &op1) {
    return op0.x * op1.x + op0.y * op1.y + op0.z * op1.z;
}

inline Vector cross(const Vector &op0, const Vector &op1) {
    return Vector(
        op0.y * op1.z - op0.z * op1.y,
        op0.z * op1.x - op0.x * op1.z,
        op0.x * op1.y - op0.y * op1.x
    );
}

inline Vector reflect(const Vector &in, const Vector &norm) {
    return in - norm * 2 * dot(norm, in);
}

inline std::ostream &operator<<(std::ostream &os, const Vector &vec) {
    os << "Vector(" << vec.x << " " << vec.y << " " << vec.z << ")";
    return os;
}

} // End namespace diorama

#endif //RAYTRACE_VECTOR3_H
