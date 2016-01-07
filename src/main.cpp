/**
 * File   : main.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-15 15:51:20
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "core/common.h"
#include "config.h"
#include "core/canvas.h"
#include "object/objdetail.h"
#include "object/mesh.h"
#include "kd/objkd.h"
#include "renderer/renbase.h"
#include "renderer/depthrenderer.h"
#include "renderer/ptrenderer.h"
#include <fstream>

using namespace diorama;
using namespace std;

int main(int argc, char *argv[]) {
    Scene *scene4 = new Scene;

    Object *left = new Plane(Vector(3, 0, 0), Vector(-1, 0 ,0));
    Object *right = new Plane(Vector(-3, 0, 0), Vector(1, 0 ,0));
    Object *back = new Plane(Vector(0, 0, 1.2), Vector(0, 0, -1));
    Object *bottom = new Plane(Vector(0, -1, 0), Vector(0, 1, 0));
    Object *top = new Plane(Vector(0, 2.5, 0), Vector(0, -1, 0));
//    Object *light = new Sphere(Vector(0, 50 + 2.5 - 0.01, -1), 50);
    Object *light = new Sphere(Vector(0, 2, -1), 0.4);
    right->set_material(new Phong(Vector(.25, .25, .75), Vector::Zero, 1))->add_to_scene(scene4);
    left->set_material(new Phong(Vector(.75, .25, .25), Vector::Zero, 1))->add_to_scene(scene4);
    back->set_material(new Phong(Vector(.25, .65, .25), Vector::Zero, 1))->add_to_scene(scene4);
    bottom->set_material(new Phong(Vector(.75, .75, .75), Vector::Zero, 1))->add_to_scene(scene4);
    top->set_material(new Phong(Vector(.75, .75, .75), Vector::Zero, 1))->add_to_scene(scene4);
    light->set_material(new Phong(Vector(.75, .75, .75), Vector(7.5, 7.5, 7.5), 1))->add_to_scene(scene4);

//    ifstream dragon_f("models/fixed.perfect.dragon.100K.0.07.obj", ios::in);
//    TriangleMesh *dragon = TriangleMesh::from_stream(
//            dragon_f, new Phong(Vector(0.999, 0.999, 0.999), Vector::Zero, 0.3, 0.7, 5, true),
//            Vector(1.5, 1, 1), Vector(0, 0, -2));
//    dragon_f.close();
//    ObjKDTree *dragon_kd = new ObjKDTree(dragon);
//    dragon_kd->add_to_scene(scene4);
    Object *sphere1 = new Sphere(Vector(-1, -0.25, -1), 0.75);
    Object *sphere2 = new Sphere(Vector(1, -0.25, -1.5), 0.75);
    sphere1->set_material(new Phong(Vector(0.99, 0.99, 0.99), Vector::Zero, 0, 1, 5, true))->add_to_scene(scene4);
    sphere2->set_material(new Phong(Vector(0.99, 0.99, 0.99), Vector::Zero, 0, 1, 1))->add_to_scene(scene4);

    Canvas *depth_canvas = new Canvas(h, w, 3);
    Canvas *pt_canvas = new Canvas(h, w, 3);
    Camera *camera4 = new PerspectiveCamera(Vector(0, 0.3, -5.75), Vector(0, 0, 1).norm(), Vector(0, 1, 0), 60);

    DepthRenderer *depth_render = new DepthRenderer(scene4, 1);
    depth_render->render(camera4, depth_canvas);
    depth_canvas->write("result_depth.bmp");
    //depth_canvas->show("Depth");

    PTRenderer *pt_render = new PTRenderer(scene4, MAX_DEPTH);
    pt_render->render(camera4, pt_canvas);

    pt_canvas->write("result_pt.bmp");
    //pt_canvas->show("PathTracing");

    return 0;
}
