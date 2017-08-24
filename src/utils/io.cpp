//
// Created by nikita on 06.07.17.
//

#include "../include/io.h"
#include <chrono>
#include <opencv2/opencv.hpp>

namespace xinar {
    namespace io {
        // IO class implementation
        IO::IO() {}

        IO::IO(std::string path_) : path(path_) {}


        //Input class implementation
        Input::Input(std::string path_) : IO(path_) {}

        Input::Input() {}

        cv::Mat Input::read_image() {
            cv::Mat mat = cv::imread(path);
            return mat;
        }

        std::string Input::get_path() {
            return path;
        }

        //Output class implementation
        Output::Output(std::string path_) : IO(path_) {}

        Output::Output() {}

        void Output::write_image(cv::Mat &mat) {
            cv::imwrite(path, mat);
        }

        std::string Output::get_path() {
            return path;
        }

        Input bind_input(const std::string &path) {
            return Input(path);
        }

        Output bind_output(const std::string &path) {
            return Output(path);
        }
    }
}
