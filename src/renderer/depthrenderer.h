/**
 * File   : depthrenderer
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-07 13:30
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_DEPTHRENDERER_H
#define RAYTRACE_DEPTHRENDERER_H

#include "renbase.h"

namespace diorama {

class DepthRenderer : public Renderer {
public:
    DepthRenderer(Scene *scene, double norm = 1) : _scene(scene), _norm(norm) { }

    Vector trace(const Ray &ray);

    virtual void render(Camera *camera, Canvas *canvas);

protected:
    Scene *_scene;
    double _norm;
};

} // End namespace diorama

#endif //RAYTRACE_DEPTHRENDERER_H
