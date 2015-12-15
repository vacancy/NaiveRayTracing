/**
 * File   : trace.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 11:51:02
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_TRACE_H_
#define _INCLUDE_TRACE_H_

#include "base.h"
#include "random.h"

namespace rt {

class Tracer {

};

class PathTracer : public Tracer {
public:
	PathTracer(int max_depth = 10, unsigned seed = 19961018) {
		_rng = new LCGStream(seed);
		_max_depth = max_depth;
	}
	Vector trace(Scene *scene, const Ray &ray, int depth);
protected:
	LCGStream *_rng;
	int _max_depth;
};

} // end namespace rt

#endif
