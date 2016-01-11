/**
 * File   : mesh
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 13:17
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "mesh.h"

namespace diorama {

Intersection TriangleMesh::intersect(const Ray &ray) {
    if (!_bbox.intersect(ray)) {
        return Intersection::null;
    }

    Intersection res = Intersection::null;
    for (Triangle *triangle : _triangles) {
        Intersection tmp = triangle->intersect(ray);
        if (tmp.object != NULL) {
            if (res.object == NULL || res.distance > tmp.distance)
                res = tmp;
        }
    }

    return res;
}

TriangleMesh* TriangleMesh::from_stream(std::istream &f, Material *material, const Vector &resize,
                                               const Vector &delta, bool flip_normal) {
    TriangleMesh *mesh = new TriangleMesh();
    TriangleMesh::triangle_vec_t &triangles = mesh->_triangles;
    TriangleMesh::vertex_vec_t vertexes;

    std::vector<int> fx, fy, fz;
    fx.clear(), fy.clear(), fz.clear();

    char buffer[128];
    while (f.getline(buffer, 128)) {
        if (buffer[0] == '#') {
            continue;
        } else if (buffer[0] == 'v') {
            char op;
            double x, y, z;
            sscanf(buffer, "%c %lf %lf %lf", &op, &x, &y, &z);
            Vector vertex = (Vector(x, y, z) * resize) + delta;
            vertexes.push_back(vertex);
        } else if (buffer[0] == 'f') {
            char op;
            int x, y, z;
            sscanf(buffer, "%c %d %d %d", &op, &x, &y, &z);
            if (x == y || y == z || x == z)
                continue;
            fx.push_back(x), fy.push_back(y), fz.push_back(z);
        }
    }

    for (size_t i = 0; i < fx.size(); ++i) {
        int x = fx[i], y = fy[i], z = fz[i];
        if (flip_normal) std::swap(x, y);
        Triangle *triangle = new Triangle(vertexes[x - 1], vertexes[y - 1], vertexes[z - 1]);
        triangle->set_material(material);
        triangles.push_back(triangle);
    }

    mesh->initialize(vertexes);
    return mesh;
}

} // End namespace diorama
