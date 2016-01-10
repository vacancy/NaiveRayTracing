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
#include <queue>

using std::cerr;
using std::endl;

namespace diorama {

template <typename T>
class VecKDTree {
public:
    typedef typename T::value_t value_t;
    typedef T vector_t;
    typedef T *ptr_vector_t;
    typedef std::vector<ptr_vector_t> list_t;
    typedef typename list_t::iterator list_iter_t;
    typedef std::pair<double, ptr_vector_t> knn_res_t;
    typedef typename std::priority_queue<knn_res_t> knn_queue_t;

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

    virtual ~VecKDTree() {
        for (ptr_vector_t v : wrapper)
            delete v;
    }

    inline void initialize() { _build(root, wrapper.begin(), wrapper.end(), 0); }

    inline list_t find_r(const vector_t &center, const value_t r) {
        list_t res;
        _traverse_r(res, root, center, r*r);
        return res;
    }
    inline list_t find_knn(const vector_t &center, const value_t r, int num) {
        knn_queue_t pq;
        value_t r2 = r * r;
        _traverse_knn_r(pq, root, center, r2, num);
        list_t res;
        while (!pq.empty()) {
            knn_res_t r = pq.top();
            res.push_back(r.second);
            pq.pop();
        }
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
    }

    void _traverse_knn_r(knn_queue_t &res, const VecKDNode *root, const vector_t &center, value_t &r2, int num) {
        if (!root) return ;

        if (root->is_leaf()) {
            for (auto v : root->vectors) {
                value_t dis2 = (*v - center).l2();
                if (dis2 < r2) {
                    res.push(knn_res_t(dis2, v));
                    if (res.size() > num) {
                        res.pop();
                        r2 = (*(res.top().second) - center).l2();
                    }
                }
            }
        } else {
            bool in_left = center[root->axis] < root->split;
            bool ignore = (center[root->axis] - root->split) * (center[root->axis] - root->split) > r2 + bigeps;
            if (in_left) {
                _traverse_knn_r(res, root->lson, center, r2, num);
                if (!ignore) _traverse_knn_r(res, root->rson, center, r2, num);
            } else {
                _traverse_knn_r(res, root->rson, center, r2, num);
                if (!ignore) _traverse_knn_r(res, root->lson, center, r2, num);
            }
        }
    }

private:
    int _max_leaf_size = 30;
};

} // End namespace diorama

#endif //RAYTRACE_VECKD_H
