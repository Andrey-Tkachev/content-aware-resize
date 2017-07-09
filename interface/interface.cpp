//
// Created by nikita on 06.07.17.
//

#include "interface.h"
#include "io.h"
#include "config.h"
#include "core.h"

namespace interface {
    //TODO: replace filters
    void gradient_xy(cv::Mat &in, cv::Mat &out, int x_ord,
                     int y_ord) {
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;
        int ker_size = 5;
        cv::Sobel(in, out, ddepth, x_ord, y_ord, ker_size, scale, delta, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(out, out);
    }

    void gradient(const cv::Mat &in, cv::Mat &out) {
        int sigma = 3;
        int edgeThresh = 1;
        int lowThreshold = 30;
        int const max_lowThreshold = 100;
        int ratio = 3;
        int kernel_size = 3;
        cv::Mat src_gray = in.clone();
        cvtColor(src_gray, src_gray, CV_BGR2GRAY);
        cv::blur(src_gray, src_gray, cv::Size(sigma, sigma));
        cv::Canny(src_gray, src_gray, lowThreshold, lowThreshold*ratio, kernel_size);
        //cv::imshow("grad", src_gray);
        //cv::waitKey();
        out = src_gray;
    }

    void process_image(io::Input in, io::Output out, cv::Size size, double quality) {
        Config &config = Singleton<Config>::Instance();
        cv::Mat input_matrix = in.read_image();
        cv::Mat output_matrix = in.read_image();
        core::shrink_to_fit(input_matrix, output_matrix, size, gradient, quality);
        out.write_image(output_matrix);
        cv::imshow("out", output_matrix);
    }

}