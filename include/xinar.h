#pragma once

#include "../src/include/io.h"
#include "../src/include/matrixwrapper.h"
#include "../src/include/types.h"
#include "../src/include/filters.h"

namespace xinar {
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

        bool _is_init;

    public:
        /**
         * Class constructor. Gets no parameters, prepares class
         * for future calculations.
         */
        Resize();

        void init(cv::Mat &&in);
        void init(cv::Mat &in);
        void release();

        ~Resize() = default;

        bool is_init() const;

        void process(cv::Mat &result, cv::Size new_size);
    };

    filter::Compose build_filters();

    /**
     * Deprecated method
     */

    //void process_image(io::Input in, io::Output out, cv::Size size, bool show_images);

    //Filter *build_filters();

}
