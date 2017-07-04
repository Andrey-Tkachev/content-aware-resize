//
// Created by nikita on 03.07.17.
//

#ifndef CONTENTAWARERESIZE_IMG_SHOW_H
#define CONTENTAWARERESIZE_IMG_SHOW_H

#include <opencv2/core/mat.hpp>

namespace img {


    void show(cv::Mat& m);

    void show(cv::Mat& m, std::string window_name);

    void show(cv::Mat& m, std::string window_name, int width, int height);

    void show(cv::Mat& m, std::string window_name, )
}


#endif //CONTENTAWARERESIZE_IMG_SHOW_H
