#include <opencv2/opencv.hpp>
#include <vector>

namespace resize {
    typedef std::vector<cv::Point2i> PointsVec; // Pixels to add/delete
    typedef std::vector<std::vector<int>> Matrix;

    typedef PointsVec(*method)(cv::Mat &); // Method of path searching

    enum direction {
        VERT,
        HORIZ
    };

    // Remove/add k rows / columns to the image
    void remove_k(cv::Mat& in, cv::Mat& out, direction dir, int k);
    void add_k(cv::Mat& in, cv::Mat& out, direction dir);

    void remove_row(PointsVec& points, cv::Mat& from, cv::Mat& to);

    std::vector<double> calc_dynamics(cv::Mat& in);

    PointsVec dp_remove_method(cv::Mat& in);

    void dp_add_method();
}