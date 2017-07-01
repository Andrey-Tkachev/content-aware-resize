#include <opencv2/opencv.hpp>
#include <vector>

namespace resize {
    typedef std::vector<cv::Point2d<int>> min_cost_path; // Pixels to add/delete
    typedef std::vector<std::vector<int>> dp_matrix;
    typedef min_cost_path(*method) (cv::Mat&); // Method of path searching

    enum direction {
        VERT,
        HORIZ
    };

    // Remove/add k rows / columns to the image
    void remove_k(cv::Mat& in, cv::Mat& out, direction dir);
    void add_k(cv::Mat& in, cv::Mat& out, direction dir);

    dp_matrix calc_dynamics(cv::Mat& in);

    min_cost_path
        dp_remove_method();
    void dp_add_method();
}