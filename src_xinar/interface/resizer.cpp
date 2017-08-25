#include "xinar.h"
#include "core.h"

namespace xinar {
    void _resize_width(core::MatWrp& in, int delta, std::shared_ptr<filter::Filter> filter) {
        if (delta == 0) return;
        core::MatWrp energy_wrp(in.mat.rows, in.mat.cols, in.mat.type());
        energy_wrp.set_orientation(in);
        (*filter)(in.mat, energy_wrp.mat);
        auto seams = core::get_seams(energy_wrp, std::abs(delta));
        core::process_seams(in, seams, delta < 0);
    }

    std::shared_ptr<filter::Filter> build_filters() {
        auto gs = std::shared_ptr<filter::Filter>(new filter::GrayScale());
        auto blur = std::shared_ptr<filter::Filter>(new filter::Blur(/*sigma = */ 3));
        //auto canny = std::shared_ptr<filter::Filter>(new filter::Canny(/*low_threshold = */ 20, /*ratio = */ 3, /*kernel_size = */ 3));
        auto sbl = std::shared_ptr<filter::Filter>(new filter::Sobel(0, 0, 3, 1, 0, CV_16S, cv::BORDER_DEFAULT));
        std::vector<std::shared_ptr<filter::Filter>> filters = {gs, blur, sbl};
        return std::shared_ptr<filter::Filter>(new filter::Compose(filters));
    }

    Resize::Resize(cv::Mat &&in) {
        filter = build_filters();
        image = core::MatWrp(in);
        core::MatWrp energy(in.rows, in.cols, in.type());
        (*filter)(in, energy.mat);
        vseams = core::get_seams(energy);
        energy.transpose();
        hseams = core::get_seams(energy);
    }

    Resize::Resize(cv::Mat &in) {
        filter = build_filters();
        image = core::MatWrp(in);
        core::MatWrp energy(in.rows, in.cols, in.type());
        (*filter)(in, energy.mat);
        vseams = core::get_seams(energy);
        energy.transpose();
        hseams = core::get_seams(energy);
    }

    void
    Resize::process(cv::Mat &result, cv::Size new_size) {
        int orig_w = image.mat.cols,
            orig_h = image.mat.rows;
        int h_delta = new_size.height - orig_h,
            w_delta = new_size.width - orig_w;

        h_delta = std::min(std::abs(h_delta), orig_h) * (h_delta < 0 ? -1 : 1);
        w_delta = std::min(std::abs(w_delta), orig_w) * (w_delta < 0 ? -1 : 1);
        
        core::MatWrp in_wrp(image.clone());
        auto vseams_p = &vseams,
             hseams_p = &hseams; 

        if (w_delta + in_wrp.width() > h_delta + in_wrp.height()) {
            std::swap(vseams_p, hseams_p);
            std::swap(w_delta, h_delta);
            in_wrp.transpose();
        }
        
        auto needed_vseams = Seams(vseams_p->begin(), vseams_p->begin() + std::abs(w_delta));
        core::process_seams(in_wrp, needed_vseams, w_delta < 0);

        in_wrp.transpose();
        _resize_width(in_wrp, h_delta, filter);
        result = in_wrp.mat;
    }

    void resize(const cv::Mat& image, cv::Mat& out, cv::Size new_size) {
        int orig_w = image.cols,
            orig_h = image.rows;
        int h_delta = new_size.height - orig_h,
            w_delta = new_size.width - orig_w;

        h_delta = std::min(std::abs(h_delta), orig_h) * (h_delta < 0 ? -1 : 1);
        w_delta = std::min(std::abs(w_delta), orig_w) * (w_delta < 0 ? -1 : 1);
        
        core::MatWrp in_wrp(image.clone());
        auto filter = build_filters();
        if (w_delta + in_wrp.width() > h_delta + in_wrp.height()) {
            std::swap(w_delta, h_delta);
            in_wrp.transpose();
        }

        _resize_width(in_wrp, w_delta, filter);
        in_wrp.transpose();
        _resize_width(in_wrp, h_delta, filter);
        out = in_wrp.mat;
    }

}
