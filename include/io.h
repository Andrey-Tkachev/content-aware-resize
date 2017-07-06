//
// Created by nikita on 05.07.17.
//

#ifndef CONTENTAWARERESIZE_IO_H
#define CONTENTAWARERESIZE_IO_H

#include <boost/program_options.hpp>
#include <opencv2/core/mat.hpp>

namespace io {
    class IO {
    protected:
        std::string path;

    public:
        IO();

        IO(std::string path_);

    };


    class Input : protected IO {
    public:
        Input();

        Input(std::string path_);

        cv::Mat read_image();

        std::string get_path();
    };


    class Output : protected IO {
    public:
        Output();

        Output(std::string path_);

        void write_image(cv::Mat &mat);

        std::string get_path();
    };

    Input bind_input(const std::string &path);

    Output bind_output(const std::string &path);

    boost::program_options::variables_map parse_console_arguments(int ac, char **av);
}


#endif //CONTENTAWARERESIZE_IO_H
