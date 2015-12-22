/**
 * File   : trace.cpp
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2015-11-16 11:50:46
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "base.h"
#include "object.h"
#include "camera.h"
#include "trace.h"

namespace rt {

Vector PathTracer::trace(Scene *scene, const Ray &ray, int depth) {
	Intersection inter = scene->intersect(ray);
	if (!inter.object) {
		return Vector::Zero;
	} else {
		int  new_depth = depth + 1;
		bool is_max_depth = new_depth >= _max_depth;
		Object *object = inter.object;
		Phong *material = static_cast<Phong *>(object->material);

		bool use_rr = new_depth > 5;
		bool roulette = use_rr && _rng->get() < material->color_max;

		if (is_max_depth || (use_rr && !roulette)) {
			return material->emission;
		}
		Vector f = (use_rr && roulette) ? material->c_color : material->color;

		Vector norm = inter.norm;
		Vector pos  = inter.position;
		Vector abs_norm = (dot(norm, ray.direct) < 0) ? norm : norm * -1;

		if (material->reflect_type == REFL_DIFF) {
			double r1 = 2 * PI * _rng->get();
			double r2 = _rng->get();
			double r2s = sqrt(r2);

			Vector w = abs_norm;
			Vector wo = w.x < -0.1 || w.x > 0.1 ? Vector::YAxis : Vector::XAxis;
			Vector u = cross(wo, w).norm();
			Vector v = cross(w, u);
			Vector d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).norm();
			return material->emission + f * trace(scene, Ray(pos, d), new_depth);
		} else if (material->reflect_type == REFL_SPEC) {
			Vector d = ray.direct - norm * 2 * dot(norm, ray.direct);
			return material->emission + f * trace(scene, Ray(pos, d), new_depth);
		} else if (material->reflect_type == REFL_REFR) {
			Ray reflRay(pos, ray.direct - norm * (2 * dot(norm, ray.direct)));
			bool into = dot(norm, abs_norm) > 0;  // Ray from outside going in?
			double nc = 1;
			double nt = 1.5;
			double nnt = into ? nc / nt : nt / nc;
			double ddn = dot(ray.direct, abs_norm);
			double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

			if (cos2t < 0)  // Total internal reflection
				return material->emission + f * trace(scene, reflRay, new_depth);
			else
			{
				Vector tdir = (ray.direct * nnt - norm * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
				double a = nt - nc;
				double b = nt + nc;
				double R0 = (a * a) / (b * b);
				double c = 1 - (into ? -ddn : dot(tdir, norm));
				double Re = R0 + (1 - R0) * c * c * c * c * c;
				double Tr = 1 - Re;
				double P = .25 + .5 * Re;
				double RP = Re / P;
				double TP = Tr / (1 - P);

				Vector result;
				if (new_depth > 2) {
					// Russian roulette and splitting for selecting reflection and/or refraction
					if (_rng->get() < P)
						result = trace(scene, reflRay, new_depth) * RP;
					else
						result = trace(scene, Ray(pos, tdir), new_depth) * TP;
				}
				else
					result = trace(scene, reflRay, new_depth) * Re + trace(scene, Ray(pos, tdir), new_depth) * Tr;

				return material->emission + f * result;
			}

			// Vector d_refl = ray.direct - norm * 2 * dot(norm, ray.direct);
			// bool into = dot(norm, abs_norm) > 0;
			// double alpha = 1, beta = 1.5;
			// double nnt = into ? alpha / beta : beta / alpha;
			// double ddn = dot(d_refl, abs_norm);
			// double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

			// if (cos2t < 0) {
			// 	return material->emission + f * trace(scene, Ray(pos, d_refl), new_depth);
			// } else {
			// 	Vector d_refr = (d_refl * nnt - norm * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();
			// 	double a = beta - alpha, b = beta + alpha;
			// 	double R0 = sqr(a) / sqr(b);
			// 	double c = 1 - (into ? -ddn : dot(norm, d_refr));
			// 	double Re = R0 + (1 - R0) * c * c * c * c * c;
			// 	double Tr = 1 - Re;

			// 	double P = .25 + .5 * Re;
			// 	double RP = Re / P;
			// 	double TP = Tr / (1 - P);

			// 	Vector result;
			// 	if (depth > 1) {
			// 		if (rand() < P) result = trace(scene, Ray(pos, d_refl), new_depth) * RP;
			// 		else result = trace(scene, Ray(pos, d_refr), new_depth) * TP;
			// 	} else
			// 		result = trace(scene, Ray(pos, d_refl), new_depth) * Re + trace(scene, Ray(pos, d_refr), new_depth) * Tr;

			// 	return material->emission + f * result;
			// }
		}
	}
}

} // end namespace rt
