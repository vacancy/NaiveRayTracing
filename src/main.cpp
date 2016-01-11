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
#include "material/bsdf.h"
#include "renderer/renbase.h"
#include "renderer/depthrenderer.h"
#include "renderer/ptrenderer.h"
#include "renderer/ppmprenderer.h"
#include <fstream>

using namespace diorama;
using namespace std;

Vector expand(const Vector &source, int axis, double delta) {
    Vector result = source;
    result[axis] += delta;
    return result;
}

void make_scene(Scene *scene, Scene *light, Camera *&camera) {
    Vector l00(-5.0, 9.99, -5.0);
    Vector l01(-5.0, 9.99,  5.0);
    Vector l10( 5.0, 9.99, -5.0);
    Vector l11( 5.0, 9.99,  5.0);
    Object *lt1 = new Quad(l00, l10, l11, l01);
    lt1->set_light(new SimpleLight(Vector(4., 4., 4.)))->add_to_scene(light)->add_to_scene(scene);

    Vector delta(0, 0, 15);
    Object *lt2 = new Quad(l00 + delta, l10 + delta, l11 + delta, l01 + delta);
    lt2->set_light(new SimpleLight(Vector(4., 4., 4.)))->add_to_scene(light)->add_to_scene(scene);

    delta = 2 * delta;
    Object *lt3 = new Quad(l00 + delta, l10 + delta, l11 + delta, l01 + delta);
    lt3->set_light(new SimpleLight(Vector(4., 4., 4.)))->add_to_scene(light)->add_to_scene(scene);

    Vector v000(-10.0, -10.0, -10.0);
    Vector v100( 10.0, -10.0, -10.0);
    Vector v010(-10.0,  10.0, -10.0);
    Vector v001(-10.0, -10.0,  50.0);
    Vector v110( 10.0,  10.0, -10.0);
    Vector v101( 10.0, -10.0,  50.0);
    Vector v011(-10.0,  10.0,  50.0);
    Vector v111( 10.0,  10.0,  50.0);

    Object *ceilWall = new Quad(v010, v110, v111, v011);
    Object *floorWall = new Quad(v000, v001, v101, v100);
    Object *backWall = new Quad(v010, v000, v100, v110);
    Object *leftWall = new Quad(expand(v000, 0, 0.1), expand(v010, 0, 0.1), expand(v011, 0, 0.1), expand(v001, 0, 0.1));
    Object *rightWall = new Quad(expand(v100, 0, -0.1), expand(v101, 0, -0.1), expand(v111, 0, -0.1), expand(v110, 0, -0.1));

    floorWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))
    ))->add_to_scene(scene);
    ceilWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))
    ))->add_to_scene(scene);
    backWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))
    ))->add_to_scene(scene);
    leftWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.25, 0.25)))
    ))->add_to_scene(scene);
    rightWall->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.25, 0.25, 0.75)))
    ))->add_to_scene(scene);

    Object *sphere1 = new Sphere(Vector( 0.0, -7.0,  0.0), 3.0);
    Object *sphere2 = new Sphere(Vector(-5.0, -7.0, -5.0), 3.0);
    Object *sphere3 = new Sphere(Vector( 5.0, -7.0,  5.0), 3.0);
    sphere1->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.25, 0.75, 0.25)))
    ))->add_to_scene(scene);
    sphere2->set_material(new SimpleMaterial(
        new SpecularBRDF(new ConstantTexture<Vector>(Vector(0.999, 0.999, 0.999)))
    ))->add_to_scene(scene);
    sphere3->set_material(new SimpleMaterial(
        new RefractiveBTDF(new ConstantTexture<Vector>(Vector(0.999, 0.999, 0.999)), 1000)
    ))->add_to_scene(scene);

