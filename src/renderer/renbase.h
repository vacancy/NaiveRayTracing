/**
 * File   : renbase
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-07 12:47
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_RENBASE_H
#define RAYTRACE_RENBASE_H

#include "camera.h"
#include "../core/common.h"
#include "../core/vector3.h"
#include "../core/ray.h"
#include "../core/random.h"
#include "../core/canvas.h"
#include "../object/objbase.h"

namespace diorama {

class Renderer {
    virtual void render(Camera *camera, Canvas *canvas) = 0;
};

} // End namespace diorama

#endif //RAYTRACE_RENDER_H
