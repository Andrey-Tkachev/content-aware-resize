#include "core.h"
#include <opencv2/opencv.hpp>
#include "iostream"

#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE CORE_TEST

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(MatWrapperTest)

BOOST_AUTO_TEST_CASE(Constructor) {
        cv::Mat origin(3, 5, CV_64FC1);
        core::MatWrp wrap(origin);
        BOOST_CHECK_EQUAL(wrap.width(), 5);
        BOOST_CHECK_EQUAL(wrap.hieght(), 3);
        BOOST_CHECK_EQUAL(wrap.mat.type(), CV_64FC1);
        core::MatWrp copy_wrap(wrap);
        BOOST_CHECK_EQUAL(copy_wrap.width(), 5);
        BOOST_CHECK_EQUAL(copy_wrap.hieght(), 3);
        BOOST_CHECK_EQUAL(copy_wrap.mat.type(), CV_64FC1);
        core::MatWrp empty(3, 5, CV_64FC1);
        BOOST_CHECK_EQUAL(empty.width(), 5);
        BOOST_CHECK_EQUAL(empty.hieght(), 3);
        BOOST_CHECK_EQUAL(empty.mat.type(), CV_64FC1);

        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                origin.at<double>(i, j) = 10 * i + j;
            }
        }
        core::MatWrp wrp(origin);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                BOOST_CHECK_EQUAL(origin.at<double>(i, j), wrp.at<double>(i, j));
            }
        }
    }

BOOST_AUTO_TEST_CASE(Transpose) {
        cv::Mat origin(3, 5, CV_64FC1);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                 origin.at<double>(i, j) = 10 * i + j;
            }
        }
        core::MatWrp wrp(origin);
        wrp.transpose();
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                BOOST_CHECK_EQUAL(origin.at<double>(i, j), wrp.at<double>(j, i));
            }
        }
        wrp.transpose();
        BOOST_CHECK_EQUAL(wrp.width(), origin.cols);
        wrp.transpose();
        BOOST_CHECK_EQUAL(wrp.width(), origin.rows);
        core::MatWrp copy(wrp);
        BOOST_CHECK_EQUAL(copy.width(), origin.rows);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                BOOST_CHECK_EQUAL(origin.at<double>(i, j), copy.at<double>(j, i));
            }
        }
}

BOOST_AUTO_TEST_CASE(Shape) {
        cv::Mat origin(3, 5, CV_64FC1);
            core::MatWrp copy(origin);
            copy.transpose();
        core::MatWrp empty;
        empty.set_shape(copy);
        BOOST_CHECK_EQUAL(copy.width(), origin.rows);
        BOOST_CHECK_EQUAL(copy.hieght(), origin.cols);
        BOOST_CHECK_EQUAL(copy.mat.cols, origin.cols);
        BOOST_CHECK_EQUAL(copy.mat.rows, origin.rows);
}

BOOST_AUTO_TEST_CASE(Range) {
        cv::Mat origin(3, 5, CV_64FC1);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 5; ++j) {
                origin.at<double>(i, j) = 10 * i + j;
            }
        }
        core::MatWrp wrp(origin);
        wrp.transpose();
        cv::Mat orig_slice = origin(cv::Range(0, 1), cv::Range(2, 3));
        core::MatWrp wrp_slice = wrp(cv::Range(2, 3), cv::Range(0, 1));
        BOOST_CHECK_EQUAL(wrp_slice.mat.cols, orig_slice.cols);
        BOOST_CHECK_EQUAL(wrp_slice.mat.rows, orig_slice.rows);
        BOOST_CHECK_EQUAL(wrp_slice.width(), orig_slice.rows);
        BOOST_CHECK_EQUAL(wrp_slice.hieght(), orig_slice.cols);
        for (int i = 0; i < wrp_slice.hieght(); ++i) {
            for (int j = 0; j < wrp_slice.width(); ++j) {
                BOOST_CHECK_EQUAL(orig_slice.at<double>(i, j), wrp_slice.at<double>(j, i));
            }
        }
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(RemoveTest)
    BOOST_AUTO_TEST_CASE(RemoveRow) {
        cv::Mat origin(3, 4, CV_64FC1);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                origin.at<cv::Vec3b>(i, j) = cv::Vec3b((uchar)(10 * i + j), 0, 0);
            }
        }
        core::MatWrp wrp(origin);
        core::PVec dots = {cv::Point2i(0, 1),
                           cv::Point2i(1, 2),
                           cv::Point2i(2, 0),
                           cv::Point2i(3, 1)};
        core::remove_row(dots, wrp);

        BOOST_CHECK_EQUAL(wrp.width(), 4);
        BOOST_CHECK_EQUAL(wrp.hieght(), 2);
        BOOST_CHECK_EQUAL(wrp.width(), wrp.mat.cols);
        BOOST_CHECK_EQUAL(wrp.hieght(), wrp.mat.rows);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(1, 0).val[0], 20);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(1, 1).val[0], 11);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(0, 2).val[0], 12);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(1, 3).val[0], 23);
    }

    BOOST_AUTO_TEST_CASE(RemoveColumn) {
        cv::Mat origin(3, 4, CV_64FC1);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 4; ++j) {
                origin.at<cv::Vec3b>(i, j) = cv::Vec3b((uchar)(10 * i + j), 0, 0);
            }
        }
        core::MatWrp wrp(origin);
        wrp.transpose();
        core::PVec dots = {cv::Point2i(0, 1),
                           cv::Point2i(1, 2),
                           cv::Point2i(2, 2)};
        core::remove_row(dots, wrp);
        wrp.transpose();
        BOOST_CHECK_EQUAL(wrp.width(), 3);
        BOOST_CHECK_EQUAL(wrp.hieght(), 3);
        BOOST_CHECK_EQUAL(wrp.width(), wrp.mat.cols);
        BOOST_CHECK_EQUAL(wrp.hieght(), wrp.mat.rows);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(1, 0).val[0], 10);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(0, 1).val[0], 2);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(1, 1).val[0], 11);
        BOOST_CHECK_EQUAL(wrp.at<cv::Vec3b>(2, 2).val[0], 23);
    }
BOOST_AUTO_TEST_SUITE_END()