//    ifstream kitten_f("models/kitten.50k.obj", ios::in);
//    TriangleMesh *kitten = TriangleMesh::from_stream(
//        kitten_f, new SimpleMaterial(new RefractiveBTDF(new ConstantTexture<Vector>(Vector(0.999, 0.999, 0.999)))),
//        Vector(0.12, 0.12, 0.12), Vector(-1.75, -9.6, 2.5));
//    kitten_f.close();
//    ObjKDTree *kitten_kd = new ObjKDTree(kitten);
//    kitten_kd->add_to_scene(scene);
//
//    ifstream glass_f("/Users/Iris/Playground/CG/RayTrace/models/teapot.obj", ios::in);
//    TriangleMesh *glass = TriangleMesh::from_stream(
//        glass_f, new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))),
//        Vector(0.12, 0.12, 0.12), Vector(3, -10, 10.5));
//    glass_f.close();
//    ObjKDTree *glass_kd = new ObjKDTree(glass);
//    glass_kd->add_to_scene(scene);

    camera = new PerspectiveCamera(Vector(0.0, 0.0, 100.0), Vector(0, 0, -1).norm(), Vector(0, 1, 0), 15);
//    camera = new DoFCamera(Vector(0.0, 0.0, 100.0), Vector(0, 0, -1).norm(), Vector(0, 1, 0), 20, 1, 80);
}

void make_scene_pt(Scene *scene, Scene *lt, Camera *&camera) {
    Vector l00(-0.75, 2.49, -0.75);
    Vector l01(-0.75, 2.49,  0.75);
    Vector l10( 0.75, 2.49, -0.75);
    Vector l11( 0.75, 2.49,  0.75);
    Object *light = new Quad(l00, l10, l11, l01);
    light->set_light(new SimpleLight(Vector(16, 16, 16)))->add_to_scene(scene)->add_to_scene(lt);

    Object *left = new Plane(Vector(2.5, 0, 0), Vector(-1, 0 ,0));
    Object *right = new Plane(Vector(-2.5, 0, 0), Vector(1, 0 ,0));
    Object *back = new Plane(Vector(0, 0, 1.2), Vector(0, 0, -1));
    Object *bottom = new Plane(Vector(0, -1, 0), Vector(0, 1, 0));
    Object *top = new Plane(Vector(0, 2.5, 0), Vector(0, -1, 0));
    right->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.25, .25, .75)))
    ))->add_to_scene(scene);
    left->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.75, .25, .25)))
    ))->add_to_scene(scene);
    back->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.25, .75, .25)))
    ))->add_to_scene(scene);
    bottom->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.75, .75, .75)))
    ))->add_to_scene(scene);
    top->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.75, .75, .75)))
    ))->add_to_scene(scene);

//    Object *sphere1 = new Sphere(Vector(-0.75, -0.25, -1), 0.75);
    Object *sphere2 = new Sphere(Vector(-0.6, -0.5, -1.5), 0.5);
//    sphere1->set_material(new SimpleMaterial(
//        new SpecularBRDF(new ConstantTexture<Vector>(Vector(0.99, 0.99, 0.99)))
//    ))->add_to_scene(scene);
    sphere2->set_material(new SimpleMaterial(
        new RefractiveBTDF(new ConstantTexture<Vector>(Vector(0.99, 0.99, 0.99)))
    ))->add_to_scene(scene);

    ifstream kitten_f("models/kitten.50k.obj", ios::in);
    TriangleMesh *kitten = TriangleMesh::from_stream(
        kitten_f, new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))),
        Vector(0.026, 0.026, 0.026), Vector(0.75, -1, -0.5));
    kitten_f.close();
    ObjKDTree *kitten_kd = new ObjKDTree(kitten);
    kitten_kd->add_to_scene(scene);

//    ifstream glass_f("models/teapot.obj", ios::in);
//    TriangleMesh *glass = TriangleMesh::from_stream(
//        glass_f, new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))),
//        Vector(0.02, 0.02, 0.02), Vector(-1, -1, -1.5));
//    glass_f.close();
//    ObjKDTree *glass_kd = new ObjKDTree(glass);
//    glass_kd->add_to_scene(scene);

    camera = new PerspectiveCamera(Vector(0, 0.3, -5.75), Vector(0, 0, 1).norm(), Vector(0, 1, 0), 60);
}

