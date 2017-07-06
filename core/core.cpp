#include "core.h"
#include <opencv2/opencv.hpp>

namespace core {
    void remove_row(PVec& points, MatWrp& from) {
        for (int i = 0; i != from.width(); ++i) {
            int delta = 0;
            for (int j = 0; j != from.hieght(); ++j) {
                if (points[i].y == j) {
                    delta = -1;
                    continue;
                }
                from.at<cv::Vec3b>(j + delta, i) = from.at<cv::Vec3b>(j, i);
            }
        }
        from = from(cv::Range(0, from.hieght() - 1), cv::Range(0, from.width())).clone();
    }

    void calc_dynamics(const MatWrp& in, MatWrp& dynamics) {
        for (int i = 0; i < in.hieght(); ++i) {
            dynamics.at<PixelData>(i, 0) = in.at<PixelData>(i, 0);
        }

        for (int curr_col = 1; curr_col < in.width(); ++curr_col) {
            for (int curr_row = 0; curr_row < in.hieght(); ++curr_row) {
                PixelData curr_min = dynamics.at<PixelData>(curr_row, curr_col - 1) + in.at<PixelData>(curr_row, curr_col);
                for (int delta = -1; delta <= 1; ++delta) {
                    if (delta + curr_row < in.hieght() && delta + curr_row >= 0) {
                        if (curr_min > in.at<PixelData>(curr_col, curr_row) +
                                       dynamics.at<PixelData>(curr_row + delta, curr_col - 1)) {
                            curr_min = in.at<PixelData>(curr_row, curr_col) +
                                       dynamics.at<PixelData>(curr_row + delta, curr_col - 1);
                        }
                    }
                }
                dynamics.at<PixelData>(curr_row, curr_col) = curr_min;
            }
        }
    }
}