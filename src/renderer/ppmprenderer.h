/**
 * File   : ppmprenderer
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-08 11:58
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_PPMPRENDERER_H
#define RAYTRACE_PPMPRENDERER_H

#include "photon.h"
#include "renbase.h"
#include "../core/random.h"

namespace diorama {

class PPMPRenderer : IRender {
public:
    PPMPRenderer(Scene *scene, Scene *light, int gather_num, double gather_r, int max_depth)
        : _scene(scene), _light(light),
          _gather_num(gather_num), _gather_r(gather_r), _max_depth(max_depth) {

        _photon_map = new PhotonMap(scene, light);
    }

    virtual ~PPMPRenderer() {
        delete _photon_map;
    }

    Vector trace(const Ray &ray, int depth, LCGStream *rng);
    virtual void render(Camera *camera, Canvas *canvas);

protected:
    Scene  *_scene, *_light;
    int    _gather_num;
    double _gather_r;
    int    _max_depth;

    PhotonMap *_photon_map;
};

} // End namespace diorama

#endif //RAYTRACE_PPMPRENDERER_H
