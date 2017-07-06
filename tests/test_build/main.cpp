#include "core.h"
#include <opencv2/opencv.hpp>
#include "iostream"

void gradient_xy(cv::Mat &in, cv::Mat &out, int x_ord,
                 int y_ord) {
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    int ker_size = 3;
    cv::Sobel(in, out, ddepth, x_ord, y_ord, ker_size, scale, delta, cv::BORDER_DEFAULT);
    cv::convertScaleAbs(out, out);
}

void gradient(const cv::Mat &in, cv::Mat &out) {
    int blur_size = 3;
    int sigma = 4;
    cv::Mat src_gray = in.clone();
    cvtColor(src_gray, src_gray, CV_BGR2GRAY);
    cv::GaussianBlur(src_gray, src_gray, cv::Size(blur_size, blur_size), sigma, sigma,
                     cv::BORDER_DEFAULT);
    cv::Mat grad_x, grad_y;
    gradient_xy(src_gray, grad_x, 1, 0);
    gradient_xy(src_gray, grad_y, 0, 1);
    addWeighted(grad_x, 0.5, grad_y, 0.5, 0, out);
}

int main() {
    cv::Mat image;
    image = cv::imread("test_images/duck_640x640.jpg", CV_LOAD_IMAGE_COLOR);

    cv::Mat new_im;
    core::shrink_to_fit(image, new_im, cv::Size(600, 600), gradient);
    cv::imshow("image", new_im);
    cv::imshow("original", image);

    while (cv::waitKey(10) != 27);
}