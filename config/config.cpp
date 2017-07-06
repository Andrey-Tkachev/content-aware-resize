//
// Created by nikita on 04.07.17.
//

// Implementation of config.h

#include <iostream>
#include <fstream>
#include "config.h"

Config::Config() :data(new std::map<std::string, std::string>()){
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

void Config::update_from(boost::program_options::variables_map vm) {
    for (auto el : *data) {
        if (vm.count(el.first)) {
            (*data)[el.first] = vm[el.first].as<std::string>();
        }
    }
}