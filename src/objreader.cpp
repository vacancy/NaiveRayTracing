/**
 * File   : objreader.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-12-23 23:48:46
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/objreader.h"
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;
using std::ifstream;
using std::ios;

namespace rt {

TriangleMesh *ObjReader::process(Material *material, const Vector &resize, const Vector &delta) {
    ifstream f;
    f.open(_filename.c_str(), ios::in);

    TriangleMesh *mesh = new TriangleMesh();
    TriangleMesh::TriangleVector &triangels = mesh->_triangles;
    TriangleMesh::VertexVector vertexes;

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
            Triangle *triangle = new Triangle(vertexes[x - 1], vertexes[y - 1], vertexes[z - 1]);
            triangle->set_material(material);
            triangels.push_back(triangle);
        }
    }

    mesh->initialize(vertexes);
    f.close();

    return mesh;
}

} // end namespace rt