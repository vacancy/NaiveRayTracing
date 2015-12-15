/**
 * File   : random.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-12-01 11:17:33
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_RANDOM_H_
#define _INCLUDE_RANDOM_H_

namespace rt {

class RandomStream {
public:
	virtual double get() = 0;
protected:
	unsigned seed;
};

class LCGStream : public RandomStream {
public:
	LCGStream(unsigned s = 0) {
		seed = s;
	}

	inline double get() {
		seed = 214013u * seed + 2531011u;
		return seed *  (1.0 / 4294967296.0);
	}
};

} // end namespace rt

#endif
