#pragma once

#include <opencv2/opencv.hpp>
#include <vector>
#include "matrixwrapper.h"
#include "types.h"

namespace xinar {
    namespace core {
        Seams get_seams(const MatWrp &energy);

        Seams get_seams(const MatWrp &energy, int k);

        void process_seams(MatWrp &image, Seams &v_seams, bool delete_mode);
    }
}