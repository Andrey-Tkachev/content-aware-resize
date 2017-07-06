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

namespace po = boost::program_options;

int main(int ac, char **av) {
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,?", "|  prints help page")
            ("input-file,i", po::value<std::string>(), "|  path to input image")
            ("width,w", po::value<int>(), "|  desirable width")
            ("height,h", po::value<int>(), "|  desirable height");

    if (ac == 1) {
        std::cout << "Content aware resize. v0.43" << '\n';
        std::cout << desc << '\n';
        return 0;
    }
    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).options(desc).run(), vm);
    po::notify(vm);

    Config &config = Singleton<Config>::Instance();
    config.update_from(vm);

    if (vm.count("help")) {
        std::cout << "Content aware resize. v0.43" << '\n';
        std::cout << desc << '\n';
        return 0;
    }

    io::Input in;
    if (vm.count("input-file")) {
        in = io::bind_input(vm["input-file"].as<std::string>());
    }

    auto image = in.read_image();
    if (!image.empty()) {
        int a = 0;
    } else {
        std::cout << "Image not found. Check image path. Passed: " << in.get_path();
    }


    io::Output out;
    if (vm.count("output-file")) {
        out = io::bind_output(vm["output-file"].as<std::string>());
    } else {
        out = io::bind_output(vm["input-file"].as<std::string>() + ".out.jpg");
    }

    interface::process_image(in, out, cv::Size(vm["width"].as<int>(), vm["height"].as<int>()));

    cv::imshow("in", image);
    while (cv::waitKey(100) != 27);

    return 0;
}