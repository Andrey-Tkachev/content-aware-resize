#include "core.h"
#include <opencv2/opencv.hpp>

#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE CORE_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MatWrapperTest)

BOOST_AUTO_TEST_CASE(Constructor) {
        cv::Mat origin(3, 5, CV_64FC1);
        core::MatWrp wrap(origin);
        BOOST_CHECK_EQUAL(wrap.width(), 5);
        BOOST_CHECK_EQUAL(wrap.hieght(), 3);
}

BOOST_AUTO_TEST_SUITE_END()