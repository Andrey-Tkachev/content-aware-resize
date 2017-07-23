#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include "matrixwrapper.h"
#include "types.h"

namespace core {

    // Shrinks/expands width and height of the in image to сorrespond with desirable size
    // image 640x640, new size 500x700 -> new image 500x700
    // image 640x640, new size 500x500 -> new image 500x500 etc.
    template <typename TFilter>
    void resize_to_fit(const cv::Mat& in, cv::Mat& out, const cv::Size& new_size, TFilter filter);

    Seams get_seams(const MatWrp& energy);
    Seams get_seams(const MatWrp& energy, int k);
    void remove_seams(MatWrp& from, Seams seams);
    void add_seams(MatWrp& from, Seams seams);

    template <typename TFilter>
    void resize_with_filter(MatWrp& in, int delta, const TFilter& filter) {
        MatWrp energy;
        energy.set_orientation(in);
        filter(in.mat, energy.mat);
        auto seams = get_seams(energy, std::abs(delta));
        if (delta > 0) {
            remove_seams(in, seams);
        } else if (delta < 0) {
            add_seams(in, seams);
        }
    }

    void resize_with_seams(MatWrp& in, int delta, const Seams& seams) {
        Seams to_procces(seams.begin(), seams.begin() + std::abs(delta));
        if (delta > 0) {
            remove_seams(in, to_procces);
        } else if (delta < 0) {
            add_seams(in, to_procces);
        }
    }

    template<typename TFilter>
    void resize_to_fit(const cv::Mat &in, cv::Mat &out, const cv::Size &new_size, TFilter filter) {
        cv::Size in_size = in.size();
        MatWrp in_wrp(in.clone());

        resize_with_filter(in_wrp, in_size.width - new_size.width, filter);
        in_wrp.transpose();
        resize_with_filter(in_wrp, in_size.height - new_size.height, filter);

        out = in_wrp.mat;
    }
}
