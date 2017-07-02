#include "img_resize.h"
#include "img_preprocess.h"
#include <opencv2/opencv.hpp>
#include <algorithm>

namespace resize {
    // Remove/add k rows / columns to the image
    void remove_k(cv::Mat &in, cv::Mat &out, direction dir) {
        // TODO
    }

    void add_k(cv::Mat &in, cv::Mat &out, direction dir) {
        // TODO
    }

    std::vector<long long> calc_dynamics(cv::Mat& in) {
        std::vector<long long> dynamics(in.rows, 0);
        std::vector<long long> dynamics_front(in.rows, 0);
        for (int i = 0; i < in.rows; ++i) {
            dynamics[i] = in.at(i, 0);
        }
        for (int curr_col = 1; curr_col < in.cols; ++curr_col) {
            for (int curr_row = 0; curr_row < in.rows; ++curr_row) {
                long long curr_min = in.at<int>(curr_row, curr_col) + dynamics[curr_row];
                for (int delta = -1; delta <= 1; ++delta) {
                    if (delta + curr_row < in.rows && delta + curr_row >= 0) {
                        if (curr_min > in.at<int>(curr_row, curr_col) + dynamics[curr_row + delta]) {
                            curr_min = in.at<int>(curr_row, curr_col) + dynamics[curr_row + delta];
                        }
                    }
                }
                dynamics_front[curr_row] = curr_min;
            }
            std::swap(dynamics_front, dynamics);
        }
        return dynamics;
    }

    PointsVec
    dp_remove_method(cv::Mat &in) {
        cv::Mat grad;
        preprocess::gradient(in, grad, preprocess::HIG);
        auto dynamics = calc_dynamics(grad);
        long long min = dynamics[0];
        int min_i = 0;
        for (int i = 0; i != dynamics.size(); ++i) {
            if (min > dynamics[i]) {
                min = dynamics[i];
                min_i = i;
            }
        }
        int curr_row = min_i;
        int curr_col = in.cols - 1;
        PointsVec path;
        path.emplace_back(curr_row, curr_col);
        while (curr_col > 0) {
            int suitable_delta = 0;
            int curr_min = 1000;
            for (int delta = -1; delta <= 1; ++delta) {
                if (delta + curr_row < in.rows && delta + curr_row >= 0) {
                    if (curr_min > grad.at<int>(curr_row + delta, curr_col)) {
                        curr_min = grad.at<int>(curr_row + delta, curr_col);
                        suitable_delta = delta;
                    }
                }
            }
            curr_row += suitable_delta;
            --curr_col;
            path.emplace_back(curr_row, curr_col);
        }
        return path;
    }

    void dp_add_method() {
        // TODO
    }
}
