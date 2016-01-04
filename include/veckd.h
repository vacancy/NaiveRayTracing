/**
 * File   : vector.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-01-04 10:34:59
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_VECTOR_KD_H_
#define _INCLUDE_VECTOR_KD_H_

#include "base.h"
#include "kdbase.h"
#include <vector>

namespace rt {

struct VecKDNode : KDNodeBase<VecKDNode> {
    VecKDNode(void) : KDNodeBase<VecKDNode>() { }
    std::vector<Vector> vectors;
};

class VecKDTree {
    
};

} // end namespace rt


#endif
