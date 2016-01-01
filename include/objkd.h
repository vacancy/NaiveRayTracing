/**
 * File   : objkd.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-12-31 16:59:47
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_OBJECT_KD_H_
#define _INCLUDE_OBJECT_KD_H_

#include "base.h"
#include "objreader.h"
#include <vector>


namespace rt {

struct ObjKDNode {
    ObjKDNode(void) : axis(-1), split(inf), lson(NULL), rson(NULL), is_leaf(false) {

    }

    int axis;
    double split;
    ObjKDNode *lson, *rson;

    BoundingBox bbox;
    TriangleMesh mesh;

    bool is_leaf;
};

class ObjKDTree : public Object {
public:
    ObjKDTree(TriangleMesh *wrapper, int max_leaf_size = 30)
            : wrapper(wrapper), root(NULL), _max_leaf_size(max_leaf_size) {

        initialize();
    }

    void initialize();
    // void finalize();

    virtual Intersection intersect(const Ray &ray);

    TriangleMesh *wrapper;
    ObjKDNode *root;

protected:
    void _build(ObjKDNode *&root, const std::vector<Triangle *> a, BoundingBox bbox, int current);

    Intersection _traverse(ObjKDNode *root, const Ray &ray);

private:
    int _max_leaf_size = 30;
};

} // end namespace rt


#endif
