/**
 * File   : canvas.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 10:44:30
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_CANVAS_H_
#define _INCLUDE_CANVAS_H_

#include <cstring>
#include <string>
#include <opencv2/core/core.hpp>

namespace rt {

typedef unsigned char uint8;

class Canvas {
public:
	uint8 *mat;
	int h, w, c;
	int st0, st1, size;

	Canvas(void) {}
	Canvas(int h_, int w_, int c_) : h(h_), w(w_), c(c_) {
		mat = new uint8[h_*w_*c_];
		st1 = c, st0 = c*w, size = c*w*h;
		memset(mat, 0, size*sizeof(uint8));
	}
	inline uint8 &get(int x, int y, int c=0) {
		return mat[x*st0 + y*st1 + c];
	}
	inline void set(int x, int y, int c, int v) {
		mat[x*st0 + y*st1 + c] = v;
	}

	void show(void);
	void write(const std::string &);

protected:
	cv2::Mat _to_mat(void);
};

} // end namespace rt

#endif
