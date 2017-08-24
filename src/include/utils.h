#pragma once

#include <limits>
#include <algorithm>
#include <vector>
#include <string>

namespace xinar {
    namespace utils {
        /*
        using NUMBER = int;

        constexpr int NUMBER_BITS = std::numeric_limits<NUMBER>::digits;

        template<typename T>
        struct pair {
            int first;
            T *second;
        };

        template<typename It, typename F>
        void radix_sort(It begin, It end, F f) {
            using DATA_TYPE = typeof(*begin);

            size_t data_size = end - begin;

            std::vector<pair<DATA_TYPE>> data;
            data.resize(data_size);

            auto at = [begin](int x) { return (begin + x); };

            for (size_t i = 0; i != data_size; ++i) {
                data[i].first = f(*at(i));
                data[i].second = *at(i);
            }

            std::vector<pair<DATA_TYPE>> tmp(data.begin(), data.end());

            for (int shift = NUMBER_BITS; shift >= 0; --shift) {
                int j = 0;
                for (int i = 0; i != data_size; ++i) {
                    bool move = (data[i].first << shift) >= 0;
                    if (shift == 0 == !move) {
                        data[i - j] = data[i];
                    } else {
                        tmp[j++] = data[i];
                    }
                }
                std::copy(tmp.begin(), tmp.begin() + j, data.end() - j);
            }

            for (int i = 0; i != data_size; ++i) {
                *(begin + i) = data[i].second;
            }
        }


        void radix_sort(std::vector<int> &data) {
            std::vector<int> tmp(data.begin(), data.end());

            for (int shift = NUMBER_BITS; shift >= 0; --shift) {
                int j = 0;
                for (int i = 0; i != data.size(); ++i) {
                    bool move = (data[i] << shift) >= 0;
                    if (shift == 0 == !move) {
                        data[i - j] = data[i];
                    } else {
                        tmp[j++] = data[i];
                    }
                }
                std::copy(tmp.begin(), tmp.begin() + j, data.end() - j);
            }
        }
        */
        std::vector<std::string> split(const std::string &line, char delimiter);
    }
}