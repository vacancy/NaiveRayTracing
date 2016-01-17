/**
 * File   : texture
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-11 12:58
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#ifndef RAYTRACE_TEXTURE_H_H
#define RAYTRACE_TEXTURE_H_H

#include "../core/common.h"
#include "../core/vector3.h"
#include <opencv2/opencv.hpp>

namespace diorama {

template <typename T>
class Texture {
public:
    typedef T value_t;

    virtual T get(const Vector &pos, const Vector &norm) const = 0;
};

template <typename T>
class ConstantTexture : public Texture<T> {
public:
    typedef T value_t;

    ConstantTexture(const value_t &c) : _const(c) {

    }
    virtual T get(const Vector &pos, const Vector &norm) const override {
        return _const;
    }

private:
    T _const;
};

class BmpTexture : public Texture<Vector> {
public:
    BmpTexture(std::string filename, const Vector &origin, const Vector &u, const Vector &v, double du, double dv, double gamma = 1.)
        : _origin(origin), _u(u), _v(v), _du(du), _dv(dv) {

        cv::Mat raw = cv::imread(filename, 1);
        cv::Mat rawf;
        raw.convertTo(rawf, CV_32FC3, 1. / 255, 0);
        if (abs(gamma - 1) > eps) {
            cv::pow(rawf, gamma, _picture);
        } else {
            _picture = rawf;
        }
    }

    virtual Vector get(const Vector &pos, const Vector &norm) const override {
        int uu = static_cast<int>(dot((pos - _origin), _u) * _du);
        int vv = static_cast<int>(dot((pos - _origin), _v) * _dv);
        uu %= _picture.cols, vv %= _picture.rows;
        if (uu < 0) uu += _picture.cols;
        if (vv < 0) vv += _picture.rows;
        cv::Vec3f res = _picture.at<cv::Vec3f>(vv, uu);
        return Vector(res[2], res[1], res[0]);
    }

protected:
    cv::Mat _picture;
    Vector _origin, _u, _v;
    double _du, _dv;
};

class BumpTexture : public BmpTexture {
public:
    BumpTexture(std::string filename, const Vector &origin, const Vector &u, const Vector &v, double du, double dv, double gamma = 1., double coeff = 1.)
        : BmpTexture(filename, origin, u, v, du, dv, gamma), _coeff(coeff) {

    }

    virtual Vector get(const Vector &pos, const Vector &norm) const override {
        int uu = static_cast<int>(dot((pos - _origin), _u) * _du);
        int vv = static_cast<int>(dot((pos - _origin), _v) * _dv);

        uu %= _picture.cols, vv %= _picture.rows;
        if (uu < 0) uu += _picture.cols;
        if (vv < 0) vv += _picture.rows;
        cv::Vec3f du, dv;
        if (uu == 0) {
            du = (_picture.at<cv::Vec3f>(vv, uu+1) - _picture.at<cv::Vec3f>(vv, uu)) * 2;
        } else if (uu == _picture.cols - 1) {
            du = (_picture.at<cv::Vec3f>(vv, uu) - _picture.at<cv::Vec3f>(vv, uu-1)) * 2;
        } else {
            du = _picture.at<cv::Vec3f>(vv, uu+1) - _picture.at<cv::Vec3f>(vv, uu-1);
        }
        if (vv == 0) {
            dv = (_picture.at<cv::Vec3f>(vv+1, uu) - _picture.at<cv::Vec3f>(vv, uu)) * 2;
        } else if (vv == _picture.rows - 1) {
            dv = (_picture.at<cv::Vec3f>(vv, uu) - _picture.at<cv::Vec3f>(vv-1, uu)) * 2;
        } else {
            dv = _picture.at<cv::Vec3f>(vv+1, uu) - _picture.at<cv::Vec3f>(vv-1, uu);
        }

        return (norm + du[0] * _u * _coeff + dv[0] * _v * _coeff).norm();
    }

private:
    double _coeff;
};

typedef Texture<Vector> VectorTexture;
typedef Texture<float>  FloatTexture;

} // End namespace diorama

#endif //RAYTRACE_TEXTURE_H_H
