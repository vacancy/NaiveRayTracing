/**
 * File   : ray
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-07 13:14
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_RAY_H
#define RAYTRACE_RAY_H

namespace diorama {

class Ray {
public:
    Vector origin;
    Vector direct;

    Ray(void) { }

    Ray(const Vector &o, const Vector &d) : origin(o), direct(d) { }

    Ray(double ox, double oy, double oz,
        double dx, double dy, double dz) {

        origin.x = ox, origin.y = oy, origin.z = oz;
        direct.x = dx, direct.y = dy, direct.z = dz;
    }

    inline Vector get(double t) const {
        return origin + direct * t;
    }
};

} // End namespace diorama

#endif //RAYTRACE_RAY_H
