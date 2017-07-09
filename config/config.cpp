//
// Created by nikita on 04.07.17.
//

// Implementation of config.h

#include <iostream>
#include <fstream>
#include "config.h"

namespace po = boost::program_options;

Config::Config() : data(new std::map<std::string, std::string>()) {
    parse_config();
}

void Config::parse_config() {
    std::ifstream parameters;
    parameters.open("configuration");
    std::string word;
    std::string key, value;
    while (parameters >> key >> value) {
        data->emplace(key, value);
    }
}

Config::~Config() {
}

std::string Config::operator[](const std::string &parameter) {
    std::string result;
    try {
        result = data->at(parameter);
    } catch (...) {
        std::cerr << "No parameter found. Passed '" << parameter << "'.";
        throw -1;
    }
    return result;
}

void Config::update_from(const po::variables_map &vm) {
    //TODO: rewrite config subsystem
    /*for (const auto &el : vm) {
        (*data)[el.first] = boost::any_cast<std::string>(&el.second);
    }*/
}

void Config::print() {
    std::cout << "====================\n"
              << "CURRENT CONFIG:\n";
    for (const auto &el : *data) {
        std::cout << el.first << " -- " << el.second << '\n';
    }
    std::cout << "====================\n";

}