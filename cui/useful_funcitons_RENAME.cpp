//
// Created by nikita on 08.07.17.
//

#pragma once

#include <boost/program_options.hpp>
#include "utils.h"
#include <chrono>

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
            ("DEBUG", "|  DEBUG MODE")
        //("show-images,s", po::value<bool>()->default_value(false)->implicit_value(true),
        // "|  show images windows while working {DEBUG}")
            ;


    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);

    if (argc == 1 || vm.count("help")) {
        std::cout << "XINAR Content Aware Resize. v.56\n"
                  << desc;
        throw 0;
    }

    c.update_from(vm);

    return vm;
}


void run_debug_code() {
    std::vector<int> a, b;
    for (int i =0 ; i != 1000 * 1000; ++i) {
        a.push_back(rand());
    }
    b = a;
    auto t0 = std::chrono::system_clock::now();
    //utils::radix_sort(a.begin(), a.end(), [](int a) { return a; });
    utils::radix_sort(a);
    std::cout << "Radix sort 1.000.000 numbers: " << (std::chrono::system_clock::now() - t0).count() << '\n';
    auto t1 = std::chrono::system_clock::now();
    std::sort(b.begin(), b.end());
    std::cout << "QSort 1.000.000 numbers: " << (std::chrono::system_clock::now() - t1).count();
}