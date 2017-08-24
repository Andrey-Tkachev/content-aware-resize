//
// Created by nikita on 22.08.17.
//

#include "console_handler.cpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;

using namespace xinar;




int main(int argc, char **argv) {
    auto config = Singleton<io::Config>::Instance();
    po::options_description desc = build_options();
    po::variables_map vm;
    try {
        update_vm(argc, argv, vm, desc);
    } catch (std::string error) {
        std::cerr << error << '\n';
        return 1;
    }
    po::notify(vm);

    if (argc == 2 && vm.count("DEBUG")) {
        run_debug_code();
        return 0;
    }

    if (argc == 1 || vm.count("help")) {
        std::cout << desc << '\n' << help_page() << std::endl;
        return 0;
    }

    if (vm.count("config")) {
        config.print(std::cout);
        return 0;
    }

    io::Input in;
    io::Output out;

    if (vm.count("input-file")) {
        in = io::bind_input(vm["input-file"].as<std::string>());
    } else {
        std::cerr << "No input file providen!\n";
        return 1;
    }

    if (vm.count("output-file")) {
        out = io::bind_output(vm["output-file"].as<std::string>());
    } else {
        out = io::bind_output(create_default_filepath(in.get_path()));
    }

    auto image = in.read_image();

    if (image.empty()) {
        std::cerr << "Error while opening file. Passed: \"" << in.get_path() << "\"\n";
        return 1;
    }

    cv::Size size(image.rows, image.cols);

    if (vm.count("preset")) {
        auto parameter = vm["preset"].as<std::string>();
        try {
            size = update_size(parameter);
        } catch (char *e) {
            std::cout << e;
            return 1;
        }

    } else if (vm.count("width") || vm.count("height")) {
        size = update_size(vm, size);
    } else {
        std::cerr << "No size providen. Use either --width/--height or --preset command. --help for more info\n";
        return 1;
    }
    if (!check_dims(image, size)) {
        std::cerr << "Image can't be resized to " << size
                  << ". Possible dimentions: " << possible_dims(image) << ". --help to more info\n";
        return 1;
    }

    Resize resizer;
    resizer.init(image);
    cv::Mat output;
    resizer.process(output, size);
    out.write_image(output);
    return 0;
}
