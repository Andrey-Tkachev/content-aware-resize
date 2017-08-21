#include "interface.h"
#include "core.h"

namespace interface {
    filter::Compose build_filters() {
        auto* gs = new filter::GrayScale();
        auto* blur = new filter::Blur(/*sigma = */ 5);
        auto* canny = new filter::Canny(/*low_threshold = */ 20, /*ratio = */ 3, /*kernel_size = */ 3);
        filter::Sobel *sbl = new filter::Sobel(0, 0, 3, 1, 0, CV_16S, cv::BORDER_DEFAULT);
        std::vector<filter::Filter *> filters = {gs, blur, sbl};
        filter::Compose compose(filters);
        return compose;
    }

    bool
    Resize::is_init() const {
        return !vseams.empty();
    }

    Resize::Resize() {
        filter = build_filters();
    }

    void
    Resize::init(cv::Mat&& in) {
        image = core::MatWrp(in);
        core::MatWrp energy(in);
        filter(in, energy.mat);
        vseams = core::get_seams(energy);
    }


    void
    Resize::process(cv::Mat& result, cv::Size new_size) {
        core::MatWrp in_wrp(image.clone());

        int h_delta = new_size.height - in_wrp.height();
        int w_delta = new_size.width - in_wrp.width();
        auto needed_vseams = Seams(vseams.begin(), vseams.begin() + std::abs(w_delta));
        core::process_seams(in_wrp, needed_vseams, w_delta < 0);
        core::MatWrp energy(in_wrp.mat);
        filter(in_wrp.mat, energy.mat);
        energy.transpose();
        in_wrp.transpose();
        auto needed_hseams = core::get_seams(energy, std::abs(h_delta));
        core::process_seams(in_wrp, needed_hseams, h_delta < 0);
        result = in_wrp.mat;
    }
}
