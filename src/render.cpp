/**
 * File   : render.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 11:50:46
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "../include/config.h"
#include "../include/render.h"
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

extern bool DEBUG;

namespace rt {

Vector PathTraceRender::trace(const Ray &ray, int depth, LCGStream *rng) {
    Intersection inter = _scene->intersect(ray);
    if (DEBUG) {
        cout << "current depth " << depth << endl;
        cout << "current ray.x" << ray.origin << endl;
        cout << "current ray.d" << ray.direct << endl;
        cout << "intersection distance = " << inter.distance << endl;
    }

    if (!inter.object) {
        return Vector::Zero;
    } else {
        int new_depth = depth + 1;
        bool is_max_depth = new_depth >= _max_depth;
        Object *object = inter.object;
        Phong *material = static_cast<Phong *>(object->material);

        bool use_rr = new_depth > 5;
        bool roulette = use_rr && rng->get() < material->color_max;

        if (is_max_depth || (use_rr && !roulette)) {
            return material->emission;
        }
        Vector f = (use_rr && roulette) ? material->c_color : material->color;

        Vector norm = inter.norm;
        Vector pos = inter.position;
        Vector abs_norm = (dot(norm, ray.direct) < 0) ? norm : norm * -1;

        if (material->reflect_type == REFL_DIFF) {
            double r1 = 2 * PI * rng->get();
            double r2 = rng->get();
            double r2s = sqrt(r2);

            Vector w = abs_norm;
            Vector wo = w.x < -0.1 || w.x > 0.1 ? Vector::YAxis : Vector::XAxis;
            Vector u = cross(wo, w).norm();
            Vector v = cross(w, u);
            Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
            return material->emission + f * trace(Ray(pos, d), new_depth, rng);
        } else if (material->reflect_type == REFL_SPEC) {
            Vector d = ray.direct - norm * 2 * dot(norm, ray.direct);
            return material->emission + f * trace(Ray(pos, d), new_depth, rng);
        } else { // if (material->reflect_type == REFL_REFR) {
            Vector ans2;

            Vector d_refl = ray.direct - norm * 2 * dot(norm, ray.direct);
            // cout << "refl = " << d_refl << endl;

            bool into = dot(norm, abs_norm) > 0;
            double alpha = 1, beta = 1.5;
            double nnt = into ? alpha / beta : beta / alpha;
            double ddn = dot(ray.direct, abs_norm);
            double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

            if (cos2t < 0) {
                Vector res1 = trace(Ray(pos, d_refl), new_depth, rng);
                return material->emission + f * res1;
            } else {
                Vector d_refr = (ray.direct * nnt - norm * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
                // cout << "refl = " << d_refr << endl;

                double a = beta - alpha, b = beta + alpha;
                double R0 = sqr(a) / sqr(b);
                double c = 1 - (into ? -ddn : dot(norm, d_refr));
                double Re = R0 + (1 - R0) * c * c * c * c * c;
                double Tr = 1 - Re;
                double P = 0.25 + 0.5 * Re;

                Vector result;
                if (new_depth > 2) {
                    if (rng->get() < P) result = trace(Ray(pos, d_refl), new_depth, rng) * Re / P;
                    else result = trace(Ray(pos, d_refr), new_depth, rng) * Tr / (1 - P);
                } else {
                    Vector res1 = trace(Ray(pos, d_refl), new_depth, rng) * Re;
                    Vector res2 = trace(Ray(pos, d_refr), new_depth, rng) * Tr;
                    result = res1 + res2;
                }

                Vector ans = (material->emission + f * result);
                // if (new_depth == 1)
                // 	printf("result = %f %f %f\n", ans.x, ans.y, ans.z);

                return ans;
            }
        }
    }
}

void PathTraceRender::render(Camera *camera, Canvas *canvas) {
    int h = canvas->h, w = canvas->w;

#pragma omp parallel for schedule(dynamic, 1)
    for (int y = 0; y < h; ++y) {
        LCGStream *rng = new LCGStream(666666 + y);
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
    }
}

Vector DepthRender::trace(const Ray &ray) {
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
    Phong *material = static_cast<Phong *>(object->material);
    return material->color / inter.distance;
}

void DepthRender::render(Camera *camera, Canvas *canvas) {
    int h = canvas->h, w = canvas->w;

    for (int y = 0; y < h; ++y) {
        fprintf(stderr, "\rRendering %5.2f%%", 100. * y / (h - 1));
        for (int x = 0; x < w; ++x) {
            double sy = 1.0 - (double(y) / h);
            double sx = double(x) / w;
            Ray ray = camera->generate(sx, sy);
            Vector color = trace(ray);
            color = color * _norm;
            canvas->set(y, x, 0, clamp_int(color.x));
            canvas->set(y, x, 1, clamp_int(color.y));
            canvas->set(y, x, 2, clamp_int(color.z));
        }
    }
}

} // end namespace rt
