/**
 * File   : depthrenderer
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 13:31
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "depthrenderer.h"
#include "../config.h"

using std::cout;
using std::cerr;
using std::endl;

namespace diorama {

Vector DepthRenderer::trace(const Ray &ray) {
    Intersection inter = _scene->intersect(ray);
    if (DEBUG) {
        cout << "current ray.x " << ray.origin << endl;
        cout << "current ray.d " << ray.direct << endl;
        cout << "intersection distance = " << inter.distance << endl;
    }

    if (!inter.object) {
        return Vector::Zero;
    }
    Object *object = inter.object;
    Vector pos = inter.position;
    Vector norm = inter.norm;

    if (object->is_light) {
        return object->light->get_emission(pos, norm) / inter.distance;
    } else {
        Material *material = object->material;
        return material->get_bsdf(pos, norm)->get_reflectance(pos, norm) / inter.distance;
    }
}

void DepthRenderer::render(Camera *camera, Canvas *canvas) {
    int h = canvas->h, w = canvas->w;

    RandomStream *rng = new LCGStream(19961018 + rand());
    for (int y = 0; y < h; ++y) {
        fprintf(stderr, "\rRendering %5.2f%%", 100. * y / (h - 1));
        for (int x = 0; x < w; ++x) {
            double sy = 1.0 - (double(y) / h);
            double sx = double(x) / w;
            Ray ray = camera->generate(sx, sy, rng);
            Vector color = trace(ray);
            color = color * _norm;
            canvas->set(y, x, 0, clamp_int(color.x));
            canvas->set(y, x, 1, clamp_int(color.y));
            canvas->set(y, x, 2, clamp_int(color.z));
        }
    }
    fprintf(stderr, "\rDepth rendering finished\n");
    delete rng;
}

} // End namespace diorama
