/**
 * File   : mesh
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-07 13:16
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_MESH_H
#define RAYTRACE_MESH_H

#include "objbase.h"
#include "objdetail.h"

namespace diorama {

class ObjKDTree;
class ObjReader;

class TriangleMesh : public Object {
    friend class ObjKDTree;
    friend class ObjReader;

public:
    typedef std::vector<Triangle *> triangle_vec_t;
    typedef std::vector<Vector> vertex_vec_t;

    TriangleMesh(void) { }

    TriangleMesh(const triangle_vec_t &triangles, const BoundingBox &bbox)
            : _triangles(triangles), _bbox(bbox) {

    }

    TriangleMesh(const triangle_vec_t &triangles, const vertex_vec_t &vertexes)
            : _triangles(triangles) {

        initialize(vertexes);
    }

    inline void initialize(vertex_vec_t vertexes) {
        Vector vmin(inf, inf, inf), vmax(-inf, -inf, -inf);
        for (auto v : vertexes) {
            if (v.x < vmin.x) vmin.x = v.x;
            if (v.y < vmin.y) vmin.y = v.y;
            if (v.z < vmin.z) vmin.z = v.z;

            if (v.x > vmax.x) vmax.x = v.x;
            if (v.y > vmax.y) vmax.y = v.y;
            if (v.z > vmax.z) vmax.z = v.z;
        }
        _bbox = BoundingBox(vmin, vmax);
        std::cout << _bbox.vmin << _bbox.vmax << std::endl;
    }

    virtual Intersection intersect(const Ray &ray);
    virtual void sample(RandomStream *rng, Ray &ray, double &pdf) {

    }

    static TriangleMesh *from_stream(std::istream &f, Material *material, const Vector &resize, const Vector &delta, bool flip_normal = false);

private:
    triangle_vec_t _triangles;
    BoundingBox    _bbox;
};

} // End namespace diorama

#endif //RAYTRACE_MESH_H
