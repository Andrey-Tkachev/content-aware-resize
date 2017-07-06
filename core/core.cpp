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
}