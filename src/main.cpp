/**
 * File   : main.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-15 15:51:20
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/base.h"
#include "../include/config.h"
#include "../include/canvas.h"
#include "../include/object.h"
#include "../include/objkd.h"
#include "../include/objreader.h"
#include "../include/camera.h"
#include "../include/render.h"

bool DEBUG = false;

using namespace rt;
using namespace std;

int main(int argc, char *argv[]) {
    Scene *scene1 = new Scene;
    if (SCENE_ID == 1) {
        Object *sphere1 = new Sphere(Vector(-2.25, 0, 0), 4);
        Object *sphere2 = new Sphere(Vector(2.25, 0, 0), 2);
        Object *left = new Sphere(Vector(-1e5 - 10, 0, 0), 1e5);
        Object *right = new Sphere(Vector(1e5 + 10, 0, 0), 1e5);
        Object *back = new Sphere(Vector(0, 0, -1e5 - 10), 1e5);
        Object *bottom = new Sphere(Vector(0, -1e5 - 10, 0), 1e5);
        Object *top = new Sphere(Vector(0, 1e5 + 10, 0), 1e5);
        sphere1->set_material(new Phong(REFL_SPEC, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene1);
        sphere2->set_material(new Phong(REFL_REFR, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene1);
        left->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .25), Vector::Zero))->add_to_scene(scene1);
        right->set_material(new Phong(REFL_DIFF, Vector(.25, .75, .75), Vector::Zero))->add_to_scene(scene1);
        back->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector::Zero))->add_to_scene(scene1);
        bottom->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector::Zero))->add_to_scene(scene1);
        top->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector(0.5, 0.5, 0.5)))->add_to_scene(scene1);
    }

    Scene *scene2 = new Scene;
    if (SCENE_ID == 2) {
        Object *obj1 = new Sphere(Vector(-6, 0, -13), 5);
        Object *obj2 = new Sphere(Vector(6, 0, -13), 5);
        Object *obj3 = new Plane(Vector(0, -5, 0), Vector(0, 1, 0));  // Bottom
        Object *obj4 = new Plane(Vector(-17, 0, 0), Vector(1, 0, 0)); // Left
        Object *obj5 = new Plane(Vector(17, 0, 0), Vector(-1, 0, 0)); // Right
        Object *obj6 = new Plane(Vector(0, 0, -20), Vector(0, 0, 1)); // Back
        Object *obj7 = new Plane(Vector(0, 20, 0), Vector(0, -1, 0)); // Top
        Object *lite = new Sphere(Vector(0, 600 + 20 - .03, -7), 600);

        obj1->set_material(new Phong(REFL_SPEC, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene2);
        obj2->set_material(new Phong(REFL_REFR, Vector(.999, .999, .999), Vector::Zero))->add_to_scene(scene2);
        obj3->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector::Zero))->add_to_scene(scene2);
        obj4->set_material(new Phong(REFL_DIFF, Vector(.75, .25, .25), Vector::Zero))->add_to_scene(scene2);
        obj5->set_material(new Phong(REFL_DIFF, Vector(.25, .25, .75), Vector::Zero))->add_to_scene(scene2);
        obj6->set_material(new Phong(REFL_DIFF, Vector(.25, .25, .25), Vector::Zero))->add_to_scene(scene2);
        obj7->set_material(new Phong(REFL_DIFF, Vector(.25, .25, .25), Vector::Zero))->add_to_scene(scene2);
        lite->set_material(new Phong(REFL_DIFF, Vector::Zero, Vector(12, 12, 12)))->add_to_scene(scene2);
    }

    Scene *scene3 = new Scene;
    if (SCENE_ID == 3) {
        ObjReader *reader = new ObjReader("dinosaur.2k.obj");
        TriangleMesh *dinosaur = reader->process(new Phong(REFL_REFR, Vector(0.66, 0.66, 0.66), Vector::Zero));
        dinosaur->add_to_scene(scene3);

        Object *left = new Sphere(Vector(0, -1e5 - 100, 0), 1e5);
        Object *right = new Sphere(Vector(0, 1e5 + 100, 0), 1e5);
        Object *back = new Sphere(Vector(1e5 + 50, 0, 0), 1e5);
        Object *bottom = new Sphere(Vector(0, 0, -1e5 - 50), 1e5);
        Object *top = new Sphere(Vector(0, 0, 1e5 + 100), 1e5);
        left->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .25), Vector::Zero))->add_to_scene(scene3);
        right->set_material(new Phong(REFL_DIFF, Vector(.25, .75, .75), Vector::Zero))->add_to_scene(scene3);
        back->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector::Zero))->add_to_scene(scene3);
        bottom->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector::Zero))->add_to_scene(scene3);
        top->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector(0.5, 0.5, 0.5)))->add_to_scene(scene3);
    }

    Scene *scene4 = new Scene;
    if (SCENE_ID == 4) {
        ObjReader *reader = new ObjReader("fixed.perfect.dragon.100K.0.07.obj");
        TriangleMesh *dragon = reader->process(new Phong(REFL_REFR, Vector(0.9, 0.9, 0.9), Vector::Zero));
        ObjKDTree *dragon_kd = new ObjKDTree(dragon);
        dragon_kd->add_to_scene(scene4);
        //dragon->add_to_scene(scene4);

        Object *left = new Sphere(Vector(-1e5 - 3, 0, 0), 1e5);
        Object *right = new Sphere(Vector(1e5 + 3, 0, 0), 1e5);
        Object *back = new Sphere(Vector(0, 0, 1e5 + 1.2), 1e5);
        Object *bottom = new Sphere(Vector(0, -1e5 - 1, 0), 1e5);
        Object *top = new Sphere(Vector(0, 1e5 + 2.5, 0), 1e5);
        left->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .25), Vector::Zero))->add_to_scene(scene4);
        right->set_material(new Phong(REFL_DIFF, Vector(.25, .75, .75), Vector::Zero))->add_to_scene(scene4);
        back->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector::Zero))->add_to_scene(scene4);
        bottom->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector(0.5, 0.5, 0.5)))->add_to_scene(scene4);
        top->set_material(new Phong(REFL_DIFF, Vector(.75, .75, .75), Vector(0.5, 0.5, 0.5)))->add_to_scene(scene4);
    }

    Canvas *canvas = new Canvas(h, w, 3);
    Camera *camera1 = new PerspectiveCamera(Vector(-100, -5, 30), Vector(1, 0, -0.2).norm(), Vector(0, 0, 1), 90);
    Camera *camera3 = new PerspectiveCamera(Vector(-100, -5, 30), Vector(1, 0, -0.2).norm(), Vector(0, 0, 1), 90);
    Camera *camera4 = new PerspectiveCamera(Vector(0, 0, -1.25), Vector(0, 0, 1).norm(), Vector(0, 1, 0), 120);

    if (SCENE_ID == 3) {
        PathTraceRender *render = new PathTraceRender(scene3, MAX_DEPTH);
        render->render(camera3, canvas);
        //DepthRender *render = new DepthRender(scene3, 100);
        //render->render(camera3, canvas);
    } else if (SCENE_ID == 4) {
        PathTraceRender *render = new PathTraceRender(scene4, MAX_DEPTH);
        render->render(camera4, canvas);
        //DepthRender *render = new DepthRender(scene4, 1);
        //render->render(camera4, canvas);
    }

    canvas->write("result.bmp");
    canvas->show();

    return 0;
}
