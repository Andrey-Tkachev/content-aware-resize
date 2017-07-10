#include "core.h"
#include <opencv2/opencv.hpp>
#include <climits>

namespace core {
    MatWrp::MatWrp() {
        this->mat = cv::Mat();
        this->transposed = false;
    }

    MatWrp::MatWrp(const MatWrp& other) {
        this->mat = other.mat;
        this->transposed = other.transposed;
    }

    MatWrp::MatWrp(const cv::Mat& other) {
        this->mat = other;
        this->transposed = false;
    }

    MatWrp::MatWrp(int h, int w, int type=CV_64F) {
        this->mat.create(h, w, type);
        this->transposed = false;
    }

    MatWrp
    MatWrp::clone() const {
        MatWrp cl((this->mat).clone());
        cl.transposed = this->transposed;
        return cl;
    }
    const
    int MatWrp::width() const {
        return ((this->transposed) ? this->mat.rows :
                this->mat.cols);
    }

    const
    int MatWrp::height() const {
        return ((this->transposed) ? this->mat.cols :
                this->mat.rows);
    }

    bool
    MatWrp::is_transposed() const {
        return transposed;
    }

    void
    MatWrp::transpose() {
        this->transposed ^= 1;
    }

    void
    MatWrp::set_shape(const MatWrp& other) {
        this->mat.create(other.mat.rows, other.mat.cols, CV_64F);
        this->transposed = other.transposed;
    }

    void
    MatWrp::set_shape(const MatWrp& other, int type) {
        this->mat.create(other.mat.rows, other.mat.cols, type);
        this->transposed = other.transposed;
    }

    void
    MatWrp::set_orientation(const MatWrp& other) {
        this->transposed = other.transposed;
    }

    MatWrp
    MatWrp::operator() (cv::Range rowRange, cv::Range colRange)  const {
        if (this->transposed) {
            std::swap(rowRange, colRange);
        }
        MatWrp copy((this->mat)(rowRange, colRange));
        if (this->transposed) {
            copy.transpose();
        }
        return copy;
    }

    MatWrp&
    MatWrp::operator= (const MatWrp& other) {
        this->mat = other.mat;
        this->transposed = other.transposed;
        return *this;
    }

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

    WeightData get_w(MatWrp& optimum_energy, MatWrp& seem_energy, int i, int j, int row) {
        return seem_energy.at<WeightData>(row, i) * optimum_energy.at<WeightData>(row + 1, j);
    }

    bool seem_comparator(std::pair<WeightData, Seem>& it1, std::pair<WeightData, Seem>& it2) {
        return it1.first < it2.first;
    }

    std::vector<Seem>
    get_seems(MatWrp& energy, int k) {
        MatWrp seem_energy;
        seem_energy.set_shape(energy);

        MatWrp optimum_energy;
        optimum_energy.set_shape(energy);
        calc_optimum_dynamics(energy, optimum_energy);

        MatWrp prev;
        prev.set_shape(energy);
        for (int i = 0; i < energy.width(); ++i) {
            prev.at<long long>(0, i) = -1;
            seem_energy.at<WeightData>(0, i) = energy.at<EnergyData>(0, i);
        }

        for (int row = 0; row < energy.height() - 1; ++row) {
            std::vector<WeightData> matches(energy.width());
            matches[0] = get_w(optimum_energy, seem_energy, 0, 0, row);
            matches[1] = std::max(matches[0] + get_w(optimum_energy, seem_energy, 1, 1, row),
                                  get_w(optimum_energy, seem_energy, 0, 1, row) +
                                  get_w(optimum_energy, seem_energy, 1, 0, row));
            for (int col = 2; col < energy.width(); ++col) {
                WeightData w1 = matches[col - 1] + get_w(optimum_energy, seem_energy, col, col, row);
                WeightData w2 = matches[col - 2] + get_w(optimum_energy, seem_energy, col, col - 1, row) +
                                                 + get_w(optimum_energy, seem_energy, col - 1, col, row);
                matches[col] = std::max(w1, w2);
            }
            int x = energy.width() - 1;
            while (x >= 0) {
                WeightData last_match = (x == 0 ? 0 : matches[x - 1]);
                if (matches[x] == last_match + get_w(optimum_energy, seem_energy, x, x, row)) {
                    prev.at<long long> (row + 1, x) = x;
                    seem_energy.at<WeightData>(row + 1, x) = seem_energy.at<WeightData>(row, x) +
                                                             energy.at<EnergyData>(row + 1, x);
                    --x;
                } else {
                    prev.at<long long> (row + 1, x) = x - 1;
                    prev.at<long long> (row + 1, x - 1) = x;
                    seem_energy.at<WeightData>(row + 1, x - 1) = seem_energy.at<WeightData>(row, x) +
                                                                 energy.at<EnergyData>(row + 1, x - 1);
                    seem_energy.at<WeightData>(row + 1, x) = seem_energy.at<WeightData>(row, x - 1) +
                                                                 energy.at<EnergyData>(row + 1, x);
                    x -= 2;
                }
            }
        }

        std::vector<Seem> seems(energy.width());
        for (int i = 0; i < energy.width(); ++i) {
            int curr = i;
            int curr_row = energy.height() - 1;
            while (curr != -1) {
                seems[i].emplace_back(curr, curr_row);
                curr = prev.at<long long> (curr_row, curr);
                --curr_row;
            }
        }
        std::vector<std::pair<WeightData, Seem>> weighted_seems;
        for (int i = 0; i != seems.size(); ++i) {
            weighted_seems.push_back(std::make_pair(seem_energy.at<WeightData>(energy.height() - 1, i),
                                                    seems[i]));
        }
        std::sort(weighted_seems.begin(), weighted_seems.end(), seem_comparator);
        std::vector<Seem> res;
        for (int i = 0; i < k; ++i) {
            res.push_back(weighted_seems[i].second);
        }

        return res;
    }

    bool point_comparator(cv::Point2i& p1, cv::Point2i& p2) {
        return p1.x < p2.x;
    }

    void add_seems(MatWrp& from, std::vector<Seem>& seems) {
        int delta = seems.size();
        MatWrp out(from.mat.rows + (from.is_transposed() ? delta : 0),
                   from.mat.cols + (from.is_transposed() ? 0 : delta), from.mat.type());
        out.set_orientation(from);
        for (int row = from.height() - 1; row >= 0; --row) {
            Seem pool;
            for (auto seem : seems) {
                pool.push_back(seem[from.height() - row - 1]);
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

    void remove_seems(MatWrp& from, std::vector<Seem>& seems) {
        for (int row = from.height() - 1; row >= 0; --row) {
            Seem pool;
            for (auto seem : seems) {
                pool.push_back(seem[from.height() - row - 1]);
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
        from = from(cv::Range(0, from.height()), cv::Range(0, from.width() - seems.size())).clone();
    }
}