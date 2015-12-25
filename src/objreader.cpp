/**
 * File   : objreader.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-12-23 23:48:46
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/base.h"
#include "../include/objreader.h"
#include <fstream>
#include <vector>

using namespace std;

namespace rt {

void ObjReader::process(Scene *scene, Material *material) {
	vector<Vector> vertexes;
	ifstream f;
	f.open(_filename.c_str(), ios::in);

	char buffer[128];
	while (f.getline(buffer, 128)) {
		if (buffer[0] == '#') {
			continue;
		} else if (buffer[0] == 'v') {
			char op; double x, y, z;
			sscanf(buffer, "%c %lf %lf %lf", &op, &x, &y, &z);
			vertexes.push_back(Vector(x, y, z));
			//cout << "added 1 vertex " << vertexes.size() << " " << vertexes.back() << endl;
		} else if (buffer[0] == 'f') {
			char op; int x, y, z;
			sscanf(buffer, "%c %d %d %d", &op, &x, &y, &z);
			Triangle *triangle = new Triangle(vertexes[x-1], vertexes[y-1], vertexes[z-1]);
			triangle->set_material(material)->add_to_scene(scene);
			//cout << "added 1 triangle " << x << " " << y  << " " << z << " " << endl;
		}
	}

	Vector vmin(1e5, 1e5, 1e5), vmax(-1e5, -1e5, -1e5);
	for (auto v : vertexes) {
		if (v.x < vmin.x) vmin.x = v.x;
		if (v.y < vmin.y) vmin.y = v.y;
		if (v.z < vmin.z) vmin.z = v.z;

		if (v.x > vmax.x) vmax.x = v.x;
		if (v.y > vmax.y) vmax.y = v.y;
		if (v.z > vmax.z) vmax.z = v.z;
	}
	cout << vmin << endl;
	cout << vmax << endl;

	f.close();
}

} // end namespace rt