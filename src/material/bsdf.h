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

#include "texture.h"
#include "../core/random.h"

namespace diorama {

enum class BSDFType : int {
    Scatter     = 0x0200,
    Dielectric  = 0x0400,

    None        = 0x0000,
    Lambertian  = 0x0001 | Scatter,
    Specular    = 0x0002 | Dielectric,
    Phong       = 0x0004 | Scatter,
    Refractive  = 0x0008 | Dielectric,
};

enum class BSDFQueryType : int {
    BRDF = 0x0001,
    BTDF = 0x0002,
    All  = BRDF | BTDF,
};

inline BSDFType operator |(BSDFType t1, BSDFType t2) {
    return static_cast<BSDFType>((static_cast<int>(t1) | static_cast<int>(t2)));
}
inline bool operator &(BSDFType t1, BSDFType t2) {
    return (static_cast<int>(t1) & static_cast<int>(t2)) != 0;
}
inline BSDFQueryType operator |(BSDFQueryType t1, BSDFQueryType t2) {
    return static_cast<BSDFQueryType>((static_cast<int>(t1) | static_cast<int>(t2)));
}

inline bool operator &(BSDFQueryType t1, BSDFQueryType t2) {
    return (static_cast<int>(t1) & static_cast<int>(t2)) != 0;
}

class BSDF {
public:
    BSDF(VectorTexture *const reflectance) : _reflectance(reflectance) {

    }
    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) const = 0;

    virtual Vector get_reflectance(const Vector &pos, const Vector &norm) const {
        return _reflectance->get(pos, norm);
    }

    virtual BSDFType get_type() const = 0;

    Vector make_spherical(const Vector &norm, double phi, double costheta, double sintheta) const {
        Vector w = norm;
        Vector wo = w.x < -0.1 || w.x > 0.1 ? Vector::YAxis : Vector::XAxis;
        Vector u = cross(wo, w).norm();
        Vector v = cross(w, u);

        return w * costheta + u * sintheta * cos(phi) + v * sintheta * sin(phi);
    }

private:
    VectorTexture *_reflectance;
};

class LambertianBRDF : public BSDF {
public:
    LambertianBRDF(VectorTexture *const reflectance) : BSDF(reflectance) { }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        Vector abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        out.origin = pos;
        out.direct = rng->sample_hemisphere(abs_norm);
        pdf = 1;
    }

    virtual BSDFType get_type() const override { return BSDFType::Lambertian; }
};

class SpecularBRDF : public BSDF {
public:
    SpecularBRDF(VectorTexture *const reflectance) : BSDF(reflectance) { }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        out.origin = pos;
        out.direct = reflect(in.direct, norm);
        pdf = 1;
    }

    virtual BSDFType get_type() const override { return BSDFType::Specular; }
};

class PhongBRDF : public BSDF {
public:
    PhongBRDF(VectorTexture *const reflectance, int coeff=5) : BSDF(reflectance), _coeff(coeff) {

    }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        Vector abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        Vector refl = reflect(in.direct, abs_norm);

        Vector w = refl;
        Vector wo = w.x < -0.1 || w.x > 0.1 ? Vector::YAxis : Vector::XAxis;
        Vector u = cross(wo, w).norm();
        Vector v = cross(w, u);

        double theta = acos(pow(rng->get(), 1.0 / (_coeff + 1.0)));
        double phi   = 2.0 * pi * rng->get();

        pdf = 1.0;
        out.origin = pos;
        out.direct = (u * sin(theta) * cos(phi) + w * cos(theta) + v * sin(theta) * sin(phi)).norm();
    }

    virtual BSDFType get_type() const override { return BSDFType::Phong; }

private:
    int _coeff;
};

class RefractiveBTDF : public BSDF {
public:
    RefractiveBTDF(VectorTexture *const reflectance, double beta = 1.5) : BSDF(reflectance), _beta(beta) {

    }

    virtual inline void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        sample(in, pos, norm, rng, out, pdf, 0);
    }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf, int next_id) const {

        Vector abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
        bool   into = dot(norm, abs_norm) > 0;
        double alpha = 1, beta = _beta;
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

    virtual BSDFType get_type() const override { return BSDFType::Refractive; }

private:
    double _beta;
};

//class MonteCaloSamplingBSDF : BSDF {
//public:
//    MonteCaloSamplingBSDF(VectorTexture *const reflectance) : BSDF(reflectance) {
//
//    }
//    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
//                        Ray &out, double &pdf) const override {
//
//        rng->sample_sample_cos_hemisphere(pos, norm);
//        pdf = pdf(in, norm, out);
//    }
//    virtual double pdf(const Ray &in, const Vector &norm, const Ray &out) const = 0;
//
//    virtual Vector get_reflectance(const Vector &pos, const Vector &norm) const {
//        return _reflectance->get(pos, norm);
//    }
//};

class LambertianSpecularBSDF : public BSDF {
public:
    LambertianSpecularBSDF(VectorTexture *const reflectance, double diff) : BSDF(reflectance), _diff(diff) { }

    virtual void sample(const Ray &in, const Vector &pos, const Vector &norm, RandomStream *rng,
                        Ray &out, double &pdf) const override {

        if (rng->get() < _diff) {
            Vector abs_norm = (dot(norm, in.direct) < 0) ? norm : norm * -1;
            out.origin = pos;
            out.direct = rng->sample_hemisphere(abs_norm);
            pdf = 1;
        } else {
            out.origin = pos;
            out.direct = reflect(in.direct, norm);
            pdf = 1;
        }
    }

    virtual BSDFType get_type() const override { return BSDFType::Lambertian | BSDFType::Specular ; }

private:
    double _diff;
};

} // End namespace diorama

#endif //RAYTRACE_MTLDETAIL_H
