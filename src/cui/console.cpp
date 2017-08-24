//
// Created by nikita on 05.07.17.
//
#include <boost/program_options.hpp>
#include "xinar.h"
#include "config.h"

namespace po = boost::program_options;

int main(int argc, char **argv) {
    po::variables_map vm;
    try {
        vm = process_arguments(argc, argv);
    } catch (...) {
        return 0;
    }

    if (vm.count("DEBUG")) {
        run_debug_code();
        return 0;
    }

    xinar::io::Input in;
    xinar::io::Output out;

    if (vm.count("input-file")) {
        in = xinar::io::bind_input(vm["input-file"].as<std::string>());
    } else {
        std::cerr << "No input file providen!\n";
        return -1;
    }

    auto image = in.read_image();
    if (image.empty()) {
        std::cerr << "No image found! Passed: \"" << in.get_path() << "\".";
        return -1;
    }


    int height = 0, width = 0;

    if (vm.count("height")) {
        height = vm["height"].as<int>();
    } else {
        std::cerr << "No height providen!\n";
    }
    if (vm.count("width")) {
        width = vm["width"].as<int>();
    } else {
        std::cerr << "No width providen!\n";
    }

    if ()

    if (vm.count("output-file")) {
        out = xinar::io::bind_output(vm["output-file"].as<std::string>());
    } else {
        out = xinar::io::bind_output(vm["input-file"].as<std::string>() + ".out.jpg");
    }

    xinar::Resize resize;
    resize.init(in.read_image());
    cv::Mat out_matrix;
    resize.process(out_matrix, cv::Size(width, height));
    out.write_image(out_matrix);
    return 0;
}
