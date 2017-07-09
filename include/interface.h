//
// Created by nikita on 05.07.17.
//

#ifndef CONTENTAWARERESIZE_INTERFACE_H
#define CONTENTAWARERESIZE_INTERFACE_H

#include "io.h"
//typedef Filter;

namespace interface {
    void process_image(io::Input in, io::Output out, cv::Size size, bool show_images, double quality);

    //Filter *build_filters();



}


#endif //CONTENTAWARERESIZE_INTERFACE_H
