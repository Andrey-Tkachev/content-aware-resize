#include "img_resize.h"
#include "img_preprocess.h"

namespace resize {

    // Remove/add k rows / columns to the image
    void remove_k(cv::Mat& in, cv::Mat& out, direction dir, int k) {
        cv::Mat current = in.clone();
        cv::Mat next(in.size(), in.type());

        for (int l = 0; l != k; ++l) {
            auto pixels_to_remove = dp_remove_method(current);
            remove_row(pixels_to_remove, current, next);
            current = next.clone();
        }

        out = next.clone();
        out = out(cv::Range(0, in.rows - k), cv::Range(0, in.cols));
    }

    void remove_row(PointsVec& points, cv::Mat& from, cv::Mat& to) {
        for (int i = 0; i != from.size().width; ++i) {
            int needed_j = -1;
            for (auto el : points) {
                if (el.x == i) {
                    if (needed_j != -1)
                        throw 0;
                    needed_j = el.y;
                }
            }
            for (int j = 0, k = 0; j < from.size().height; ++j, ++k) {
                if (j == needed_j) {
                    j++;
                }
                to.at<cv::Vec3b>(k, i) = from.at<cv::Vec3b>(j, i);
            }
        }
    }


    void add_k(cv::Mat& in, cv::Mat& out, direction dir) {
        // TODO
    }

    std::vector<double> calc_dynamics(cv::Mat& in) {
        std::vector<double> dynamics(in.rows, 0);
        std::vector<double> dynamics_front(in.rows, 0);
        for (int i = 0; i < in.rows; ++i) {
            dynamics[i] = in.at<double>(cv::Point(0, i));
        }
        for (int curr_col = 1; curr_col < in.cols; ++curr_col) {
            for (int curr_row = 0; curr_row < in.rows; ++curr_row) {
                cv::Scalar init_intent = in.at<double>(cv::Point(curr_col, curr_row));
                double curr_min = dynamics[curr_row] + static_cast<double>(init_intent[0]);
                for (int delta = -1; delta <= 1; ++delta) {
                    if (delta + curr_row < in.rows && delta + curr_row >= 0) {
                        cv::Scalar intensity = in.at<double>(cv::Point(curr_col, curr_row));
                        if (curr_min > intensity[0] + dynamics[curr_row + delta]) {
                            curr_min = static_cast<double>(intensity[0]) +
                                       dynamics[curr_row + delta];
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
    dp_remove_method(cv::Mat& in) {
        cv::Mat grad;
        preprocess::gradient(in, grad, preprocess::LOW);
        auto dynamics = calc_dynamics(grad);
        double min = dynamics[0];
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
        path.emplace_back(curr_col, curr_row);
        while (curr_col > 0) {
            int suitable_delta = 0;
            int curr_min = 1000;
            for (int delta = -1; delta <= 1; ++delta) {
                if (delta + curr_row < in.rows && delta + curr_row >= 0) {
                    if (curr_min > grad.at<uchar>(cv::Point(curr_col, curr_row + delta))) {
                        curr_min = grad.at<uchar>(cv::Point(curr_col, curr_row + delta));
                        suitable_delta = delta;
                    }
                }
            }
            curr_row += suitable_delta;
            --curr_col;
            path.emplace_back(curr_col, curr_row);
        }
        return path;
    }

    void dp_add_method() {
        // TODO
    }
}
