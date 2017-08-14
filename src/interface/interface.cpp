#include "interface.h"
#include "filters.h"
#include "core.h"

namespace interface {
    filter::Compose build_filters() {
        auto* gs = new filter::GrayScale();
        auto* blur = new filter::Blur(/*sigma = */ 3);
        auto* canny = new filter::Canny(/*low_threshold = */ 20, /*ratio = */ 3, /*kernel_size = */ 3);
        std::vector<filter::Filter *> filters = {gs, blur, canny};
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
        for (auto seam : vseams) delete seam;
        vseams.clear();
        image = core::MatWrp(in);
        core::MatWrp energy(in);
        filter(in, energy.mat);
        vseams = core::get_seams(energy);
        energy.transpose();
    }

    Resize::~Resize() {
        for (auto seam : vseams) delete seam;
        vseams.clear();
    }

    void
    Resize::process(cv::Mat& result, cv::Size new_size) {
        cv::Size in_size = image.mat.size();
        core::MatWrp in_wrp(image.clone());

        core::resize_with_seams(in_wrp, in_size.width - new_size.width, vseams);
        in_wrp.transpose();
        core::resize_with_filter(in_wrp, in_size.height - new_size.height, filter);
        result = in_wrp.mat;
    }
}
