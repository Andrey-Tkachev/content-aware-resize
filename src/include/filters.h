#pragma once

#include "opencv2/opencv.hpp"
#include "matrixwrapper.h"

namespace xinar {
    namespace filter {
        class Filter {
        public:
            virtual void operator()(cv::Mat &src, cv::Mat &dst) const = 0;

            virtual ~Filter() {}
        };


        class GausBlur : public Filter {
        private:
            cv::Size ksize;  // only positive uneven
            double sX = 0;  // sigmaX
            double sY = 0;  // sigmaY
            int bt = cv::BORDER_DEFAULT;  // boarder type

        public:
            GausBlur(cv::Size size, double X, double Y, int borT);

            void operator()(cv::Mat &src, cv::Mat &dst) const override;

            cv::Size getKsize();

            double getSX();

            double getSY();

            int getBt();

            void setKsize(cv::Size other);

            void setSX(double other);

            void setSY(double other);

            void setBt(int other);
        };


        class Sobel : public Filter {
        private:
            int xord;  // 0, 1 or 2
            int yord;  // if one of them == 0, other != 0
            int ksize = 3;  //  aperture size
            double scale = 1;  // optional scale factor for the computed derivative values
            double delta = 0;  // optional delta value that is added to the results prior to storing them in dst.
            int ddepth = IPL_DEPTH_16S;  // output image depth
            int bt = cv::BORDER_DEFAULT;  // boarder type

        public:
            Sobel(int x, int y, int size, double sc, double del, int ddepth,
                  int bT);

            void operator()(cv::Mat &src, cv::Mat &dst) const override;

            int getXord();

            int getYord();

            int getKsize();

            double getScale();

            double getDelta();

            int getDdepth();

            int getBt();

            void setXord(int other);

            void setYord(int other);

            void setKsize(int other);

            void setScale(double other);

            void setDelta(double other);

            void setDdepth(int other);

            void setBt(int other);
        };

        class Canny : public Filter {
        private:
            int low_threshold = 30;
            int ratio = 3;
            int kernel_size = 3;


        public:
            Canny(int low_threshold, int ratio, int kernel_size);

            void operator()(cv::Mat &src, cv::Mat &dst) const override;

            int getLowThreshold();

            int getRatio();

            int getKernelSize();

            void setLowThreshold(int other);

            void setRatio(int other);

            void setKernelSize(int other);

        };

        class Blur : public Filter {
        private:
            int sigma;

        public:
            Blur(int sigma);

            void operator()(cv::Mat &src, cv::Mat &dst) const override;

            int getSigma();

            void setSigma(int other);
        };

        class GrayScale : public Filter {
        public:
            GrayScale();

            void operator()(cv::Mat &src, cv::Mat &dst) const override;
        };

        class Compose : public Filter {
        private:
            std::vector<Filter *> fil;  // filtres

        public:
            Compose() = default;

            explicit Compose(const std::vector<Filter *> &filters)
                    : fil(filters) {
            }

            void operator()(cv::Mat &src, cv::Mat &dst) const {
                dst = src.clone();
                for (auto filter : fil) {
                    cv::Mat new_dst;
                    (*filter)(dst, new_dst);
                    std::swap(dst, new_dst);
                }
            }

        };
    }
}