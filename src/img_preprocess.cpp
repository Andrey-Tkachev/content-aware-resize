#include "img_preprocess.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/photo/photo.hpp"

namespace preprocess {
    void gradient_xy(cv::Mat &in, cv::Mat &out, int x_ord,
                     int y_ord) {
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;
        int ker_size = 3;
        cv::Sobel(in, out, ddepth, x_ord, y_ord, ker_size, scale, delta, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(out, out);
    }

    void gradient(cv::Mat& in, cv::Mat& out, blur_extent ext) {
        int blur_size = 0;
        int sigma = 0;

        switch (ext) {
            case blur_extent::LOW:
                blur_size = 3;
                break;
            case blur_extent::MID:
                 blur_size = 5;
                sigma = 2;
                break;
            case blur_extent::HIG:
                blur_size = 11;
                sigma = 4;
                break;
        }
        cv::Mat src_gray = in.clone();
        cvtColor(src_gray, src_gray, CV_BGR2GRAY);
        cv::GaussianBlur(src_gray, src_gray, cv::Size(blur_size, blur_size), sigma, sigma,
                         cv::BORDER_DEFAULT);
        //cv::bilateralFilter(src_gray.clone(), src_gray, 5, 10, 10);
        cv::Mat grad_x, grad_y;
        gradient_xy(src_gray, grad_x, 1, 0);
        gradient_xy(src_gray, grad_y, 0, 1);
        addWeighted(grad_x, 0.5, grad_y, 0.5, 1, out);
        int dilation_size = 3;
        cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                             cv::Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                             cv::Point( dilation_size, dilation_size ) );
        cv::morphologyEx(out, out, cv::MORPH_OPEN, element);
        // Apply the dilation operation
        //dilate(out, out, element);
        cv::imshow("bilateral", out);
        //out = grad_x;
    }
}
