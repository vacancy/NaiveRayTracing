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

template <typename T>
class VecKDTree {
public:
    typedef T vector_t;
    typedef std::vector<vector_t> list_t;

    struct VecKDNode : KDNodeBase<VecKDNode> {
        VecKDNode(void) : KDNodeBase<VecKDNode>() { }
        list_t vectors;
    };

    struct VecKDCompare : KDCompareBase<vector_t> {
        VecKDCompare(int axis = 0) : axis(axis) { std::cout << axis << std::endl; }
        virtual inline bool operator ()(const vector_t &lhs, const vector_t &rhs) const {
            return lhs._values[axis] < rhs._values[axis];
        }

        int axis;
    };

    VecKDTree(TriangleMesh *wrapper, int max_leaf_size = 30)
            : wrapper(wrapper), root(NULL), _max_leaf_size(max_leaf_size) {

        initialize();
    }

    inline void initialize() { _build(root, wrapper, 0); }

    inline list_t find_r(const vector_t center, const double r) {
        list_t res;
        _traverse_r(res, root, center, r*r);
        return res;
    }

    list_t wrapper;
    VecKDNode *root;

protected:
    void _build(VecKDNode *&root, list_t &a, int current);
    void _traverse_r(list_t &res, const VecKDNode *root, const vector_t center, const double r2);

private:
    int _max_leaf_size = 30;
};

} // end namespace rt


#endif
