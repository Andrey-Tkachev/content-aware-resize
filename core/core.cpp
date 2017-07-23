#include "core.h"
#include "matrixwrapper.h"
#include <opencv2/opencv.hpp>
#include <climits>

namespace core {
    void calc_optimum_dynamics(const MatWrp& in, MatWrp& dynamics) {
        for (int i = 0; i < in.width(); ++i) {
            dynamics.at<WeightData>(in.height() - 1, i) = in.at<EnergyData>(in.height() - 1, i);
        }

        for (int curr_row = in.height() - 2; curr_row >= 0; --curr_row) {
            for (int curr_col = 0; curr_col < in.width(); ++curr_col) {
                WeightData curr_min = dynamics.at<WeightData>(curr_row + 1, curr_col) +
                                      in.at<EnergyData>(curr_row + 1, curr_col);

                for (int delta = -1; delta <= 1; delta += 2) {
                    if (delta + curr_col < in.width() && delta + curr_col >= 0) {
                        if (curr_min > dynamics.at<WeightData>(curr_row + 1, curr_col + delta) +
                                       in.at<EnergyData>(curr_row, curr_col)) {
                            curr_min = dynamics.at<WeightData>(curr_row + 1, curr_col + delta) +
                                       in.at<EnergyData>(curr_row, curr_col);
                        }
                    }
                }
                dynamics.at<WeightData>(curr_row, curr_col) = curr_min;
            }
        }
    }

    WeightData get_w(MatWrp& optimum_energy, MatWrp& seam_energy, int i, int j, int row) {
        return seam_energy.at<WeightData>(row, i) * optimum_energy.at<WeightData>(row + 1, j);
    }

    bool seam_comparator(std::pair<WeightData, seam*>& it1, std::pair<WeightData, seam*>& it2) {
        return it1.first < it2.first;
    }

    Seams
    get_seams(const MatWrp& energy) {
        MatWrp seam_energy;
        seam_energy.set_shape(energy);

        MatWrp optimum_energy;
        optimum_energy.set_shape(energy);
        calc_optimum_dynamics(energy, optimum_energy);

        std::vector<seam*> seams(energy.width());
        for (int i = 0; i < energy.width(); ++i) {
            seam_energy.at<WeightData>(0, i) = energy.at<EnergyData>(0, i);
            seams[i] = new seam();
            seams[i]->emplace_back(0, i);
        }

        for (int row = 0; row < energy.height() - 1; ++row) {
            std::vector<WeightData> matches(energy.width());
            matches[0] = get_w(optimum_energy, seam_energy, 0, 0, row);
            matches[1] = std::max(matches[0] + get_w(optimum_energy, seam_energy, 1, 1, row),
                                  get_w(optimum_energy, seam_energy, 0, 1, row) +
                                  get_w(optimum_energy, seam_energy, 1, 0, row));
            for (int col = 2; col < energy.width(); ++col) {
                WeightData w1 = matches[col - 1] + get_w(optimum_energy, seam_energy, col, col, row);
                WeightData w2 = matches[col - 2] + get_w(optimum_energy, seam_energy, col, col - 1, row) +
                                                 + get_w(optimum_energy, seam_energy, col - 1, col, row);
                matches[col] = std::max(w1, w2);
            }
            int x = energy.width() - 1;
            while (x >= 0) {
                WeightData last_match = (x == 0 ? 0 : matches[x - 1]);
                if (matches[x] == last_match + get_w(optimum_energy, seam_energy, x, x, row)) {
                    seams[x]->emplace_back(x, row + 1);
                    seam_energy.at<WeightData>(row + 1, x) = seam_energy.at<WeightData>(row, x) +
                                                             energy.at<EnergyData>(row + 1, x);
                    --x;
                } else {
                    seams[x - 1]->emplace_back(x, row + 1);
                    seams[x]->emplace_back(x - 1, row + 1);
                    std::swap(seams[x], seams[x - 1]);
                    seam_energy.at<WeightData>(row + 1, x - 1) = seam_energy.at<WeightData>(row, x) +
                                                                 energy.at<EnergyData>(row + 1, x - 1);
                    seam_energy.at<WeightData>(row + 1, x) = seam_energy.at<WeightData>(row, x - 1) +
                                                                 energy.at<EnergyData>(row + 1, x);
                    x -= 2;
                }
            }
        }

        std::vector<std::pair<WeightData, seam*>> weighted_seams;
        for (int i = 0; i != seams.size(); ++i) {
            weighted_seams.push_back(std::make_pair(seam_energy.at<WeightData>(energy.height() - 1, i),
                                                    seams[i]));
        }
        std::sort(weighted_seams.begin(), weighted_seams.end(), seam_comparator);
        Seams res;
        for (int i = 0; i < weighted_seams.size(); ++i) {
            res.emplace_back(std::move(weighted_seams[i].second));
        }

        return res;
    }

    Seams get_seams(const MatWrp& energy, int k) {
        auto seams = get_seams(energy);
        seams.resize(k);
        return seams;
    }


    bool point_comparator(cv::Point2i& p1, cv::Point2i& p2) {
        return p1.x < p2.x;
    }

    void add_seams(MatWrp& from, Seams seams) {
        int w_delta = static_cast<int> (seams.size());
        MatWrp out(from.mat.rows + (from.is_transposed() ? w_delta : 0),
                   from.mat.cols + (from.is_transposed() ? 0 : w_delta), from.mat.type());
        out.set_orientation(from);
        for (int row = 0; row < from.height(); ++row) {
            seam pool;
            for (const auto seam : seams) {
                pool.push_back((*seam)[from.height() - row - 1]);
            }
            std::sort(pool.begin(), pool.end(), point_comparator);
            int delta = 0;
            int curr_pix = 0;
            for (int col = 0; col < from.width(); ++col) {
                out.at<cv::Vec3b>(row, col + delta) = from.at<cv::Vec3b>(row, col);
                if (curr_pix < pool.size() && col == pool[curr_pix].x) {
                    ++curr_pix;
                    delta += 1;
                    out.at<cv::Vec3b>(row, col + delta) = from.at<cv::Vec3b>(row, col);
                    continue;
                }
            }
        }
        from = out;
    }

    void remove_seams(MatWrp& from, Seams seams) {
        for (int row = 0; row < from.height(); ++row) {
            seam pool;
            for (const auto seam : seams) {
                pool.push_back((*seam)[row]);
            }
            std::sort(pool.begin(), pool.end(), point_comparator);
            int delta = 0;
            int curr_pix = 0;
            for (int col = 0; col < from.width(); ++col) {
                if (curr_pix < pool.size() && col == pool[curr_pix].x) {
                    ++curr_pix;
                    delta -= 1;
                    continue;
                }
                from.at<cv::Vec3b>(row, col + delta) = from.at<cv::Vec3b>(row, col);
            }
        }
        from = from(cv::Range(0, from.height()), cv::Range(0, from.width() - seams.size())).clone();
    }
}