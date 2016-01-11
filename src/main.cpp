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
        new LambertianSpecularBSDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)), 0.7)
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

//    camera = new PerspectiveCamera(Vector(0, 1, -13.75), Vector(0, -.1, 1).norm(), Vector(0, 1, 0), 30);
    camera = new DoFCamera(Vector(0, 1, -13.75), Vector(0, -.1, 1).norm(), Vector(0, 1, 0), 30, 0.034, 11.25);
}

void big_helper(Scene *scene, string filename, Material *mat, bool flip_norm=true) {
    ifstream f(filename, ios::in);
    TriangleMesh *mesh = TriangleMesh::from_stream(f, mat, Vector(1, 1, 1), Vector::Zero, flip_norm);
    f.close();
    ObjKDTree *kd = new ObjKDTree(mesh);
    kd->add_to_scene(scene);
}
void make_scene_big(Scene *scene, Scene *light, Camera *&camera) {
    Plane *floor = new Plane(Vector(0, 0, 0), Vector(0, 1, 0));
    floor->set_material(new SimpleMaterial(new LambertianBRDF(new BmpTexture("models/Chene_huile_naturel.jpg", Vector(-200, -50, 0), Vector(1, 0, 0), Vector(0, 0, -1), 20, 20, 2.2))));
    floor->set_bump(new BumpTexture("models/Chene_huile_naturel_bump.jpg", Vector(-200, -50, 0), Vector(1, 0, 0), Vector(0, 0, -1), 20, 20, 2.2, 5));
    floor->add_to_scene(scene);
    Plane *wall = new Plane(Vector(0, 0, -76), Vector(0, 0, 1));
    wall->set_material(new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.2078, 0.3922, 0.3647)))));
    wall->set_bump(new BumpTexture("models/AS2_cloth_10_bump.jpg", Vector(0, 0, -76), Vector(1, 0, 0), Vector(0, -1, 0), 20, 20, 2.2, 100));
    wall->add_to_scene(scene);

    big_helper(scene, "models/8.1.obj", new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.75, 0.75, 0.75)))));
//    big_helper(scene, "models/1.obj", new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(0.5647, 0.3333, 0.1764)))));
    big_helper(scene, "models/1.obj", new SimpleMaterial(new LambertianBRDF(new BmpTexture("models/ch_2234_t_antique_oxford_0.jpg", Vector(0, 0, 0), Vector(1, 0, 0), Vector(0, -1, -1).norm(), 20, 20, 2.2))));
    big_helper(scene, "models/2.obj", new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(214./255, 204./255, 193./255)))));
    big_helper(scene, "models/3.obj", new SimpleMaterial(new RefractiveBTDF(new ConstantTexture<Vector>(Vector(0.999, 0.999, 0.999)))), false);
//    big_helper(scene, "models/3.obj", new SimpleMaterial(new LambertianSpecularBSDF(new ConstantTexture<Vector>(Vector(0.88, 0.88, 0.88)), 0.)));
    big_helper(scene, "models/4.obj", new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(52./255, 86./255, 22./255)))));
    big_helper(scene, "models/5.obj", new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(102./255, 123./255, 142./255)))));
    big_helper(scene, "models/6.obj", new SimpleMaterial(new LambertianBRDF(new ConstantTexture<Vector>(Vector(102./255, 123./255, 142./255)))));

    {
        Vector l00(16.0721, 99.4089, -73.0000);
        Vector l01(-14.0867, 99.4089, -73.0000);
        Vector l10(-14.0867, 56.6286, -73.0000);
        Vector l11(16.0721, 56.6286, -73.0000);
        Object *quad = new Quad(l11, l10, l01, l00);
        quad->set_material(
            new SimpleMaterial(new LambertianBRDF(
                new BmpTexture("models/alonesky.jpg", Vector(-14.0867, 56.6286, -73.0000), Vector(1, 0, 0), Vector(0, -1, 0), 20.4915, 20.523, 2.2)
            ))
        );
        quad->add_to_scene(scene);
    }
    {
        Vector l00(118.0322, 90.6049, -52.5106);
        Vector l01(118.0322, 17.6197, -52.5106);
        Vector l10(118.0322, 90.6049, 7.8514);
        Vector l11(118.0322, 17.6197, 7.8514);
        Object *quad = new Quad(l00, l01, l10, l11);
        quad->set_light(new SimpleLight(Vector(50, 50, 50)));
        quad->add_to_scene(scene)->add_to_scene(light);
    }
    {
        Vector l00(394.3732, 244.5573, -25.3815);
        Vector l01(443.0328, 161.4010, -12.3249);
        Vector l10(373.0712, 248.3344, 78.0624);
        Vector l11(421.7308, 165.1782, 91.1191);
        Object *quad = new Quad(l00, l01, l10, l11);
        quad->set_light(new SimpleLight(Vector(250, 250, 250)));
        quad->add_to_scene(scene)->add_to_scene(light);
    }

    camera = new PerspectiveCamera2(Vector(0, 100, 500), Vector(0, -0.1, -1).norm(), Vector(0, 1, 0), 20, 15);
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
//    make_scene_dof(scene, light, camera);
    make_scene_big(scene, light, camera);

    DepthRenderer *depth_render = new DepthRenderer(scene, 150);
    depth_render->render(camera, depth_canvas);
    depth_canvas->write("result_depth.bmp");
    depth_canvas->show("Depth", false);

    PTRenderer *pt_render = new PTRenderer(scene, MAX_DEPTH);
    pt_render->render(camera, pt_canvas);
    pt_canvas->write("result_pt.bmp");
    pt_canvas->show("PathTracing");

    PPMPRenderer *pm_render = new PPMPRenderer(scene, light, MAX_DEPTH);
    pm_render->render(camera, pm_canvas);

    return 0;
}
