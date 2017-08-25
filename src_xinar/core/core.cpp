#include "core.h"
#include <memory>

namespace xinar {
    namespace core {

    #define get_w(optimum_energy, seam_energy, i, j, row)\
     ( seam_energy.at<WeightData>(row, i) * optimum_energy.at<WeightData>(row + 1, j) )

        void calc_optimum_dynamics(const MatWrp &in, MatWrp &dynamics) {
            int h = in.height(),
                    w = in.width();
            for (int i = 0; i < in.width(); ++i) {
                dynamics.at<WeightData>(h - 1, i) = in.at<EnergyData>(h - 1, i);
            }
            WeightData val_to_exam;
            for (int curr_row = in.height() - 2; curr_row >= 0; --curr_row) {
                for (int curr_col = 0; curr_col < w; ++curr_col) {
                    WeightData curr_min = dynamics.at<WeightData>(curr_row + 1, curr_col) +
                                          in.at<EnergyData>(curr_row + 1, curr_col);

                    for (int delta = -1; delta <= 1; delta += 2) {
                        if (delta + curr_col < w && delta + curr_col >= 0) {
                            val_to_exam = dynamics.at<WeightData>(curr_row + 1, curr_col + delta) +
                                          in.at<EnergyData>(curr_row, curr_col);
                            if (curr_min > val_to_exam) {
                                curr_min = val_to_exam;
                            }
                        }
                    }
                    dynamics.at<WeightData>(curr_row, curr_col) = curr_min;
                }
            }
        }

        void calc_matches(const MatWrp &energy,
                          const MatWrp &optimum_energy,
                          const MatWrp &seam_energy,
                          std::vector<WeightData> &matches,
                          int row) {
            matches[0] = get_w(optimum_energy, seam_energy, 0, 0, row);
            matches[1] = std::max(matches[0] + get_w(optimum_energy, seam_energy, 1, 1, row),
                                  get_w(optimum_energy, seam_energy, 0, 1, row) +
                                  get_w(optimum_energy, seam_energy, 1, 0, row));
            for (int col = 2; col < energy.width(); ++col) {
                WeightData w1 = matches[col - 1] + get_w(optimum_energy, seam_energy, col, col, row);
                WeightData w2 = matches[col - 2] + get_w(optimum_energy, seam_energy, col, col - 1, row) +
                                +get_w(optimum_energy, seam_energy, col - 1, col, row);
                matches[col] = std::max(w1, w2);
            }
        }

        void increase_seams(const MatWrp &energy,
                            const MatWrp &optimum_energy,
                            const std::vector<WeightData> &matches,
                            MatWrp &seam_energy,
                            Seams &seams,
                            int row) {
            int x = energy.width() - 1;
            while (x >= 0) {
                WeightData last_match = (x == 0 ? 0 : matches[x - 1]);
                if (matches[x] == last_match + get_w(optimum_energy, seam_energy, x, x, row)) {
                    seams[x]->push_back(x);
                    seam_energy.at<WeightData>(row + 1, x) = seam_energy.at<WeightData>(row, x) +
                                                             energy.at<EnergyData>(row + 1, x);
                    --x;
                } else {
                    seams[x - 1]->push_back(x);
                    seams[x]->push_back(x - 1);
                    std::swap(seams[x], seams[x - 1]);
                    seam_energy.at<WeightData>(row + 1, x - 1) = seam_energy.at<WeightData>(row, x) +
                                                                 energy.at<EnergyData>(row + 1, x - 1);
                    seam_energy.at<WeightData>(row + 1, x) = seam_energy.at<WeightData>(row, x - 1) +
                                                             energy.at<EnergyData>(row + 1, x);
                    x -= 2;
                }
            }
        }

        bool seam_comparator(const std::pair<WeightData, std::shared_ptr<seam>> &it1,
                             const std::pair<WeightData, std::shared_ptr<seam>> &it2) {
            return it1.first < it2.first;
        }

        Seams
        get_seams(const MatWrp &energy, int k) {
            MatWrp seam_energy;
            seam_energy.set_shape(energy);
            MatWrp optimum_energy;
            optimum_energy.set_shape(energy);
            calc_optimum_dynamics(energy, optimum_energy);

            Seams seams(energy.width());
            for (int i = 0; i < energy.width(); ++i) {
                seam_energy.at<WeightData>(0, i) = energy.at<EnergyData>(0, i);
                seams[i] = std::make_shared<seam>(seam());
                seams[i]->reserve(energy.height());
                seams[i]->push_back(i);
            }

            std::vector<WeightData> matches(energy.width());
            for (int row = 0; row < energy.height() - 1; ++row) {
                calc_matches(energy, optimum_energy, seam_energy, matches, row);
                increase_seams(energy, optimum_energy, matches, seam_energy, seams, row);
            }

            std::vector<std::pair<WeightData, std::shared_ptr<seam>>> weighted_seams;
            weighted_seams.reserve(seams.size());
            for (int i = 0; i != seams.size(); ++i) {
                weighted_seams.emplace_back(std::make_pair(seam_energy.at<WeightData>(energy.height() - 1, i),
                                                           seams[i]));
            }
            std::sort(weighted_seams.begin(), weighted_seams.end(), seam_comparator);
            Seams res;
            res.reserve(static_cast<size_t> (k));
            for (int i = 0; i < k; ++i) {
                res.push_back(weighted_seams[i].second);
            }
            return res;
        }

        Seams get_seams(const MatWrp &energy) {
            return get_seams(energy, energy.width());
        }

        void process_seams(MatWrp &from, Seams &seams, bool delete_mode) {
            auto w_delta = static_cast<int> (seams.size()) * (delete_mode ? -1 : 1);
            MatWrp out(from.mat.rows + (from.is_transposed() ? w_delta : 0),
                       from.mat.cols + (from.is_transposed() ? 0 : w_delta), from.mat.type());
            out.set_orientation(from);
            for (int row = 0; row < from.height(); ++row) {
                seam pool;
                pool.reserve(seams.size());
                for (const auto &seam : seams) {
                    pool.push_back((*seam)[from.height() - row - 1]);
                }
                std::sort(pool.begin(), pool.end());
                int delta = 0;
                int curr_pix = 0;
                for (int col = 0; col < from.width(); ++col) {
                    if (curr_pix < pool.size() && col == pool[curr_pix]) {
                        ++curr_pix;
                        delta += (delete_mode ? -1 : 1);
                        if (delete_mode) continue;
                        out.at<cv::Vec3b>(row, col + delta - 1) = from.at<cv::Vec3b>(row, col);
                    }
                    out.at<cv::Vec3b>(row, col + delta) = from.at<cv::Vec3b>(row, col);
                }
            }
            from = out;
        }
    }
}