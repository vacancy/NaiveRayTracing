/**
 * File   : objkd.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-01-01 11:49:07
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/objkd.h"
#include <algorithm>

using std::vector;
using std::swap;

namespace rt {

inline static int get_triangle_side(const Triangle *triangle, int axis, double split) {
    int cnt[2] = {0, 0};
    split -= eps;
    ++cnt[triangle->a[axis] < split];
    ++cnt[triangle->b[axis] < split];
    ++cnt[triangle->c[axis] < split];
    if (cnt[1] == 3)
        return -1;
    if (cnt[0] == 3)
        return 1;
    return 0;
}

void ObjKDTree::_build(ObjKDNode *&root, const vector<Triangle *> a, BoundingBox bbox, int current) {
    if (a.empty()) {
        return;
    }

    root = new ObjKDNode();
    root->bbox = bbox;

    size_t n = a.size();
    if (n <= _max_leaf_size) {
        root->is_leaf = true;
        root->mesh = TriangleMesh(a, bbox);
    } else {
        root->axis = current;
        root->split = (bbox.vmax[current] + bbox.vmin[current]) / 2;

        vector<Triangle *> la, ra;
        for (Triangle *triangle : a) {
            int res = get_triangle_side(triangle, root->axis, root->split);
            if (res <= 0) la.push_back(triangle);
            if (res >= 0) ra.push_back(triangle);
        }

        Vector lvmin = bbox.vmin, lvmax = bbox.vmax;
        Vector rvmin = bbox.vmin, rvmax = bbox.vmax;
        lvmax[current] = root->split, rvmin[current] = root->split;

        int next = (current + 1) % 3;
        _build(root->lson, la, BoundingBox(lvmin, lvmax), next);
        _build(root->rson, ra, BoundingBox(rvmin, rvmax), next);
    }
}

Intersection ObjKDTree::_traverse(ObjKDNode *root, const Ray &ray) {
    if (root == NULL)
        return Intersection::null;

    if (root->is_leaf) {
        return root->mesh.intersect(ray);
    } else {
        ObjKDNode *lson = root->lson;
        ObjKDNode *rson = root->rson;

        int axis = root->axis;
        Pair p = root->bbox.get_near_far(ray);
        double near = p.first, far = p.second;

        if (near > far || far < -1)
            return Intersection::null;

        double split = root->split;
        double near_x = ray.get(near)[axis];
        double far_x = ray.get(far)[axis];
        bool ignored = false, swaped = false;

        if (near_x > split) {
            swap(lson, rson), swaped = true;
            if (far_x > split) ignored = true;
        } else if (far_x < split) {
            ignored = true;
        }

        Intersection interl = _traverse(lson, ray);
        if (interl.object != NULL) {
            if (ignored)
                return interl;
            if (!swaped && interl.position[axis] < split + 1e-9)
                return interl;
            if (swaped && interl.position[axis] > split - 1e-9)
                return interl;
        }

        if (!ignored) {
            Intersection interr = _traverse(rson, ray);
            if (interr.distance < far + 1e-9)
                return interr;
        }
    }

    return Intersection::null;
}

void ObjKDTree::initialize() {
    _build(root, wrapper->_triangles, wrapper->_bbox, 0);
}

Intersection ObjKDTree::intersect(const Ray &ray) {
    Intersection res = _traverse(root, ray);
    return res;
}

}