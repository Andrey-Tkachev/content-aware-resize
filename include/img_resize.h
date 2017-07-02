#include <opencv2/opencv.hpp>
#include <vector>

namespace resize {
    typedef std::vector<cv::Point2d> PointsVec; // Pixels to add/delete
    typedef std::vector<std::vector<int>> Matrix;

    typedef PointsVec(*method)(cv::Mat &); // Method of path searching

    enum direction {
        VERT,
        HORIZ
    };

    // Remove/add k rows / columns to the image
    void remove_k(cv::Mat& in, cv::Mat& out, direction dir);
    void add_k(cv::Mat& in, cv::Mat& out, direction dir);

    std::vector<long long> calc_dynamics(cv::Mat &in);

    PointsVec
    dp_remove_method(cv::Mat &in);
    void dp_add_method();
}