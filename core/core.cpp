#include "core.h"
#include <opencv2/opencv.hpp>
#include <algorithm>
#include <iostream>

namespace core {
    MatWrp::MatWrp() {
        this->mat = cv::Mat();
        this->transposed = false;
    }

    MatWrp::MatWrp(MatWrp& other) {
        this->mat = other.mat;
        this->transposed = other.transposed;
    }
    MatWrp::MatWrp(MatWrp&& other) {
        this->mat = std::move(other.mat);
        this->transposed = other.transposed;
    }

    MatWrp::MatWrp(cv::Mat& other) {
        this->mat = other;
        this->transposed = false;
    }
    MatWrp::MatWrp(cv::Mat&& other) {
        this->mat = std::move(other);
        this->transposed = false;
    }

    MatWrp::MatWrp(int h, int w, int type) {
        this->mat = cv::Mat::zeros(h, w, type);
        this->transposed = false;
    }

    MatWrp
    MatWrp::clone() const {
        MatWrp cl((this->mat).clone());
        cl.transposed = this->transposed;
        return cl;
    }

    const
    int MatWrp::width() const {
        return ((this->transposed) ? this->mat.rows :
                                     this->mat.cols);
    }
    const
    int MatWrp::hieght() const {
        return ((this->transposed) ? this->mat.cols :
                                     this->mat.rows);
    }

    template <typename TData>
    TData&
    MatWrp::at(int i, int j) {
        return ((this->transposed) ? (this->mat).at<TData> (j, i) :
                                     (this->mat).at<TData> (i, j)); // Hello, opencv index style
    }

    template <typename TData>
    const TData&
    MatWrp::at(int i, int j) const {
        return ((this->transposed) ? (this->mat).at<TData> (j, i) :
                                     (this->mat).at<TData> (i, j)); // Hello, opencv index style
    }

    void
    MatWrp::transpose() {
        this->transposed ^= 1;
    }

    void
    MatWrp::set_shape(const MatWrp& other) {
        mat = cv::Mat(other.mat.cols, other.mat.rows, other.mat.type());
        transposed = other.transposed;
    }

    void
    MatWrp::set_orientation(const MatWrp& other) {
        if (other.transposed && !transposed) {
            this->transpose();
        }
    }

    MatWrp
    MatWrp::operator() (cv::Range rowRange, cv::Range colRange)  const {
        if (this->transposed) {
            std::swap(rowRange, colRange);
        }
        MatWrp copy((this->mat)(rowRange, colRange));
        if (this->transposed) {
            copy.transpose();
        }
        return copy;
    }

    MatWrp&
    MatWrp::operator= (const MatWrp& other) {
        this->mat = other.mat;
        this->transposed = false;
        if (other.transposed) {
            this->transpose();
        }
        return *this;
    }

    MatWrp&
    MatWrp::operator= (MatWrp&& other) {
        this->mat = std::move(other.mat);
        this->transposed = false;
        if (other.transposed) {
            this->transpose();
        }
        return *this;
    }

    template <typename TFilter>
    void shrink_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter) {
        cv::Size in_size = in.size();
        MatWrp in_wrp(in.clone());
        MatWrp out_wrp;
        if (in_size.height > new_size.height) {
            remove_rows(in_wrp, in_size.height - new_size.height, filter);
        }
        if (in_size.width > new_size.width) {
            in_wrp.transpose();
            remove_rows(in_wrp, in_size.width - new_size.width, filter);
        }
        out = in_wrp.mat;
    }

    template <typename TFilter>
    void expand_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter) {
        // TODO
    }

    // Remove/add k rows to the image
    template <typename TFilter> // Class with overrided operator()
    void remove_rows(MatWrp& in, int k, const TFilter& filter) {
        for (int l = 0; l != k; ++l) {
            auto pixels_to_remove = low_energy_path(in, filter);
            remove_row(pixels_to_remove, in);
        }
    }

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
        from = from(cv::Range(0, from.hieght() - 1), cv::Range(0, from.width()));
    }

    void calc_dynamics(const MatWrp& in, MatWrp& dynamics) {
        for (int i = 0; i < in.hieght(); ++i) {
            dynamics.at<double>(i, 0) = in.at<double>(i, 0);
        }

        for (int curr_col = 1; curr_col < in.width(); ++curr_col) {
            for (int curr_row = 0; curr_row < in.hieght(); ++curr_row) {
                double curr_min = dynamics.at<double>(curr_row, curr_col - 1) + in.at<double>(curr_row, curr_col);
                for (int delta = -1; delta <= 1; ++delta) {
                    if (delta + curr_row < in.hieght() && delta + curr_row >= 0) {
                        if (curr_min > in.at<double>(curr_col, curr_row) +
                                       dynamics.at<double>(curr_row + delta, curr_col - 1)) {
                            curr_min = in.at<double>(curr_row, curr_col) +
                                       dynamics.at<double>(curr_row + delta, curr_col - 1);
                        }
                    }
                }
                dynamics.at<double>(curr_row, curr_col) = curr_min;
            }
        }
    }

    template <typename TFilter>
    PVec low_energy_path(const MatWrp& in, const TFilter& filter) {
        MatWrp grad;
        grad.set_shape(in);
        filter(in.mat, grad.mat);
        MatWrp dynamics(in.hieght(), in.width(), in.mat.type());
        calc_dynamics(grad, dynamics);
        double min = dynamics.at<double>(0, in.width() - 1);
        int min_i = 0;
        for (int i = 0; i < dynamics.hieght(); ++i) {
            if (min > dynamics.at<double>(i, in.width() - 1)) {
                min = dynamics.at<double>(i, in.width() - 1);
                min_i = i;
            }
        }
        int curr_row = min_i;
        int curr_col = in.width() - 1;
        PVec path;
        path.emplace_back(curr_col, curr_row);
        while (curr_col > 0) {
            int suitable_delta = 0;
            double curr_min = 1e8;
            for (int delta = -1; delta <= 1; ++delta) {
                if (delta + curr_row < in.hieght() && delta + curr_row >= 0) {
                    if (curr_min > dynamics.at<double>(curr_row + delta, curr_col)) {
                        curr_min = dynamics.at<double>(curr_row + delta, curr_col);
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