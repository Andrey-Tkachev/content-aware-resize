#ifndef CONTENTAWARERESIZE_IMG_PREPROCESS_H
#define CONTENTAWARERESIZE_IMG_PREPROCESS_H

namespace preprocess {

    enum blur_extent { // Extent of gaussian blur which is used before Sobel gradient
        LOW,
        MID,
        HIG
    };

    void gradient(cv::Mat& in, cv::Mat& out, blur_extent ext);
}

#endif //CONTENTAWARERESIZE_IMG_PREPROCESS_H
