/**
 * File   : photon
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 20:05
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_PHOTON_H
#define RAYTRACE_PHOTON_H

#include "../core/common.h"
#include "../core/vector3.h"
#include "../core/random.h"
#include "../core/ray.h"
#include "../kd/veckd.h"
#include "../object/objbase.h"


namespace diorama {

struct Photon : Vector {
    Vector flux;
    Vector direction;
};

typedef VecKDTree<Photon> PhotonKDTree;
typedef std::vector<Photon *> photon_vec_t;

class PhotonMap {
public:
    void initialize(Scene *scene);
    Vector sample(const Vector &position, const Vector &norm, int gather_num, double gather_r);

private:
    void find_knn(const Photon &photon, int gather_num, double gather_r, photon_vec_t &result);
    void trace(Scene *scene, const Ray &ray, const Vector &flux, int depth, photon_vec_t &result, RandomStream *rand);

    PhotonKDTree _global, _caustic, _volume;
};

} // End namespace diorama

#endif //RAYTRACE_PHOTON_H
