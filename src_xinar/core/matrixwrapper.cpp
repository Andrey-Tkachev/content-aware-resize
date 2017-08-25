#include "matrixwrapper.h"

namespace xinar {
    namespace core {
        MatWrp::MatWrp() {
            this->mat = cv::Mat();
            this->transposed = false;
        }

        MatWrp::MatWrp(const MatWrp &other) {
            this->mat = other.mat;
            this->transposed = other.transposed;
        }

        MatWrp::MatWrp(const cv::Mat &other) {
            this->mat = other;
            this->transposed = false;
        }

        MatWrp::MatWrp(int h, int w, int type = CV_64F) {
            this->mat.create(h, w, type);
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
        int MatWrp::height() const {
            return ((this->transposed) ? this->mat.cols :
                    this->mat.rows);
        }

        bool
        MatWrp::is_transposed() const {
            return transposed;
        }

        void
        MatWrp::transpose() {
            this->transposed ^= 1;
        }

        void
        MatWrp::set_shape(const MatWrp &other) {
            this->mat.create(other.mat.rows, other.mat.cols, CV_64F);
            this->transposed = other.transposed;
        }

        void
        MatWrp::set_shape(const MatWrp &other, int type) {
            this->mat.create(other.mat.rows, other.mat.cols, type);
            this->transposed = other.transposed;
        }

        void
        MatWrp::set_orientation(const MatWrp &other) {
            this->transposed = other.transposed;
        }

        MatWrp
        MatWrp::operator()(cv::Range rowRange, cv::Range colRange) const {
            if (this->transposed) {
                std::swap(rowRange, colRange);
            }
            MatWrp copy((this->mat)(rowRange, colRange));
            if (this->transposed) {
                copy.transpose();
            }
            return copy;
        }

        MatWrp &
        MatWrp::operator=(const MatWrp &other) {
            this->mat = other.mat;
            this->transposed = other.transposed;
            return *this;
        }
    }
}
