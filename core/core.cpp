#include "core.h"
#include <opencv2/opencv.hpp>

namespace core {
    static unsigned long _x=123456789, _y=362436069, _z=521288629;

    unsigned long xorshf96(void) {          //period 2^96-1
        unsigned long t;
        _x ^= _x << 16;
        _x ^= _x >> 5;
        _x ^= _x << 1;

        t = _x;
        _x = _y;
        _y = _z;
        _z = t ^ _x ^ _y;

        return _z;
    }

    void remove_row(PVec& points, MatWrp& from) {
        for (int i = 0; i != from.width(); ++i) {
            int delta = -1;
            for (int j = points[i].y + 1; j != from.hieght(); ++j) {
                switch (from.mat.type()) {
                    case CV_64F:
                        from.at<double>(j + delta, i) = from.at<double>(j, i);
                        break;

                    case CV_8UC1:
                        from.at<uchar>(j + delta, i) = from.at<uchar>(j, i);
                        break;

                    case CV_8UC3:
                    default:
                        from.at<cv::Vec3b>(j + delta, i) = from.at<cv::Vec3b>(j, i);
                        break;
                }
            }
        }
        from = from(cv::Range(0, from.hieght() - 1), cv::Range(0, from.width())).clone();
    }

    void calc_dynamics(const MatWrp& in, MatWrp& dynamics, int offset, int window_size) {
        for (int i = offset; i < offset + window_size; ++i) {
            dynamics.at<PixelData>(i, 0) = in.at<PixelData>(i, 0);
        }

        for (int curr_col = 1; curr_col < in.width(); ++curr_col) {
            for (int curr_row = offset; curr_row < offset + window_size; ++curr_row) {
                    PixelData curr_min = dynamics.at<PixelData>(curr_row, curr_col - 1) + in.at<WeightData>(curr_row, curr_col);
                for (int delta = -1; delta <= 1; delta += 2) {
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

    PVec low_energy_path(const MatWrp& in, const MatWrp& grad, double quality) {
        MatWrp dynamics;
        dynamics.set_shape(grad);
        int window_size = static_cast<double>(in.hieght()) * quality;
        int offset = static_cast<int>(xorshf96() % static_cast<unsigned long>(std::max(in.hieght() - window_size, 1)));
        calc_dynamics(grad, dynamics, offset, window_size);
        PixelData min = dynamics.at<PixelData>(0, in.width() - 1);
        int min_i = 0;
        for (int i = offset; i < offset + window_size; ++i) {
            if (min > dynamics.at<PixelData>(i, in.width() - 1)) {
                min = dynamics.at<PixelData>(i, in.width() - 1);
                min_i = i;
            }
        }
        int curr_row = min_i;
        int curr_col = in.width() - 1;
        PVec path;
        path.emplace_back(curr_col, curr_row);
        while (curr_col > 0) {
            int suitable_delta = 0;
            PixelData curr_min = dynamics.at<PixelData>(curr_row, curr_col);
            for (int delta = -1; delta <= 1; ++delta, ++delta) {
                if (delta + curr_row < in.hieght() && delta + curr_row >= 0) {
                    if (curr_min > dynamics.at<PixelData>(curr_row + delta, curr_col)) {
                        curr_min = dynamics.at<PixelData>(curr_row + delta, curr_col);
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
}