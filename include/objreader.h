/**
 * File   : objreader.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-12-23 23:45:00
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_OBJREADER_H_
#define _INCLUDE_OBJREADER_H_

#include "object.h"
#include <string>

namespace rt {

class ObjReader {
public:
	ObjReader(std::string filename) {
		_filename = filename;
	}
	void process(Scene *scene, Material *material);

protected:
	std::string _filename;
};

} // end namespace rt

#endif
