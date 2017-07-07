#include <opencv2/opencv.hpp>
#include <vector>
#include <boost/thread.hpp>

#ifndef CONTENTAWARERESIZE_CORE_H
#define CONTENTAWARERESIZE_CORE_H

namespace core {
    class MatWrp;
    typedef long long PixelData;
    typedef long long WeightData;
    typedef std::vector<cv::Point2i> PVec; // Pixels to add/delete

    // Change image size to desirable
    // shrink_to_fit of image (640 x 480) with new_size (600 x 500) returns image (600 x 480)
    template <typename TFilter>
    void shrink_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter, double quality);
    template <typename TFilter>
    void shrink_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter);

    // expand_to_fit of image (640 x 480) with new_size (600 x 500) returns image (640 x 500)
    template <typename TFilter>
    void expand_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter);

    // Remove/add k rows to the image
    template <typename TFilter> // Class with overrided operator()
    void remove_rows(MatWrp& in, int k, const TFilter* filter, double quality);

    void split_mat(const MatWrp& in, MatWrp& out1, MatWrp& out2);
    void remove_row(PVec& points, MatWrp& from);
    void calc_dynamics(const MatWrp& in, MatWrp& dynamics, int offset, int windows_size);

    // Search path of pixels with min energy
    PVec low_energy_path(const MatWrp& in, const MatWrp& weight_map, double quality);

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
        const int height() const;

        template <typename T>
        T& at(int i, int j);

        template <typename T>
        const T& at(int i, int j) const;

        void transpose();
        void set_shape(const MatWrp& other);
        void set_orientation(const MatWrp& other);
        MatWrp  operator() (cv::Range rowRange, cv::Range colRange) const;
        MatWrp& operator= (const MatWrp& other);
        ~MatWrp() {
            //std::cout << mat.type() << "\n";
        }
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
    int MatWrp::height() const {
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

    void
    MatWrp::set_orientation(const MatWrp& other) {
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

    // Remove/add k rows to the image
    template <typename TFilter> // Class with overrided operator()
    void remove_rows(MatWrp& in, int k, const TFilter* filter, double quality) {
        MatWrp grad;
        grad.set_shape(in);
        for (int l = 0; l != k; ++l) {
            (*filter)(in.mat, grad.mat);
            PVec pixels_to_remove = low_energy_path(in, grad, quality);
            remove_row(pixels_to_remove, in);
            remove_row(pixels_to_remove, grad);
        }
    }

    template <typename TFilter>
    void shrink_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter, double quality) {
        cv::Size in_size = in.size();
        MatWrp in_wrp(in.clone());
        if (in_size.height > new_size.height) {
            MatWrp m1, m2;
            split_mat(in_wrp, m1, m2);
            int delta = in_size.height - new_size.height;
            boost::thread rm_rows1(&remove_rows<TFilter>,
                                   boost::ref(m1),
                                   delta / 2,
                                   &filter,
                                   quality);
            boost::thread rm_rows2(&remove_rows<TFilter>,
                                   boost::ref(m2),
                                   delta / 2 + (delta & 1),
                                   &filter,
                                   quality);
            rm_rows1.join();
            rm_rows2.join();
            std::cout << "Horizontal shrink done\n";
            cv::vconcat(m1.mat, m2.mat, in_wrp.mat);

        }
        if (in_size.width > new_size.width) {
            in_wrp.transpose();
            MatWrp m1, m2;
            split_mat(in_wrp, m1, m2);

            int delta = in_wrp.height() - new_size.width;
            boost::thread rm_rows1(&remove_rows<TFilter>,
                                   boost::ref(m1),
                                   delta / 2,
                                   &filter,
                                   quality);

            boost::thread rm_rows2(&remove_rows<TFilter>,
                                   boost::ref(m2),
                                   delta / 2 + (delta & 1),
                                   &filter,
                                   quality);
            rm_rows1.join();
            rm_rows2.join();
            std::cout << "Vertical shrink done\n";
            cv::hconcat(m1.mat, m2.mat, in_wrp.mat);
        }
        out = in_wrp.mat;
    }

    template <typename TFilter>
    void shrink_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter) {
        shrink_to_fit(in, out, new_size, filter, 1.f);
    }

    template <typename TFilter>
    void expand_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, const TFilter& filter) {
        // TODO
    }
}

#endif //CONTENTAWARERESIZE_CORE_H
