/**
 * File   : objbase
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 12:45
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_OBJBASE_H
#define RAYTRACE_OBJBASE_H

#include "../core/common.h"
#include "../core/vector3.h"
#include "../core/ray.h"
#include "../material/mtlbase.h"

namespace diorama {

class Intersection;
class BoundingBox;
class Object;
class Scene;

class Intersection {
public:
    Object *object;
    double distance;
    Vector position;
    Vector norm;

    Intersection(void) : object(NULL), distance(0) { }

    const static Intersection null;
};

class Scene {
public:
    std::vector<Object *> objects;

    Scene(void) { }

    inline Scene *add(Object *obj) {
        objects.push_back(obj);
        return this;
    }

    Intersection intersect(const Ray &ray);
};

class BoundingBox {
public:
    Vector vmin, vmax;

    BoundingBox(void) { }

    BoundingBox(Vector vmin, Vector vmax) : vmin(vmin), vmax(vmax) {
        initialize();
    }

    inline void initialize(void) {

    }

    inline Pair get_near_far(const Ray &ray) {
        Vector result_min = vmin - ray.origin;
        Vector result_max = vmax - ray.origin;
        result_min = result_min / (ray.direct + eps);
        result_max = result_max / (ray.direct + eps);
        for (int i = 0; i < 3; ++i)
            if (result_min[i] > result_max[i])
                std::swap(result_min[i], result_max[i]);
        return Pair(result_min.max(), result_max.min());
    }

    inline bool intersect(const Ray &ray) {
        Pair near_far = get_near_far(ray);
        return !(near_far.first > near_far.second || near_far.second < 0);
    }
};

class Object {
public:
    Material *material;

    Object(void) : material(NULL) {

    }

    virtual Intersection intersect(const Ray &ray) { return Intersection::null; }

    inline Object *set_material(Material *m) {
        material = m;
        return this;
    }

    inline Object *add_to_scene(Scene *s) {
        s->add(this);
        return this;
    }

    inline void sample_diff(Ray &result) {

    }
};

} // End namespace diorama

#endif //RAYTRACE_OBJBASE_H
