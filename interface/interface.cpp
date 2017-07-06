//
// Created by nikita on 06.07.17.
//

#include "interface.h"
#include "io.h"
#include "config.h"
#include "core.h"

namespace interface {
    void process_image(io::Input in, io::Output out) {
        Config &config = Singleton<Config>::Instance();
        core::MatWrp matrix(in.read_image());

    }

}