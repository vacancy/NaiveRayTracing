/**
 * File   : ppmprenderer
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-08 12:01
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "ppmprenderer.h"
#include "../material/bsdf.h"
#include "../config.h"

using std::cout;
using std::cerr;
using std::endl;

namespace diorama {

void PPMPRenderer::render(Camera *camera, Canvas *canvas) {
    _photon_map->initialize();

    int h = canvas->h, w = canvas->w;

#pragma omp parallel for schedule(dynamic, 1)
    for (int y = 0; y < h; ++y) {
        LCGStream *rng = new LCGStream(19961018 + y);
        fprintf(stderr, "\rRendering (%d spp) %5.2f%%", SAMPLE_ALL, 100. * y / (h - 1));
        for (int x = 0; x < w; ++x) {
            Vector color = Vector::Zero;
            for (int i = 0; i < SAMPLE_Y; ++i) {
                for (int j = 0; j < SAMPLE_X; ++j) {
                    double sy = 1.0 - (double(y) / h + 1.0 / h * i);
                    double sx = double(x) / w + 1.0 / h * j;
                    Ray ray = camera->generate(sx, sy);

                    Vector sub_color = Vector::Zero;
                    for (int k = 0; k < SAMPLE; ++k) {
                        Vector tmp = trace(ray, 0, rng);
                        sub_color = sub_color + tmp / double(SAMPLE);
                    }
                    color = color + Vector(
                        clamp(sub_color.x),
                        clamp(sub_color.y),
                        clamp(sub_color.z)
                    ) / SAMPLE_XY;
                }
            }
            canvas->set(y, x, 0, clamp_int(color.x));
            canvas->set(y, x, 1, clamp_int(color.y));
            canvas->set(y, x, 2, clamp_int(color.z));
        }
        delete rng;
    }
}

Vector PPMPRenderer::trace(const Ray &ray, int depth, LCGStream *rng) {
    Intersection inter = _scene->intersect(ray);
    if (DEBUG) {
        cout << "current depth " << depth << endl;
        cout << "current ray.x" << ray.origin << endl;
        cout << "current ray.d" << ray.direct << endl;
        cout << "intersection distance = " << inter.distance << endl;
    }

    if (!inter.object) {
        return Vector::Zero;
    }

    int new_depth = depth + 1;
    bool is_max_depth = new_depth >= _max_depth;
    Object *object = inter.object;
    BSDF *material = static_cast<BSDF *>(object->material);

    bool use_rr = new_depth > 5;
    bool roulette = use_rr && rng->get() < material->max_color;

    if (is_max_depth || (use_rr && !roulette)) {
        return material->emission;
    }
    Vector flux = (use_rr && roulette) ? material->normed_color : material->color;
    Vector norm = inter.norm;
    Vector pos = inter.position;
    Vector radiance = Vector::Zero;

    double pdf = 1.0;
    if (material->get_type() & BSDFType::Scatter) {
        radiance = _photon_map->sample(pos, norm, _gather_num, _gather_r);
    } else {
        Ray new_ray;
        material->sample(ray, pos, norm, rng, new_ray, pdf);
        radiance = trace(new_ray, new_depth, rng);
    }

    return material->emission + flux * radiance / pdf;
}


} // End namespace diorama
