/**
 * File   : random
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:39
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_RANDOM_H
#define RAYTRACE_RANDOM_H

namespace diorama {

class RandomStream {
public:
    virtual double get() = 0;

protected:
    unsigned seed;
};

class LCGStream : public RandomStream {
public:
    LCGStream(unsigned s = 12345) {
        seed = s;
    }

    virtual inline double get() {
        seed = 214013u * seed + 2531011u;
        return seed * (1.0 / 4294967296.0);
    }
};

}

#endif //RAYTRACE_RANDOM_H
