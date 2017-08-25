#pragma once

#include <limits>
#include <algorithm>
#include <vector>
#include <string>

namespace xinar_utils {
    namespace utils {
        std::vector<std::string> split(const std::string &line, char delimiter);
    }
}