void make_scene_dof(Scene *scene, Scene *lt, Camera *&camera) {
    Vector l00(-0.75, 2.49, -0.75);
    Vector l01(-0.75, 2.49,  0.75);
    Vector l10( 0.75, 2.49, -0.75);
    Vector l11( 0.75, 2.49,  0.75);
    Object *light = new Quad(l00, l10, l11, l01);
    light->set_light(new SimpleLight(Vector(16, 16, 16)))->add_to_scene(scene)->add_to_scene(lt);

    Object *left = new Plane(Vector(2.5, 0, 0), Vector(-1, 0 ,0));
    Object *right = new Plane(Vector(-2.5, 0, 0), Vector(1, 0 ,0));
    Object *back = new Plane(Vector(0, 0, 1.2), Vector(0, 0, -1));
    Object *bottom = new Plane(Vector(0, -1, 0), Vector(0, 1, 0));
    Object *top = new Plane(Vector(0, 2.5, 0), Vector(0, -1, 0));
    right->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.25, .25, .75)))
    ))->add_to_scene(scene);
    left->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.75, .25, .25)))
    ))->add_to_scene(scene);
    back->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.25, .75, .25)))
    ))->add_to_scene(scene);
    bottom->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.75, .75, .75)))
    ))->add_to_scene(scene);
    top->set_material(new SimpleMaterial(
        new LambertianBRDF(new ConstantTexture<Vector>(Vector(.75, .75, .75)))
    ))->add_to_scene(scene);

    ifstream dragonf1("models/fixed.perfect.dragon.100K.0.07.obj", ios::in);
    TriangleMesh *dragon1 = TriangleMesh::from_stream(
        dragonf1, new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))),
        Vector(1.4*0.8, 1*0.8, 1*0.8), Vector(0.75, -0.3, -2.5));
    dragonf1.close();
    ObjKDTree *dragon1kd = new ObjKDTree(dragon1);
    dragon1kd->add_to_scene(scene);

    ifstream dragonf2("models/fixed.perfect.dragon.100K.0.07.obj", ios::in);
    TriangleMesh *dragon2 = TriangleMesh::from_stream(
        dragonf2, new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))),
        Vector(1.4*0.8, 1*0.8, 1*0.8), Vector(-0.75, -0.3, -5.5));
    dragonf2.close();
    ObjKDTree *dragon2kd = new ObjKDTree(dragon2);
    dragon2kd->add_to_scene(scene);

    camera = new PerspectiveCamera(Vector(0, 1, -13.75), Vector(0, -.1, 1).norm(), Vector(0, 1, 0), 30);
    camera = new DoFCamera(Vector(0, 1, -13.75), Vector(0, -.1, 1).norm(), Vector(0, 1, 0), 30, 0.33, 11.25);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    Canvas *depth_canvas = new Canvas(h, w, 3);
    Canvas *pt_canvas = new Canvas(h, w, 3);
    Canvas *pm_canvas = new Canvas(h, w, 3);

    Scene *scene = new Scene;
    Scene *light = new Scene;
    Camera *camera;
//    make_scene(scene, light, camera);
//    make_scene_pt(scene, light, camera);
    make_scene_dof(scene, light, camera);

    DepthRenderer *depth_render = new DepthRenderer(scene, 1);
    depth_render->render(camera, depth_canvas);
    depth_canvas->write("result_depth.bmp");
    depth_canvas->show("Depth", true);

    PTRenderer *pt_render = new PTRenderer(scene, MAX_DEPTH);
    pt_render->render(camera, pt_canvas);
    pt_canvas->write("result_pt.bmp");
//    pt_canvas->show("PathTracing", true);

//    PPMPRenderer *pm_render = new PPMPRenderer(scene, light, MAX_DEPTH);
//    pm_render->render(camera, pm_canvas);

    return 0;
}
