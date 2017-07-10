//
// Created by nikita on 05.07.17.
//
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "io.h"
#include "config.h"
#include "interface.h"
#include <boost/program_options.hpp>
#include "useful_funcitons_RENAME.cpp"

namespace po = boost::program_options;

int main(int argc, char **argv) {
    Config &config = Singleton<Config>::Instance();
    po::variables_map vm;
    try {
        vm = process_arguments(argc, argv);
    } catch (...) {
        return 0;
    }

    //config.update_from(vm);

    io::Input in;
    io::Output out;

    if (vm.count("input-file")) {
        in = io::bind_input(vm["input-file"].as<std::string>());
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

    if (vm.count("output-file")) {
        out = io::bind_output(vm["output-file"].as<std::string>());
    } else {
        out = io::bind_output(vm["input-file"].as<std::string>() + ".out.jpg");
    }

    bool show_images = vm.count("show-images") ? vm["show-images"].as<bool>() : false;

    interface::process_image(in, out, cv::Size(width, height),
                             show_images);


    if (vm["show-images"].as<bool>()) {
        cv::namedWindow("in", cv::WINDOW_NORMAL);
        cv::imshow("in", image);
        while (cv::waitKey(100) != 27);
    }
    return 0;
}