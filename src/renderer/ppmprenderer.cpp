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
//    for (Object *object : _light->objects)
//        _scene->objects.push_back(object);

    int h = canvas->h, w = canvas->w;
    double alpha = 0.7;
    Vector *colors = new Vector[h * w];
    memset(colors, 0, sizeof(Vector)*h*w);

    for (int k = 0; k < SAMPLE; ++k) {
        _photon_map = new PhotonMap(_scene, _light);
        _photon_map->initialize();

#pragma omp parallel for schedule(dynamic, 1)
        for (int y = 0; y < h; ++y) {
            LCGStream *rng = new LCGStream(19961018 + y + rand());
            fprintf(stderr, "\rRendering (iteration %d) %5.2f%%", k, 100. * y / (h - 1));
            for (int x = 0; x < w; ++x) {
                Vector color = Vector::Zero;
                for (int i = 0; i < SAMPLE_Y; ++i) {
                    for (int j = 0; j < SAMPLE_X; ++j) {
                        double sy = 1.0 - (double(y) / h + 1.0 / h * i);
                        double sx = double(x) / w + 1.0 / h * j;
                        Ray ray = camera->generate(sx, sy, rng);

                        Vector sub_color = Vector::Zero;
                        int global_n = PHOTON_GLOBAL_N;
                        int caustic_n = PHOTON_CAUSTIC_N;
                        double global_r = PHOTON_GLOBAL_R;
                        double caustic_r = PHOTON_CAUSTIC_R;
                        global_r = ((i + 1) + alpha) / ((i + 1) + 1.0) * global_r;

                        Vector tmp = trace(ray, 0, rng, global_n, global_r, caustic_n, caustic_r);
                        color += tmp / double(SAMPLE_XY);
                    }
                }
                colors[y * w + x] += color / double(SAMPLE);
            }
            delete rng;
        }
        fprintf(stderr, "\rRendering (iteration %d) finished\n", k);

        double scale = double(SAMPLE) / double(k+1);
        std::cerr << "rescale factor " << scale << std::endl;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                Vector color = colors[y * w + x];
                canvas->set(y, x, 0, clamp_int(color.x * scale));
                canvas->set(y, x, 1, clamp_int(color.y * scale));
                canvas->set(y, x, 2, clamp_int(color.z * scale));
            }
        }
        char buffer[128];
        sprintf(buffer, "result_pm_%d.bmp", k);
        canvas->write(buffer);
        sprintf(buffer, "Photon Mapping Iteration %d", k);
        canvas->show(buffer, false);
    }
}

Vector PPMPRenderer::trace(const Ray &ray, int depth, LCGStream *rng,
                           int global_n, double global_r, int caustic_n, double caustic_r) {

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
    Vector flux = material->color;
    Vector norm = inter.norm;
    Vector pos = inter.position;
    Vector radiance = Vector::Zero;

    if (material->get_type() & BSDFType::Scatter) {
        radiance = _photon_map->sample(pos, norm, global_n, global_r, caustic_n, caustic_r);
    }

    if (material->get_type() & BSDFType::Dielectric){
        if (material->get_type() == BSDFType::Refractive && new_depth < 5) {
            BTDF *btdf = static_cast<BTDF *>(material);
            Ray new_ray1, new_ray2; double pdf1, pdf2;
            btdf->sample(ray, pos, norm, rng, new_ray1, pdf1, 1);
            btdf->sample(ray, pos, norm, rng, new_ray2, pdf2, 2);
            radiance += trace(new_ray1, new_depth, rng, global_n, global_r, caustic_n, caustic_r) * pdf1;
            if (pdf2 > bigeps) radiance += trace(new_ray2, new_depth, rng, global_n, global_r, caustic_n, caustic_r) * pdf2;
        } else {
            Ray new_ray; double pdf = 1.0;
            material->sample(ray, pos, norm, rng, new_ray, pdf);
            radiance = trace(new_ray, new_depth, rng, global_n, global_r, caustic_n, caustic_r) / pdf;
        }
    }

    return material->emission + flux * radiance;
}


} // End namespace diorama
