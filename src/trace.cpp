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

		bool isUseRR = new_depth > 5;
		bool isRR = isUseRR && _rng->get() < material->color_max;

		if (is_max_depth || (isUseRR && !isRR)) {
			return material->emission;
		}

		Vector norm = inter.norm;
		Vector pos  = inter.position;
		Vector abs_norm = (dot(norm, ray.direct) < 0) ? norm : norm * -1;

		Vector f = (isUseRR && isRR) ? material->c_color : material->color;

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
		}
		// bool use_reflection = new_depth > 5;
		// bool is_reflection = use_reflection && _rng() < obj->max_color;
// 		  if (obj.refl == DIFF){                  // Ideal DIFFUSE reflection
//     double r1=2*M_PI*erand48(Xi), r2=erand48(Xi), r2s=sqrt(r2);
//     Vec w=nl, u=((fabs(w.x)>.1?Vec(0,1):Vec(1))%w).norm(), v=w%u;
//     Vec d = (u*cos(r1)*r2s + v*sin(r1)*r2s + w*sqrt(1-r2)).norm();
//     return obj.e + f.mult(radiance(Ray(x,d),depth,Xi));
//   } else if (obj.refl == SPEC)            // Ideal SPECULAR reflection
//     return obj.e + f.mult(radiance(Ray(x,r.d-n*2*n.dot(r.d)),depth,Xi));
//   Ray reflRay(x, r.d-n*2*n.dot(r.d));     // Ideal dielectric REFRACTION
//   bool into = n.dot(nl)>0;                // Ray from outside going in?
//   double nc=1, nt=1.5, nnt=into?nc/nt:nt/nc, ddn=r.d.dot(nl), cos2t;
//   if ((cos2t=1-nnt*nnt*(1-ddn*ddn))<0)    // Total internal reflection
//     return obj.e + f.mult(radiance(reflRay,depth,Xi));
//   Vec tdir = (r.d*nnt - n*((into?1:-1)*(ddn*nnt+sqrt(cos2t)))).norm();
//   double a=nt-nc, b=nt+nc, R0=a*a/(b*b), c = 1-(into?-ddn:tdir.dot(n));
//   double Re=R0+(1-R0)*c*c*c*c*c,Tr=1-Re,P=.25+.5*Re,RP=Re/P,TP=Tr/(1-P);
//   return obj.e + f.mult(depth>2 ? (erand48(Xi)<P ?   // Russian roulette
//     radiance(reflRay,depth,Xi)*RP:radiance(Ray(x,tdir),depth,Xi)*TP) :
//     radiance(reflRay,depth,Xi)*Re+radiance(Ray(x,tdir),depth,Xi)*Tr);
// }
	}
}

} // end namespace rt
