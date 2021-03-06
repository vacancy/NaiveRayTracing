/**
 * File   : canvas
 * Author : Jiayuan Mao
 * Email  : mjy14@mails.tsinghua.edu.cn
 * Date   : 2016-$MONTH-07 12:37
 * This file is part of the school project RayTracing of course
 * ``Advanced Computational Geometry''.
 **/

#include "canvas.h"
#include <opencv2/highgui/highgui.hpp>


namespace diorama {

void Canvas::show(const std::string &windowname, bool wait) {
    cv::Mat res = _to_mat();
    cv::imshow(windowname, res);
    if (wait)
        cv::waitKey(0);
}

void Canvas::write(const std::string &filename) {
    cv::Mat res = _to_mat();
    cv::imwrite(filename, res);
}

cv::Mat Canvas::_to_mat(void) {
    assert(c == 3);
    cv::Mat res(h, w, CV_8UC3);
    for (int i = 0; i < h; ++i) {
        for (int j = 0; j < w; ++j) {
            res.at<cv::Vec3b>(i, j)[2] = get(i, j, 0);
            res.at<cv::Vec3b>(i, j)[1] = get(i, j, 1);
            res.at<cv::Vec3b>(i, j)[0] = get(i, j, 2);
        }
    }
    return res;
}

} // End namespace diorama
