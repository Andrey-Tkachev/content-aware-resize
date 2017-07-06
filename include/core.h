#include <opencv2/opencv.hpp>
#include <vector>

#ifndef CONTENTAWARERESIZE_CORE_H
#define CONTENTAWARERESIZE_CORE_H

namespace core {
    class MatWrp;
    typedef long long PixelData;
    typedef std::vector<cv::Point2i> PVec; // Pixels to add/delete

    // Change image size to desirable
    // shrink_to_fit of image (640 x 480) with new_size (600 x 500) returns image (600 x 480)
    template <typename TFilter>
    void shrink_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter);

    // expand_to_fit of image (640 x 480) with new_size (600 x 500) returns image (640 x 500)
    template <typename TFilter>
    void expand_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter);

    // Remove/add k rows to the image
    template <typename TFilter> // Class with overrided operator()
    void remove_rows(MatWrp& in, int k, const TFilter& filter);

    void remove_row(PVec& points, MatWrp& from);
    void calc_dynamics(const MatWrp& in, MatWrp& dynamics);


    // Search path of pixels with min energy
    template <typename TFilter>
    PVec low_energy_path(const MatWrp& in, const TFilter& filter);

    class MatWrp {
     private:
        bool transposed;
    public:
        cv::Mat mat;
        MatWrp();
        MatWrp(const MatWrp& other);
        MatWrp(const cv::Mat& other);
        MatWrp(int h, int w, int type);

        MatWrp clone() const;

        const int width() const;
        const int hieght() const;

        template <typename T>
        T& at(int i, int j);

        template <typename T>
        const T& at(int i, int j) const;

        void transpose();
        void set_shape(const MatWrp& other);
        void set_orientation(const MatWrp& other);
        MatWrp  operator() (cv::Range rowRange, cv::Range colRange) const;
        MatWrp& operator= (const MatWrp& other);
    };

    MatWrp::MatWrp() {
        this->mat = cv::Mat();
        this->transposed = false;
    }

    MatWrp::MatWrp(const MatWrp& other) {
        this->mat = other.mat;
        this->transposed = other.transposed;
    }

    MatWrp::MatWrp(const cv::Mat& other) {
        this->mat = other;
        this->transposed = false;
    }

    MatWrp::MatWrp(int h, int w, int type=CV_64F) {
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
        this->mat.create(other.mat.rows, other.mat.cols, CV_64F);
        this->transposed = other.transposed;
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
        this->transposed = other.transposed;
        return *this;
    }

    template <typename TFilter>
    PVec low_energy_path(const MatWrp& in, const TFilter& filter) {
        MatWrp grad;
        grad.set_shape(in);
        filter(in.mat, grad.mat);
        MatWrp dynamics;
        dynamics.set_shape(grad);
        calc_dynamics(grad, dynamics);
        PixelData min = dynamics.at<PixelData>(0, in.width() - 1);
        int min_i = 0;
        for (int i = 0; i < dynamics.hieght(); ++i) {
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
            PixelData curr_min = 1e9;
            for (int delta = -1; delta <= 1; ++delta) {
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

    // Remove/add k rows to the image
    template <typename TFilter> // Class with overrided operator()
    void remove_rows(MatWrp& in, int k, const TFilter& filter) {
        for (int l = 0; l != k; ++l) {
            auto pixels_to_remove = low_energy_path(in, filter);
            remove_row(pixels_to_remove, in);
        }
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
}

#endif //CONTENTAWARERESIZE_CORE_H
