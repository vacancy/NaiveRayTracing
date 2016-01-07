/**
 * File   : ptrenderer
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 13:31
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_PTRENDERER_H
#define RAYTRACE_PTRENDERER_H

#include "renbase.h"

namespace diorama {

class PTRenderer : public IRender {
public:
    PTRenderer(Scene *scene, int max_depth = 10) : _scene(scene), _max_depth(max_depth) {

    }

    Vector trace(const Ray &ray, int depth, LCGStream *rng);

    virtual void render(Camera *camera, Canvas *canvas);

protected:
    Scene *_scene;
    int _max_depth;
};


} // End namespace diorama

#endif //RAYTRACE_PTRENDERER_H
