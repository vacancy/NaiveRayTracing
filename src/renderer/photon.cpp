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

    for (int i = 0; i < PHOTON_SAMPLE; ++i) {
        LCGStream *rng = new LCGStream(19961018 + i);
        for (Object *object : _light->objects) {

            Ray light;
            double pdf;
            object->sample(rng, light, pdf);

            Vector flux = object->material->emission * pi / pdf / PHOTON_SAMPLE;
            light.direct = rng->sample_hemisphere(light.direct);

            trace(light, flux, 0, PhotonState::Direct, rng);

            if (_global.wrapper.size() >= PHOTON_GLOBAL && !finished_global) {
                for (Photon *photon : _global.wrapper)
                    photon->flux = photon->flux / double(i);
                finished_global = true;
            }

            if (_caustic.wrapper.size() >= PHOTON_CAUSTIC && !finished_caustic) {
                for (Photon *photon : _caustic.wrapper)
                    photon->flux = photon->flux / double(i);
                finished_caustic = true;
            }

            if (_global.wrapper.size() >= PHOTON_GLOBAL && _caustic.wrapper.size() >= PHOTON_CAUSTIC)
                break;
        }
        delete rng;
    }
    std::cerr << "global: " << _global.wrapper.size() << std::endl;
    std::cerr << "caustic: " << _caustic.wrapper.size() << std::endl;
    _global.initialize();
    _caustic.initialize();
}

Vector PhotonMap::sample(const Vector &position, const Vector &norm, int gather_num, double gather_r) {
    Photon query(position, Vector::Zero, Vector::Zero);
    photon_vec_t result, valid;
    std::vector<double> distances;
    find_knn(query, gather_num, gather_r, result);

    double max_dis2 = 0.;
    for (Photon *photon : result) {
        double dis2 = (position - *photon).l2();
        if (dis2 > max_dis2) max_dis2 = dis2;
    }

    if (max_dis2 > eps) {
        Vector flux = Vector::Zero;
        for (Photon *photon : result) {
            double dis2 = (position - *photon).l2();
            double s = 1 - dis2 / max_dis2;
            double k = 3.f * inv_pi * s * s;
            flux += k / max_dis2 * photon->flux;
        }

        return flux;
    }

//    double max_dist = 0.0;
//    for (Photon *photon : result) {
//        Vector diff = position - *photon;
//        double dist = diff.l2();
//        double dt   = dot(norm, diff) / dist;
//        if (abs(dt) < gather_r * gather_r * 0.01) {
//            valid.push_back(photon);
//            distances.push_back(dist);
//            max_dist = max(max_dist, dist);
//        }
//    }
//
//    if (max_dist > eps) {
//        double k = 1.1;
//        Vector flux = Vector::Zero;
//
//        for (size_t i = 0; i < valid.size(); ++i) {
//            double w = 1.0 - (distances[i] / (k * max_dist));
//            Vector v = valid[i]->flux * inv_pi;
//            flux += w * v;
//        }
//        flux = flux / (1.0 - 2.0 / (3.0 * k));
//
//        return flux / (pi * sqr(max_dist));
//    }

    return Vector::Zero;
}

void PhotonMap::find_knn(const Photon &center, int gather_num, double gather_r, photon_vec_t &result) {
    photon_vec_t result1 = _global.find_r(center, gather_r);
    photon_vec_t result2 = _caustic.find_r(center, gather_r);

    result1.insert(result1.end(), result2.begin(), result2.end());
    if (result1.size() > gather_num)
        std::sort(result1.begin(), result1.end(), PhotonDistanceCompare(center));

    for (int i = 0; i < min(gather_num, static_cast<int>(result1.size())); ++i)
        result.push_back(result1[i]);
}

void PhotonMap::trace(const Ray &ray, const Vector &flux, int depth, PhotonState state, RandomStream *rng) {
    if (depth > MAX_DEPTH) return ;
    if (flux.max() <= 0) return ;

    Intersection inter = _scene->intersect(ray);
    int new_depth = depth + 1;

    Object *object = inter.object;
    BSDF *material = static_cast<BSDF *>(object->material);

    Vector current_flux = material->color;
    Vector norm = inter.norm;
    Vector pos = inter.position;
    Vector trace_res = Vector::Zero;

    double photon_pdf = 1.0;
    if (material->get_type() & BSDFType::Scatter) {
        if (state == PhotonState::Caustic) {
            if (_caustic.wrapper.size() < PHOTON_CAUSTIC)
                _caustic.wrapper.push_back(new Photon(pos, flux, ray.direct));
        } else {
            if (_global.wrapper.size() < PHOTON_GLOBAL)
                _global.wrapper.push_back(new Photon(pos, flux, ray.direct));
        }

        const double prob = current_flux.mean();
        if (rng->get() < prob) {
            photon_pdf *= prob;
        } else {
            return;
        }
    }

    PhotonState new_state = state;
    if (material->get_type() & BSDFType::Scatter) {
        if (state == PhotonState::Caustic) new_state = PhotonState::Indirect;
    } else if (material->get_type() & BSDFType::Refractive) {
        if (state != PhotonState::Indirect) new_state = PhotonState::Caustic;
    }

    Ray new_ray; double sample_pdf;
    material->sample(ray, pos, norm, rng, new_ray, sample_pdf);
    Vector new_flux = flux * current_flux / (sample_pdf * photon_pdf);

    trace(new_ray, new_flux, new_depth, new_state, rng);
}

} // End namespace diorama
