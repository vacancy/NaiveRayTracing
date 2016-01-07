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
#include "../config.h"

namespace diorama {

void PhotonMap::initialize(Scene *scene) {

}

Vector PhotonMap::sample(const Vector &position, const Vector &norm, int gather_num, double gather_r) {

}

void PhotonMap::find_knn(const Photon &photon, int gather_num, double gather_r, photon_vec_t &result) {

}

void PhotonMap::trace(Scene *scene, const Ray &ray, const Vector &flux, int depth, photon_vec_t &result,
                      RandomStream *rand) {

    if (depth > MAX_DEPTH) return ;
    if (flux.max() <= 0) return ;

    Intersection inter = scene->intersect(ray);

    Object *object = inter.object;
    Phong *material = static_cast<Phong *>(object->material);

    double rng = rand->get();
    if (rng < material->k_diff) {

    } else if (rng < material->k_diff + material->k_spec) {

    } else {

    }
}

} // End namespace diorama
