#pragma once

#include "matrixwrapper.h"
#include "types.h"
#include "filters.h"

#include <memory>

namespace xinar {
    /**
     * Class Reize
     *
     * Uses for multiple resing.
     * Holds an image and all seams that are precalculated in initialization.
     * When a resize query is get deletes lowest-value seams.
     */
    class Resize {
    private:
        core::MatWrp image; // original image

        std::shared_ptr<filter::Filter> filter; // filters

        Seams hseams; // horizontal seams; uses for speeding multi dimension resize
        Seams vseams; // vertical seams

    public:
        /**
        * Class constructor. Gets image to futher processing, prepares class
        * for future calculations. Uses standart sobel filter.
        */
         Resize() = default;
         Resize(const Resize&) = default;
         Resize(Resize&&) = default;
         Resize& operator= (Resize&&) = default;

         Resize(cv::Mat& in);
         Resize(cv::Mat&& in);

        ~Resize() = default;
        void process(cv::Mat &result, cv::Size new_size);
    };

    /**
     * Method provides resizing services.
     */
    void resize(const cv::Mat& in, cv::Mat& out, cv::Size new_size);
}
