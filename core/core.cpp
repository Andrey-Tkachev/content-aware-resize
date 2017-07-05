#include "core.h"
#include <opencv2/opencv.hpp>
#include <algorithm>

namespace core {
    MatWrp::MatWrp() {
        this->mat = cv::Mat();
        this->transposed = false;
        this->cols = 0;
        this->rows = 0;
    }

    MatWrp::MatWrp(MatWrp& other) {
        this->mat = cv::Mat(other.mat);
        this->transposed = other.transposed;
        this->cols = other.cols;
        this->rows = other.rows;
    }
    MatWrp::MatWrp(MatWrp&& other) {
        this->mat = std::move(other.mat);
        this->transposed = other.transposed;
        this->cols = other.cols;
        this->rows = other.rows;
    }

    MatWrp::MatWrp(cv::Mat& other) {
        this->mat = cv::Mat(other);
        this->cols = other.cols;
        this->rows = other.rows;
        this->transposed = false;
    }
    MatWrp::MatWrp(cv::Mat&& other) {
        this->mat = std::move(cv::Mat(other));
        this->cols = other.cols;
        this->rows = other.rows;
        this->transposed = false;
    }

    MatWrp::MatWrp(int h, int w, int type) {
        this->mat = cv::Mat::zeros(h, w, type);
        this->cols = w;
        this->rows = h;
        this->transposed = false;
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
        return ((this->transposed) ? (this->mat).at<TData> (i, j) :
                                     (this->mat).at<TData> (j, i)); // Hello, opencv index style
    }

    template <typename TData>
    const TData&
    MatWrp::at(int i, int j) const {
        return ((this->transposed) ? (this->mat).at<TData> (i, j) :
                                     (this->mat).at<TData> (j, i)); // Hello, opencv index style
    }

    void
    MatWrp::transpose() {
        this->transposed ^= 1;
        std::swap(this->cols, this->rows);
    }

    void
    MatWrp::set_shape(const MatWrp& other) {
        mat.reshape(other.mat.cols, other.mat.rows);
        cols = other.cols;
        rows = other.rows;
        transposed = other.transposed;
    }

    MatWrp
    MatWrp::operator() (cv::Range rowRange, cv::Range colRange)  const {
        MatWrp copy((this->mat)(rowRange, colRange));
        copy.transposed = this->transposed;
        return copy;
    }

    void shrink_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size) {
        // TODO
    }
    void expand_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size) {
        // TODO
    }

    // Remove/add k rows to the image
    template <typename TFilter> // Class with overrided operator()
    void remove_rows(const MatWrp& in, MatWrp& out, int k, const TFilter& filter) {
        MatWrp current(in);
        MatWrp next;
        next.set_shape(in);

        for (int l = 0; l != k; ++l) {
            auto pixels_to_remove = low_energy_path(current, filter);
            remove_row(pixels_to_remove, current, next);
            current.mat = next.mat.clone();
        }

        out.mat = next.mat.clone();
    }


    template <typename TFilter>
    void remove_row(PVec& points, const MatWrp& from, MatWrp& to) {
        for (int i = 0; i != from.cols; ++i) {
            int needed_j = -1;
            for (auto el : points) {
                if (el.x == i) {
                    if (needed_j != -1)
                        throw 0;
                    needed_j = el.y;
                }
            }
            for (int j = 0, k = 0; j < from.rows; ++j, ++k) {
                if (j == needed_j) {
                    j++;
                }
                to.at<cv::Vec3b>(k, i) = from.at<cv::Vec3b>(j, i);
            }
        }
        to = to(cv::Range(0, to.rows - 1), cv::Range(0, to.cols));
    }


    void calc_dynamics(const MatWrp& in, MatWrp& dynamics) {
        for (int i = 0; i < in.rows; ++i) {
            dynamics.at<double>(i, 0) = in.at<double>(i, 0);
        }

        for (int curr_col = 1; curr_col < in.cols; ++curr_col) {
            for (int curr_row = 0; curr_row < in.rows; ++curr_row) {
                double curr_min = dynamics.at<double>(curr_row, curr_col - 1) + in.at<double>(curr_row, curr_col);
                for (int delta = -1; delta <= 1; ++delta) {
                    if (delta + curr_row < in.rows && delta + curr_row >= 0) {
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
        MatWrp grad(in);
        filter(in.mat, grad);
        MatWrp dynamics(in.rows, in.cols, in.mat.type());
        calc_dynamics(grad, dynamics);
        double min = dynamics.at<double>(0, in.cols - 1);
        int min_i = 0;
        for (int i = 0; i < dynamics.rows; ++i) {
            if (min > dynamics.at<double>(i, in.cols - 1)) {
                min = dynamics.at<double>(i, in.cols - 1);
                min_i = i;
            }
        }
        int curr_row = min_i;
        int curr_col = in.cols - 1;
        PVec path;
        path.emplace_back(curr_col, curr_row);
        while (curr_col > 0) {
            int suitable_delta = 0;
            double curr_min = 1e8;
            for (int delta = -1; delta <= 1; ++delta) {
                if (delta + curr_row < in.rows && delta + curr_row >= 0) {
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