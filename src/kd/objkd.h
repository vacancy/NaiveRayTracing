/**
 * File   : objkd
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:41
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_OBJKD_H
#define RAYTRACE_OBJKD_H

#include "kdbase.h"
#include "../object/mesh.h"

namespace diorama {

class ObjKDTree : public Object {
public:
    struct ObjKDNode : KDNodeBase<ObjKDNode, double> {
        ObjKDNode(void) : KDNodeBase<ObjKDNode, double>() { }

        BoundingBox  bbox;
        TriangleMesh mesh;
    };

    ObjKDTree(TriangleMesh *wrapper, int max_leaf_size = 30)
            : wrapper(wrapper), root(NULL), _max_leaf_size(max_leaf_size) {

        initialize();
    }

    void initialize();

    virtual Intersection intersect(const Ray &ray);
    virtual void sample(RandomStream *rng, Ray &ray, double &pdf) {

    }

    TriangleMesh *wrapper;
    ObjKDNode *root;

protected:
    void _build(ObjKDNode *&root, const std::vector<Triangle *> &a, BoundingBox bbox, int current);

    Intersection _traverse(ObjKDNode *root, const Ray &ray);

private:
    int _max_leaf_size = 30;
};

} // End namespace diorama

#endif //RAYTRACE_OBJKD_H
