#include "utils.h"

namespace xinar_utils {
    namespace utils {
        std::vector<std::string> split(const std::string &line, char delimiter) {
            std::vector<std::string> result;
            std::string token = "";
            for (char c : line) {
                if (c != delimiter) {
                    token += c;
                } else {
                    result.push_back(token);
                    token = "";
                }
            }
            result.push_back(token);
            return result;
        }
    }
}