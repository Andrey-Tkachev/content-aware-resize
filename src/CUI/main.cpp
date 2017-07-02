//
// Created by nikita on 02.07.17.
//

#include "img_preprocess.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <opencv2/opencv.hpp>
#include <img_resize.h>

namespace po = boost::program_options;

int main(int ac, char** av) {
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
                ("help", "print help page")
                ("file", po::value<std::string>(), "path to file")
                ("width", po::value<int>(), "desireable width")
                ("height", po::value<int>(), "desireable height");


        po::positional_options_description p;
        p.add("file", 1);
        p.add("width", 1);
        p.add("height", 1);


        po::variables_map vm;
        po::store(po::command_line_parser(ac, av).options(desc).positional(p).run(), vm);
        po::notify(vm);


        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 0;
        }

        if (vm.count("file")) {
            std::cout << "Got file path "
                      << vm["file"] .as<std::string>() << ".\n";
            //TODO: call functions
        } else {
            std::cout << "File path was not set.\n";
            return 1;
        }

        if (vm.count("width")) {
            std::cout << "desireable width " << vm["width"].as<int>() << '\n';
        }
        if (vm.count("height")) {
            std::cout << "desireable height " << vm["height"].as<int>() << '\n';
        }

        cv::Mat image;
        image = cv::imread(vm["file"].as<std::string>(), CV_LOAD_IMAGE_COLOR);
        if (!image.data) {
            std::cout << "error\n";
            return 1;
        }

        cv::namedWindow("image", cv::WINDOW_NORMAL);

        cv::resizeWindow("image",
                         !vm["width"].empty() ? vm["width"].as<int>() : image.size().width,
                         !vm["height"].empty() ? vm["height"].as<int>() : image.size().height);

        cv::Mat new_im;
        resize::remove_k(image, new_im, resize::HORIZ, 1);
        cv::imshow("image", new_im);
        cv::waitKey(0);

    } catch (std::exception& e) {
        std::cerr << "error: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "Exception of unknown type!\n";
    }
    return 0;
}