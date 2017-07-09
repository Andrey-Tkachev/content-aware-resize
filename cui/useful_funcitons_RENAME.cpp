//
// Created by nikita on 08.07.17.
//

#pragma once

#include <boost/program_options.hpp>

namespace po = boost::program_options;

po::variables_map process_arguments(int argc, char **argv) {
    Config &c = Singleton<Config>::Instance();
    po::options_description desc("Allowed options");
    desc.add_options()
            ("help,?", "|  prints help page")
            ("input-file,i", po::value<std::string>(), "|  path to input image")
            ("output-file,o", po::value<std::string>(), "|  path to output image")
            ("width,w", po::value<int>(), "|  desirable width")
            ("height,h", po::value<int>(), "|  desirable height")
            ("percent,p", po::value<double>(),
             "|  change both dimensions in percent")
            //("width-percent,wp", po::value<double>(), "|  changes width by percent")
            //("height-percent,hp", po::value<double>(), "|  changes height by percent")
            ("quality,q", po::value<double>()->default_value(1.)->implicit_value(1.),
             "|  image compression quality, in range (0,1]")
            ("show-images,s", po::value<bool>()->default_value(false)->implicit_value(true),
             "|  show images windows while working {DEBUG}");


    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);

    if (argc == 1 || vm.count("help")) {
        std::cout << "XINAR Content Aware Resize. v.50\n"
                  << desc;
        throw 0;
    }

    c.update_from(vm);

    return vm;
}
