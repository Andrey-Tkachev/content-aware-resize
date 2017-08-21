#pragma once

#include"io.h"
#include "matrixwrapper.h"
#include "types.h"
#include "filters.h"

namespace interface {
    /**
     * Class Reize
     *
     * Holds an image and all seams that are precalculated in initialization.
     * When a resize query is get deletes lowest-value seams.
     */
    class Resize {
    private:
        core::MatWrp image; // original image

        filter::Compose filter; // filters

        Seams hseams; // horizontal seams
        Seams vseams; // vertical seams

    public:
        Resize();
        void init(cv::Mat&& in);
        ~Resize() = default;

        bool is_init() const;

        void process(cv::Mat& result, cv::Size new_size);
    };

    filter::Compose build_filters();

    /**
     * Deprecated methiod
     */

    void process_image(io::Input in, io::Output out, cv::Size size, bool show_images);

    //Filter *build_filters();

}
