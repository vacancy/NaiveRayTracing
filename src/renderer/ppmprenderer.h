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

class PPMPRenderer : Renderer {
public:
    PPMPRenderer(Scene *scene, Scene *light, int max_depth)
        : _scene(scene), _light(light), _max_depth(max_depth) {

    }

    virtual ~PPMPRenderer() {
        delete _photon_map;
    }

    Vector trace(const Ray &ray, int depth, LCGStream *rng, int global_n, double global_r, int caustic_n, double caustic_r);
    virtual void render(Camera *camera, Canvas *canvas);

protected:
    Scene  *_scene, *_light;
    int    _max_depth;

    PhotonMap *_photon_map;
};

} // End namespace diorama

#endif //RAYTRACE_PPMPRENDERER_H
