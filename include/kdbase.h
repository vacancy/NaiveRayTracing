/**
 * File   : kdbase.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-01-04 10:25:32
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_KDBASE_H_
#define _INCLUDE_KDBASE_H_

#include "base.h"
#include "objreader.h"
#include <vector>

namespace rt {

/**
 * Using CRTP mode to inherit.
 **/ 
template <typename NodeType>
struct KDNodeBase {
    typedef NodeType node_t;

    KDNodeBase(void) : axis(-1), split(inf), lson(NULL), rson(NULL) { }
    inline bool is_leaf(void) const { return lson == NULL && rson == NULL; }

    int axis;
    double split;
    node_t *lson, *rson;
};

template <typename NodeType>
struct KDCompareBase {
    typedef NodeType node_t;

    virtual bool operator ()(const node_t &A, const node_t &B) const = 0;
};

} // end namespace rt

#endif
