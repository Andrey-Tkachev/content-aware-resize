//
// Created by nikita on 23.08.17.
//
#include <string>
#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

namespace fs = boost::filesystem;
namespace xinar {
    namespace io {
        class Config {

        public:
            struct Value {
                std::string value;

                cv::Size as_cvsize();

                std::string as_string();

                double as_percent();

                Value() {};

                Value(std::string value_);
            };

        private:
            fs::path config_file_path;
            std::map<std::string, Value> options;

            void find_config_file();

            void write_sample_config(fs::path p);

            void parse_config();

        public:
            Config();

            ~Config();

            Value operator[](std::string key);
            void print(std::ostream& stream);
        };
    }
}