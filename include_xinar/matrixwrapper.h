#pragma once

//#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

namespace xinar {
    namespace core {
        class MatWrp {
        private:
            bool transposed;
        public:

            cv::Mat mat;

            MatWrp();

            MatWrp(const MatWrp &other);

            MatWrp(const cv::Mat &other);

            MatWrp(int h, int w, int type);

            MatWrp clone() const;


            bool is_transposed() const;

            const int width() const;

            const int height() const;

            template<typename T>
            T &at(int i, int j);

            template<typename T>
            const T &at(int i, int j) const;

            void transpose();

            void set_shape(const MatWrp &other);

            void set_shape(const MatWrp &other, int type);

            void set_orientation(const MatWrp &other);

            MatWrp operator()(cv::Range rowRange, cv::Range colRange) const;

            MatWrp &operator=(const MatWrp &other);
        };

        template<typename TData>
        TData &
        MatWrp::at(int i, int j) {
            return ((this->transposed) ?
                    (this->mat).at<TData>(j, i) :
                    (this->mat).at<TData>(i, j)); // Hello, opencv index style
        }

        template<typename TData>
        const TData &
        MatWrp::at(int i, int j) const {
            return ((this->transposed) ?
                    (this->mat).at<TData>(j, i) :
                    (this->mat).at<TData>(i, j)); // Hello, opencv index style
        }

    }
}