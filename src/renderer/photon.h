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
    Photon(const Vector &pos, const Vector &flux, const Vector &direction)
            : Vector(pos), flux(flux), direction(direction) {

    }

    Vector flux;
    Vector direction;
};

typedef Photon *ptr_photon_t;
typedef VecKDTree<Photon> PhotonKDTree;
typedef std::vector<ptr_photon_t> photon_vec_t;

enum class PhotonState : int {
    Direct    = 0x0100,
    Indirect  = 0x0101,
    Caustic   = 0x0200,
};

class PhotonMap {
public:
    struct PhotonDistanceCompare {
        PhotonDistanceCompare(const Vector &center) : center(center) {

        }

        inline bool operator ()(const ptr_photon_t &lhs, const ptr_photon_t &rhs) {
            return (*lhs - center).l2() < (*rhs - center).l2();
        }

        Vector center;
    };

    PhotonMap(Scene *scene = NULL, Scene *light = NULL) : _scene(scene), _light(light), _global(), _caustic(), _volume() {

    }

    void initialize();
    Vector sample(const Vector &position, const Vector &norm, int global_n, double global_r, int caustic_n, double caustic_r);

private:
    void find_knn(const Photon &center, int global_n, double global_r, int caustic_n, double caustic_r, photon_vec_t &global_result, photon_vec_t &caustic_result);
    void trace(const Ray &ray, const Vector &flux, int depth, PhotonState state, RandomStream *rng);

    PhotonKDTree _global, _caustic, _volume;
    Scene *_scene, *_light;
};

} // End namespace diorama

#endif //RAYTRACE_PHOTON_H
