#include <opencv2/opencv.hpp>
#include <vector>

#ifndef CONTENTAWARERESIZE_CORE_H
#define CONTENTAWARERESIZE_CORE_H

namespace core {
    typedef std::vector<cv::Point2i> PVec; // Pixels to add/delete

    class MatWrp {
     private:
        bool transposed;
    public:
        cv::Mat mat;
        MatWrp();
        MatWrp(MatWrp& other);
        MatWrp(MatWrp&& other);
        MatWrp(cv::Mat& other);
        MatWrp(cv::Mat&& other);
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
        MatWrp& operator= (MatWrp&& other);
    };

    // Change image size to desirable
    // shrink_to_fit of image (640 x 480) with new_size (600 x 500) returns image (600 x 480)
    // expand_to_fit of image (640 x 480) with new_size (600 x 500) returns image (640 x 500)
    template <typename TFilter>
    void shrink_to_fit(const cv::Mat in, cv::Mat out, const cv::Size& new_size, const TFilter& filter);
    template <typename TFilter>
    void expand_to_fit(const cv::Mat in, cv::Mat out, const cv::Size& new_size, const TFilter& filter);

    // Remove(add) k rows of dots from(to) matrix "in"
    template <typename TFilter>
    void remove_rows(const MatWrp& in, MatWrp& out, int k, const TFilter& filter);
    template <typename TFilter>
    void    add_rows(const MatWrp& in, MatWrp& out, int k, const TFilter& filter);

    void remove_row(PVec& points, MatWrp& from);
    void    add_row(PVec& points, MatWrp& from);

    template <typename TFilter>
    PVec low_energy_path(const MatWrp& in, const TFilter& filter);
    MatWrp calc_dynamics(const MatWrp& in);
}

#endif //CONTENTAWARERESIZE_CORE_H
