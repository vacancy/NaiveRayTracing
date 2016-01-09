/**
 * File   : veckd
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:41
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_VECKD_H
#define RAYTRACE_VECKD_H

#include "kdbase.h"

using std::cerr;
using std::endl;

namespace diorama {

extern bool debug_kdtree;

template <typename T>
class VecKDTree {
public:
    typedef typename T::value_t value_t;
    typedef T vector_t;
    typedef T *ptr_vector_t;
    typedef std::vector<ptr_vector_t> list_t;
    typedef typename list_t::iterator list_iter_t;

    struct VecKDNode : KDNodeBase<VecKDNode, value_t> {
        VecKDNode(void) : KDNodeBase<VecKDNode, value_t>() { }
        list_t vectors;
    };

    struct VecKDCompare : KDCompareBase<ptr_vector_t> {
        VecKDCompare(int axis = 0) : KDCompareBase<ptr_vector_t>(), axis(axis) { }
        virtual inline bool operator ()(const typename KDCompareBase<ptr_vector_t>::ptr_t &lhs,
                                        const typename KDCompareBase<ptr_vector_t>::ptr_t &rhs) const {
            return lhs->_values[axis] < rhs->_values[axis];
        }

        int axis;
    };

    VecKDTree(int max_leaf_size = 30) : root(NULL), _max_leaf_size(max_leaf_size) {

    }

    VecKDTree(const list_t &wrapper, int max_leaf_size = 30)
        : wrapper(wrapper), root(NULL), _max_leaf_size(max_leaf_size) {

        initialize();
    }

    inline void initialize() { _build(root, wrapper.begin(), wrapper.end(), 0); }

    inline list_t find_r(const vector_t &center, const value_t r) {
        if (debug_kdtree) cerr << endl << "new_find_r" << endl;
        list_t res;
        _traverse_r(res, root, center, r*r);
        return res;
    }

    inline list_t find_r_bf(const vector_t &center, const value_t r) {
        list_t res;
        value_t r2 = r*r;
        for (ptr_vector_t v : wrapper) {
            if ((*v - center).l2() < r2)
                res.push_back(v);
        }
        return res;
    }

    list_t wrapper;
    VecKDNode *root;

protected:
    void _build(VecKDNode *&root, list_iter_t begin, list_iter_t end, int current) {
        if (begin == end) {
            return ;
        }

        root = new VecKDNode();

        size_t n = end - begin;
        if (n <= _max_leaf_size) {
            root->vectors = list_t(begin, end);
        } else {
            std::sort(begin, end, VecKDCompare(current));
            root->axis = current;
            root->split = (*(begin + n/2))->_values[current];

            int next = (current + 1) % 3;
            _build(root->lson, begin, begin + n/2, next);
            _build(root->rson, begin + n/2, end, next);
        }
    }

    void _traverse_r(list_t &res, const VecKDNode *root, const vector_t &center, const value_t r2) {
        if (!root) return ;
        if (debug_kdtree) cerr << center << " " << root->axis << " " << root->split << " " << root->is_leaf() << endl;

        if (root->is_leaf()) {
            for (auto v : root->vectors) {
                if ((*v - center).l2() < r2)
                    res.push_back(v);
            }
        } else {
            bool in_left = center[root->axis] < root->split;
            bool ignore = (center[root->axis] - root->split) * (center[root->axis] - root->split) > r2 + bigeps;
            if (in_left) {
                _traverse_r(res, root->lson, center, r2);
                if (!ignore) _traverse_r(res, root->rson, center, r2);
            } else {
                _traverse_r(res, root->rson, center, r2);
                if (!ignore) _traverse_r(res, root->lson, center, r2);
            }
        }

//        int first = pos[cur->dim] > cur->split, second = !first;
//        VecKDNode *lson = first == 0 ? root->lson : root->rson;
//        VecKDNode *rson = second == 0 ? root->lson : root->rson;
//        real dist = (cur->t->getPos() - pos).L2();
//        if (dist < r2)
//            res.push_back(cur->t);
//        if ((cur->ch >> first) & 1){
//            if (a[root * 2 + first].b->minDist2(pos) < radius2)
//                _traverse_r(res, lson, center, r2);
//        }
//        if ((cur->ch >> second) & 1){
//            if (a[root * 2 + second].b->minDist2(pos) < radius2)
//                _traverse_r(res, rson, center, r2);
//        }
    }

private:
    int _max_leaf_size = 30;
};

} // End namespace diorama

#endif //RAYTRACE_VECKD_H
