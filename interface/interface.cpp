//
// Created by nikita on 06.07.17.
//

#include "interface.h"
#include "io.h"
#include "config.h"
#include "core.h"
#include "filters.h"

namespace interface {
    /*
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
        cv::Canny(src_gray, src_gray, lowThreshold, lowThreshold * ratio, kernel_size);
        //cv::imshow("grad", src_gray);
        //cv::waitKey();
        out = src_gray;
    }
    */


    filter::Compose build_filters() {
        filter::GrayScale *gs = new filter::GrayScale();
        filter::Blur *blur = new filter::Blur(/*sigma = */ 3);
        filter::Canny *canny = new filter::Canny(/*low_threshold = */ 30, /*ratio = */ 3, /*kernel_size = */ 3);

        // filter::Sobel *sobel_x = new filter::Sobel(/*x = */ 1, /*y = */ 0, /* size = */ 3,
        //                                                                   /*sc = */ 1,
        //                                                                   /*del = */  0,
        //                                                                   /*ddepth = */  IPL_DEPTH_16S,
        //                                                                   /*bT = */ cv::BORDER_DEFAULT);

        std::vector<filter::Filter *> filters = {gs, blur, canny};
        filter::Compose compose(filters);
        return compose;
    }


    void process_image(io::Input in, io::Output out, cv::Size size, bool show_images) {
        Config &config = Singleton<Config>::Instance();

        cv::Mat input_matrix = in.read_image();
        cv::Mat output_matrix = in.read_image();

        auto filter = build_filters();

        core::resize_to_fit(input_matrix, output_matrix, size, filter);

        out.write_image(output_matrix);
    }

}