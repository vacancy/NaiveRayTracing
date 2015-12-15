/**
 * File   : main.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-15 15:51:20
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/base.h"
#include "../include/canvas.h"
#include "../include/object.h"
#include "../include/material.h"
#include "../include/camera.h"
#include "../include/trace.h"
#include <iostream>
#include <algorithm>

using namespace rt;
using namespace std;

int main(int argc, char *argv[]) {
	int h = 512, w = 512;
	Canvas *canvas = new Canvas(h, w, 3);
	Camera *camera = new PerspectiveCamera(Vector(0, 5, 10), Vector(0, 0, -1), Vector(0, 1, 0), 90); 
	Scene *scene   = new Scene;

	Object *obj1 = new Sphere(Vector(-6, 0, -10), 5);
	Object *obj2 = new Sphere(Vector(6, 0, -10), 5);
	Object *obj3 = new Plane(Vector(0, -5, 0), Vector(0, 1, 0));
	Object *obj4 = new Plane(Vector(-20, 0, 0), Vector(1, 0, 0));
	Object *obj5 = new Plane(Vector(20, 0, 0), Vector(-1, 0, 0));
	Object *obj6 = new Plane(Vector(0, 0, -20), Vector(0, 0, 1));
	Object *obj7 = new Plane(Vector(0, 20, 0), Vector(0, -1, 0));
	Object *lite = new Sphere(Vector(0,600+20-.01,-10), 600);

	obj1->set_material(new Phong(REFL_SPEC, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene);
	obj2->set_material(new Phong(REFL_SPEC, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene);
	obj3->set_material(new Phong(REFL_DIFF, Vector(.75,.75,.75),      Vector::Zero))->add_to_scene(scene);
	obj4->set_material(new Phong(REFL_DIFF, Vector(.75,.25,.25),      Vector::Zero))->add_to_scene(scene);
	obj5->set_material(new Phong(REFL_DIFF, Vector(.25,.25,.75),      Vector::Zero))->add_to_scene(scene);
	obj6->set_material(new Phong(REFL_DIFF, Vector(.25,.25,.25),      Vector::Zero))->add_to_scene(scene);
	obj7->set_material(new Phong(REFL_DIFF, Vector(.25,.25,.25),      Vector::Zero))->add_to_scene(scene);
	lite->set_material(new Phong(REFL_DIFF, Vector::Zero, Vector(100, 100, 100)))->add_to_scene(scene);

	PathTracer *tracer = new PathTracer(10);
	FILE *df = fopen("debug.txt", "w");

	const int SAMPLE_X = 2;
	const int SAMPLE_Y = 2;
	const int SAMPLE = 100;
	const int SAMPLE_XY = SAMPLE_X * SAMPLE_Y;
	const int SAMPLE_ALL = SAMPLE*SAMPLE_XY;
	#pragma omp parallel for schedule(dynamic, 1)
	for (int y = 0; y < h; ++y) {
		fprintf(stderr,"\rRendering (%d spp) %5.2f%%", SAMPLE_ALL, 100.*y/(h-1));
		for (int x = 0; x < w; ++x) {
			Vector color = Vector::Zero;
			for (int i = 0; i < SAMPLE_Y; ++i) {
				for (int j = 0; j < SAMPLE_X; ++j) {
					double sy = 1.0 - (double(y) / h + 1.0 / h * i);
					double sx = double(x) / w + 1.0 / h * j;
					Ray ray = camera->generate(sx, sy);

					Vector sub_color = Vector::Zero;
					for (int k = 0; k < SAMPLE; ++k) {
						Vector tmp = tracer->trace(scene, ray, 0);
						sub_color = sub_color + tmp / double(SAMPLE_ALL);
					}
					color = color + Vector(
						clamp_int(sub_color.x),
						clamp_int(sub_color.y),
						clamp_int(sub_color.z)
					) / SAMPLE_XY;
				}
			}
			canvas->set(y, x, 0, color.x);
			canvas->set(y, x, 1, color.y);
			canvas->set(y, x, 2, color.z);
		}
	}

	canvas->write("result.bmp");

	return 0;
}
