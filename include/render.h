/**
 * File   : render.h
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 11:51:02
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef _INCLUDE_RENDER_H_
#define _INCLUDE_RENDER_H_

#include "base.h"
#include "canvas.h"
#include "camera.h"
#include "random.h"
#include "object.h"

namespace rt {

class Render {
    virtual void render(Camera *camera, Canvas *canvas) = 0;
};

class PathTraceRender : public Render {
public:
    PathTraceRender(Scene *scene, int max_depth = 10) {
        _scene = scene;
        _max_depth = max_depth;
    }

    Vector trace(const Ray &ray, int depth, LCGStream *rng);

    virtual void render(Camera *camera, Canvas *canvas);

protected:
    Scene *_scene;
    int _max_depth;
};

class DepthRender : public Render {
public:
    DepthRender(Scene *scene, double norm = 1) : _scene(scene), _norm(norm) { }

    Vector trace(const Ray &ray);

    virtual void render(Camera *camera, Canvas *canvas);

protected:
    Scene *_scene;
    double _norm;
};

} // end namespace rt

#endif
