#include "xinar.h"
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <singleton.h>
#include <utils.h>
#include <io.h>
#include <config.h>

using namespace xinar_utils;

namespace fs = boost::filesystem;
namespace po = boost::program_options;

void run_debug_code() {
    auto config = Singleton<io::Config>::Instance();
    std::cout << config["LOW_REL"].as_string();
}


std::string create_default_filepath(const std::string &path) {
    return path + ".out.jpg"; //TODO: more smart changed image name
}

constexpr inline const char *help_page() {
    return "This is a help page."; //TODO: write help page
}

bool check_dims(cv::Mat image, cv::Size desireable_size) {
    /* if (desireable_size.height > 2 * image.rows ||
        desireable_size.width > 2 * image.cols) {
        return false;
    }*/
    return true;
}

std::string possible_dims(cv::Mat image) {
    std::stringstream ss;
    ss << "width: [0.." << image.cols << ".." << image.cols * 2 << "], "
       << "height: [0.." << image.rows << ".." << image.rows * 2 << "]";
    return ss.str();
}

cv::Size update_size(std::string parameter) {
    cv::Size size;
    auto config = Singleton<io::Config>::Instance();
    if (parameter == "LOW") {
        size = config["LOW"].as_cvsize();
    } else if (parameter == "MID") {
        size = config["MID"].as_cvsize();
    } else if (parameter == "HIGH") {
        size = config["HIGH"].as_cvsize();
    } else {
        throw "Invalid preset";
    }
    return size;
}

cv::Size update_size(const boost::program_options::variables_map& vm, cv::Size current_size) {
    if (vm.count("width")) {
        current_size.width = vm["width"].as<int>();
    }

    if (vm.count("height")) {
        current_size.height = vm["height"].as<int>();
    }
    return current_size;
}

boost::program_options::options_description build_options() {
    boost::program_options::options_description desc("Welcome to xinar! Allowed options");
    desc.add_options()
            ("help,?", "|  prints this page")
            ("input-file,i", po::value<std::string>(), "|  path to input image")
            ("mask,m", po::value<std::string>(), "|  path to mask image")
            ("output-file,o", po::value<std::string>(), "|  path to output image")
            ("width,w", po::value<int>(), "|  desirable width")
            ("height,h", po::value<int>(), "|  desirable height")
            ("preset,p", po::value<std::string>(), "|  name of preset in config file")
            ("config", "|  print current config")
            ("DEBUG", "|  DEBUG");
    return desc;
}

void update_vm(int argc, char** argv,
               boost::program_options::variables_map& vm,
               boost::program_options::options_description& desc) {
    std::stringstream error;
    try {
        boost::program_options::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    } catch (boost::program_options::invalid_command_line_syntax &e) {
        error << "No value for option " << e.get_option_name() << "." << std::endl;
        throw error.str();
    } catch (boost::program_options::invalid_command_line_style &e) {
        error << e.what() << std::endl;
        throw error.str();
    } catch (boost::program_options::unknown_option &e) {
        error << "Option " << e.get_option_name() << " is not supported. --help for info" << '\n';
        throw error.str();
    }
}
