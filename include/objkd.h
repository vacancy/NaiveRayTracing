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
#include "kdbase.h"
#include <vector>

namespace rt {

class ObjKDTree : public Object {
public:
    struct ObjKDNode : KDNodeBase<ObjKDNode> {
        ObjKDNode(void) : KDNodeBase<ObjKDNode>() { }

        BoundingBox  bbox;
        TriangleMesh mesh;
    };

    ObjKDTree(TriangleMesh *wrapper, int max_leaf_size = 30)
            : wrapper(wrapper), root(NULL), _max_leaf_size(max_leaf_size) {

        initialize();
    }

    void initialize();

    virtual Intersection intersect(const Ray &ray);

    TriangleMesh *wrapper;
    ObjKDNode *root;

protected:
    void _build(ObjKDNode *&root, const std::vector<Triangle *> &a, BoundingBox bbox, int current);

    Intersection _traverse(ObjKDNode *root, const Ray &ray);

private:
    int _max_leaf_size = 30;
};

} // end namespace rt


#endif
