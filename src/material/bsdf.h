/**
 * File   : bsdf
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : $YEAR-$MONTH-07 23:19
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_BSDF_H
#define RAYTRACE_BSDF_H

#include "mtlbase.h"

namespace diorama {

enum class BSDFType : int {
    Bssrdf      = 0x0100,
    Scatter     = 0x0200,
    Dielectric  = 0x0400,

    Mixed       = 0x0610,

    None        = 0x0000,
    Lambertian  = 0x0001 | Scatter,
    Specular    = 0x0002 | Dielectric,
    Phong       = 0x0004 | Scatter,
    Refractive  = 0x0008 | Dielectric,
};

inline BSDFType operator |(BSDFType t1, BSDFType t2) {
    return static_cast<BSDFType>((static_cast<int>(t1) | static_cast<int>(t2)));
}

inline bool operator &(BSDFType t1, BSDFType t2) {
    return (static_cast<int>(t1) & static_cast<int>(t2)) != 0;
}

class BSDF : public Material {
public:
    BSDF(const Vector &color, const Vector &emission) : Material(color, emission) {

    }

    virtual BSDFType get_type() const = 0;
};

class BRDF : public BSDF {
public:
    BRDF(const Vector &color, const Vector &emission) : BSDF(color, emission) {

    }
};

class LambertianBRDF : public BRDF {
public:
    LambertianBRDF(const Vector &color, const Vector &emission = Vector::Zero) : BRDF(color, emission) {

    }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) {

        Vector abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        out.origin = pos;
        out.direct = rng->sample_hemisphere(abs_norm);
        pdf = 1;
    }

    virtual BSDFType get_type() const { return BSDFType::Lambertian; }
};

class SpecularBRDF : public BRDF {
public:
    SpecularBRDF(const Vector &color, const Vector &emission = Vector::Zero) : BRDF(color, emission) {

    }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) {

        out.origin = pos;
        out.direct = reflect(in.direct, norm);
        pdf = 1;
    }

    virtual BSDFType get_type() const { return BSDFType::Specular; }
};

class PhongBRDF : public BRDF {
public:
    PhongBRDF(const Vector &color, const Vector &emission, int coeff) : BRDF(color, emission), coeff(coeff){

    }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) {


        Vector abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        Vector refl = reflect(in.direct, abs_norm);

        Vector w = refl;
        Vector wo = w.x < -0.1 || w.x > 0.1 ? Vector::YAxis : Vector::XAxis;
        Vector u = cross(wo, w).norm();
        Vector v = cross(w, u);

        double theta = acos(pow(rng->get(), 1.0 / (coeff + 1.0)));
        double phi   = 2.0 * pi * rng->get();

        pdf = 1.0;
        out.origin = pos;
        out.direct = (u * sin(theta) * cos(phi) + w * cos(theta) + v * sin(theta) * sin(phi)).norm();
    }

    virtual BSDFType get_type() const { return BSDFType::Phong; }

    int coeff;
};

class BTDF : public BSDF {
public:
    BTDF(const Vector &color, const Vector &emmision = Vector::Zero, double beta = 1.5) : BSDF(color, emission), beta(beta) {

    }

    virtual inline void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) {

        sample(in, pos, norm, rng, out, pdf, 0);
    }

    // next_id: 0 - random, 1 - reflection, 2 - refraction
    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf, int next_id) {

        Vector abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        bool   into = dot(norm, abs_norm) > 0;
        double alpha = 1;
        double nnt = into ? alpha / beta : beta / alpha;
        double ddn = dot(in.direct, abs_norm);
        double cos2t = 1 - nnt * nnt * (1 - ddn * ddn);

        Vector d_refl = in.direct - norm * 2 * dot(norm, in.direct);

        out.origin = pos;
        if (cos2t < 0) {
            if (next_id == 0 || next_id == 1) {
                out.direct = d_refl;
                pdf = 1;
            } else {
                pdf = 0;
            }

        } else {
            Vector d_refr = (in.direct * nnt - norm * ((into ? 1 : -1) * (ddn * nnt + sqrt(cos2t)))).norm();

            double a = beta - alpha, b = beta + alpha;
            double R0 = sqr(a) / sqr(b);
            double c = 1 - (into ? -ddn : dot(norm, d_refr));
            double Re = R0 + (1 - R0) * (c * c * c * c * c);
            double Tr = 1 - Re;
            double P = 0.25 + 0.5 * Re;

            if (next_id == 0) {
                if (rng->get() < P) {
                    out.direct = d_refl;
                    pdf = P / Re;
                } else {
                    out.direct = d_refr;
                    pdf = (1 - P) / Tr;
                }
            } else if (next_id == 1) {
                out.direct = d_refl;
                pdf = Re;
            } else if (next_id == 2) {
                out.direct = d_refr;
                pdf = Tr;
            }
        }
    }

    virtual BSDFType get_type() const { return BSDFType::Refractive; }

    double beta;
};

class MixedBSDF : public BSDF {
public:
    LambertianBRDF *diff;
    SpecularBRDF *spec;
    BTDF *refr;
    double k1, k2, k3;

    MixedBSDF(LambertianBRDF *diff, SpecularBRDF *spec, BTDF *refr, double k1, double k2, double k3) : BSDF(diff->color, diff->emission), diff(diff), spec(spec), refr(refr), k1(k1), k2(k2), k3(k3) {

    }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,  Ray &out, double &pdf) {
        double r = rng->get();
        if (r < k1 - eps) {
            diff->sample(in, pos, norm, rng, out, pdf);
            pdf *= k1;
        } else if (r < k1 + k2 - eps) {
            spec->sample(in, pos, norm, rng, out, pdf);
            pdf *= k2;
        } else if (r < k1 + k2 + k3 - eps) {
            refr->sample(in, pos, norm, rng, out, pdf);
            pdf *= k3;
        } else {
            pdf = eps;
        }
    }

    virtual BSDFType get_type() const { return BSDFType::Mixed; }
};


} // End namespace diorama

#endif //RAYTRACE_MTLDETAIL_H
