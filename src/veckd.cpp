/**
 * File   : veckd.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-01-04 10:46:14
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/veckd.h"
#include <vector>
#include <algorithm>

using std::vector;
using std::swap;
using std::abs;

namespace rt {

template <typename T>
void VecKDTree<T>::_build(VecKDNode *&root, list_t &a, int current) {
    if (a.empty()) {
        return ;
    }

    root = new VecKDNode();

    size_t n = a.size();
    if (n <= _max_leaf_size) {
        root->vectors = a;
    } else {
        std::nth_element(a.begin(), a.begin() + n/2, a.end(), VecKDCompare(current));
        root->axis = current;
        root->split = a[n/2];

        list_t la(a.begin(), a.begin() + n/2);
        list_t ra(a.begin() + n/2, a.end());
        int next = (current + 1) % 3;
        _build(root->lson, la, next);
        _build(root->rson, ra, next);
    }
};

template <typename T>
void VecKDTree<T>::_traverse_r(list_t &res, const VecKDNode *root, const vector_t center, const double r2) {
    if (root->is_leaf()) {
        for (auto v : root->vectors) {
            if ((v - center).l2() < r2)
                res.push_back(v);
        }
    } else {
        bool in_left = center[root->axis] < root->split;
        bool ignore = (center[root->axis] - root->split) * (center[root->axis] - root->split) > r2;
        if (!in_left) {
            _traverse_r(res, root->lson, center, r2);
            if (!ignore) _traverse_r(res, root->rson, center, r2);
        } else {
            _traverse_r(res, root->rson, center, r2);
            if (!ignore) _traverse_r(res, root->lson, center, r2);
        }
    }
}

} // End namespace rt
