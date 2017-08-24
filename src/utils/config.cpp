//
// Created by nikita on 23.08.17.
//
#include "config.h"
#include "utils.h"

namespace fs = boost::filesystem;

namespace xinar {
    namespace io {
        Config::Config() {
            fs::path home_dir(getenv("HOME"));
            home_dir /= ".xinar";
            fs::path home_config(home_dir / "config");

            if (!fs::exists(home_dir)) {
                fs::create_directory(home_dir);
            }

            if (!fs::exists(home_config)) {
                write_sample_config(home_config);
            }

            config_file_path = home_config;
            parse_config();
        }


        Config::~Config() {

        }

        void Config::find_config_file() {

        }

        void Config::parse_config() {
            fs::ifstream config_file(config_file_path);
            std::string line;
            while (config_file >> line) {
                auto data = utils::split(line, '=');
                options[data[0]] = Value(data[1]);
            }
        }

        cv::Size Config::Value::as_cvsize() {
            auto data = utils::split(value, 'x');
            return cv::Size(std::stoi(data[0]), std::stoi(data[1]));
        }

        std::string Config::Value::as_string() {
            return value;
        }

        double Config::Value::as_percent() {
            auto data = utils::split(value, '%');
            return std::stoi(data[0]) / 100.0;
        }

        Config::Value Config::operator[](std::string key) {
            return this->options[key];
        }

        Config::Value::Value(std::string value_) {
            value = value_;
        }

        void Config::write_sample_config(fs::path p) {
            fs::ofstream config_file(p);
            config_file << "LOW=320x320\n"
                        << "MID=640x640\n"
                        << "HIGH=1024x1024\n"
                        << "LOW_REL=25%\n"
                        << "MID_REL=50%\n"
                        << "HIGH_REL=120%\n"
                        << "default=";
            config_file.close();
        }

        void Config::print(std::ostream& stream) {
            for (auto pair : options) {
                stream << pair.first << " " << pair.second.as_string() << '\n';
            }
        }
    }
}