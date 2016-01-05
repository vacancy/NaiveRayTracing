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
template <typename NodeType, typename ValueType>
struct KDNodeBase {
    typedef NodeType node_t;
    typedef ValueType value_t;

    KDNodeBase(void) : axis(-1), split(0), lson(NULL), rson(NULL) { }
    inline bool is_leaf(void) const { return lson == NULL && rson == NULL; }

    int     axis;
    value_t split;
    node_t  *lson, *rson;
};

template <typename PtrType>
struct KDCompareBase {
    typedef PtrType ptr_t;

    virtual bool operator ()(const ptr_t &lhs, const ptr_t &rhs) const = 0;
};

} // end namespace rt

#endif
