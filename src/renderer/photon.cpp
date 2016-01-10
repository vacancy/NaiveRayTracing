/**
 * File   : photon
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 20:28
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "photon.h"
#include "../object/objbase.h"
#include "../material/bsdf.h"
#include "../config.h"

#include <cstdlib>
#include <ctime>

using std::cerr;
using std::cout;
using std::endl;

namespace diorama {

bool debug_kdtree = false;

void PhotonMap::initialize() {
    bool finished_global = false;
    bool finished_caustic = false;

    LCGStream *rng = new LCGStream(19961018 + rand());
    for (int i = 0; i < PHOTON_SAMPLE; ++i) {
        for (Object *object : _light->objects) {

            Ray light;
            double pdf;
            object->sample(rng, light, pdf);

            Vector flux = object->material->emission * pi / pdf / PHOTON_SAMPLE;
            light.direct = rng->sample_hemisphere(light.direct);

            trace(light, flux, 0, PhotonState::Direct, rng);

//            if (_global.wrapper.size() >= PHOTON_GLOBAL && !finished_global) {
//                for (Photon *photon : _global.wrapper)
//                    photon->flux = photon->flux / double(i);
//                finished_global = true;
//            }

//            if (_caustic.wrapper.size() >= PHOTON_CAUSTIC && !finished_caustic) {
//                for (Photon *photon : _caustic.wrapper)
//                    photon->flux = photon->flux / double(i);
//                finished_caustic = true;
//            }

//            if (_global.wrapper.size() >= PHOTON_GLOBAL && _caustic.wrapper.size() >= PHOTON_CAUSTIC)
//                break;
        }
//        if (_global.wrapper.size() >= PHOTON_GLOBAL && _caustic.wrapper.size() >= PHOTON_CAUSTIC)
//            break;
    }
    delete rng;

    std::cerr << "global: " << _global.wrapper.size() << std::endl;
    std::cerr << "caustic: " << _caustic.wrapper.size() << std::endl;
    _global.initialize();
    _caustic.initialize();
}

Vector PhotonMap::sample(const Vector &position, const Vector &norm, int global_n, double global_r, int caustic_n, double caustic_r) {
    Photon query(position, Vector::Zero, Vector::Zero);
    photon_vec_t global_result, caustic_result;
    std::vector<double> distances;
    find_knn(query, global_n, global_r, caustic_n, caustic_r, global_result, caustic_result);

    Vector flux = Vector::Zero;

//    double max_dis2 = 0.;
//    for (Photon *photon : global_result) {
//        double dis2 = (position - *photon).l2();
//        if (dis2 > max_dis2) max_dis2 = dis2;
//    }
//    if (max_dis2 > eps) {
//        for (Photon *photon : global_result) {
//            double dis2 = (position - *photon).l2();
//            double s = 1 - dis2 / max_dis2;
//            double k = 3.f * inv_pi * s * s;
//            flux += k / max_dis2 * photon->flux;
//        }
//    }

//    max_dis2 = 0.;
//    for (Photon *photon : caustic_result) {
//        double dis2 = (position - *photon).l2();
//        if (dis2 > max_dis2) max_dis2 = dis2;
//    }
//    if (max_dis2 > eps) {
//        for (Photon *photon : caustic_result) {
//            double dis2 = (position - *photon).l2();
//            double s = 1 - dis2 / max_dis2;
//            double k = 3.f * inv_pi * s * s;
//            flux += k / max_dis2 * photon->flux;
//        }
//    }

    photon_vec_t valid;

    double max_dist = 0.0;
    for (Photon *photon : global_result) {
        Vector diff = position - *photon;
        double dist = diff.len();
        double dt   = dot(norm, diff) / dist;
        if (abs(dt) < global_r * global_r * 0.01) {
            valid.push_back(photon);
            max_dist = max(max_dist, dist);
        }
    }

    if (max_dist > eps) {
        double k = 1.1;

        for (size_t i = 0; i < valid.size(); ++i) {
            double dist = (position - *(valid[i])).len();
            double w = 1.0 - (dist / (k * max_dist));
            Vector v = valid[i]->flux * inv_pi;
            flux += w * v;
        }
        flux = flux / (1.0 - 2.0 / (3.0 * k));

        return flux / (pi * sqr(max_dist));
    }

    return Vector::Zero;
}

void PhotonMap::find_knn(const Photon &center, int global_n, double global_r, int caustic_n, double caustic_r,
                         photon_vec_t &global_result, photon_vec_t &caustic_result) {

    photon_vec_t result1 = _global.find_knn(center, global_r, global_n);
//    photon_vec_t result2 = _caustic.find_r(center, caustic_r);

//    if (result1.size() > global_n)
//        std::sort(result1.begin(), result1.end(), PhotonDistanceCompare(center));
//    if (result2.size() > caustic_n)
//        std::sort(result2.begin(), result2.end(), PhotonDistanceCompare(center));

    for (int i = 0; i < min(global_n, static_cast<int>(result1.size())); ++i) {
        result1[i]->flux = result1[i]->flux * PHOTON_GLOBAL_MUL;
        global_result.push_back(result1[i]);
    }
//    for (int i = 0; i < min(caustic_n, static_cast<int>(result2.size())); ++i) {
//        result2[i]->flux = result2[i]->flux * PHOTON_CAUSTIC_MUL;
//        caustic_result.push_back(result2[i]);
//    }
}

void PhotonMap::trace(const Ray &ray, const Vector &flux, int depth, PhotonState state, RandomStream *rng) {
    if (depth > MAX_DEPTH) return ;
    if (flux.max() <= 0) return ;

    Intersection inter = _scene->intersect(ray);
    if (inter.object == NULL) return ;

    int new_depth = depth + 1;

    Object *object = inter.object;
    BSDF *material = static_cast<BSDF *>(object->material);

    Vector current_flux = material->color;
    Vector norm = inter.norm;
    Vector pos = inter.position;
    Vector trace_res = Vector::Zero;

    double photon_pdf = 1.0;
    if (material->get_type() & BSDFType::Scatter) {
//        if (state == PhotonState::Caustic) {
//            if (_caustic.wrapper.size() < PHOTON_CAUSTIC)
//                _caustic.wrapper.push_back(new Photon(pos, flux, ray.direct));
//        } else {
            if (_global.wrapper.size() < PHOTON_GLOBAL)
                _global.wrapper.push_back(new Photon(pos, flux, ray.direct));
//        }

        const double prob = current_flux.mean();
        if (rng->get() < prob) {
            photon_pdf *= prob;
        } else {
            return;
        }
    }

    PhotonState new_state = state;
//    if (material->get_type() & BSDFType::Scatter) {
//        if (state == PhotonState::Caustic) new_state = PhotonState::Indirect;
//    } else if (material->get_type() & BSDFType::Refractive) {
//        if (state != PhotonState::Indirect) new_state = PhotonState::Caustic;
//    }

    Ray new_ray; double sample_pdf;
    material->sample(ray, pos, norm, rng, new_ray, sample_pdf);
    Vector new_flux = flux * current_flux / (sample_pdf * photon_pdf);

    trace(new_ray, new_flux, new_depth, new_state, rng);
}

} // End namespace diorama
