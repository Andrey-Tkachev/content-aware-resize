#include "filters.h"

namespace filter {

    /*##################################*/
    /*###GAUSSIAN BLUR IMPLEMENTATION###*/
    /*##################################*/

    GausBlur::GausBlur(cv::Size size, double X = 0, double Y = 0, int borT = cv::BORDER_DEFAULT) {
        ksize = size;
        sX = sX;
        sY = sY;
        bt = borT;
    }

    void GausBlur::operator()(cv::Mat& src, cv::Mat &dst) const {
        cv::GaussianBlur(src, dst, ksize, sX, sY, bt);
    }

    cv::Size GausBlur::getKsize() {
        return ksize;
    }

    double GausBlur::getSX() {
        return sX;
    }

    double GausBlur::getSY() {
        return sY;
    }

    int GausBlur::getBt() {
        return bt;
    }

    void GausBlur::setKsize(cv::Size other) {
        ksize = other;
    }

    void GausBlur::setSX(double other) {
        sX = other;
    }

    void GausBlur::setSY(double other) {
        sY = other;
    }

    void GausBlur::setBt(int other) {
        bt = other;
    }


    /*##################################*/
    /*###SOBEL FILTER IMPLEMENTATION####*/
    /*##################################*/


    Sobel::Sobel(int x, int y, int size = 3, double sc = 1, double del = 0, int ddepth = CV_16S,
                 int bT = cv::BORDER_DEFAULT) {
        xord = x;
        yord = y;
        ksize = size;
        scale = sc;
        delta = del;
        ddepth = CV_16S;
        bt = bT;
    }

    void Sobel::operator()(cv::Mat& src, cv::Mat &dst) const {
        cv::Mat grad_x, grad_y;
        cv::Sobel(src, grad_x, ddepth, 1, 0, ksize, scale, delta, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(grad_x, grad_x);
        cv::Sobel(src, grad_y, ddepth, 0, 1, ksize, scale, delta, cv::BORDER_DEFAULT);
        cv::convertScaleAbs(grad_y, grad_y);
        addWeighted(grad_x, 0.5, grad_y, 0.5, 0, dst);
    }

    int Sobel::getXord() {
        return xord;
    }

    int Sobel::getYord() {
        return yord;
    }

    int Sobel::getKsize() {
        return ksize;
    }

    double Sobel::getScale() {
        return scale;
    }

    double Sobel::getDelta() {
        return delta;
    }

    int Sobel::getDdepth() {
        return ddepth;
    }

    int Sobel::getBt() {
        return bt;
    }

    void Sobel::setXord(int other) {
        xord = other;
    }

    void Sobel::setYord(int other) {
        yord = other;
    }

    void Sobel::setKsize(int other) {
        ksize = other;
    }

    void Sobel::setScale(double other) {
        scale = other;
    }

    void Sobel::setDelta(double other) {
        delta = other = other;
    }

    void Sobel::setDdepth(int other) {
        ddepth = other;
    }

    void Sobel::setBt(int other) {
        bt = other;
    }

    /*##################################*/
    /*###CANNY FILTER IMPLEMENTATION####*/
    /*##################################*/

    Canny::Canny(int low_threshold, int ratio_, int kernel_size_) {
        low_threshold = low_threshold;
        ratio = ratio_;
        kernel_size = kernel_size_;
    }

    void Canny::operator()(cv::Mat& src, cv::Mat &dst) const {
        cv::Canny(src, dst, low_threshold, low_threshold * ratio, kernel_size);
    }

    int Canny::getLowThreshold() {
        return low_threshold;
    }

    int Canny::getRatio() {
        return ratio;
    }

    int Canny::getKernelSize() {
        return kernel_size;
    }

    void Canny::setLowThreshold(int other) {
        low_threshold = other;
    }

    void Canny::setRatio(int other) {
        ratio = other;
    }

    void Canny::setKernelSize(int other) {
        kernel_size = other;
    }


    /*##################################*/
    /*####BLUR FILTER IMPLEMENTATION####*/
    /*##################################*/

    Blur::Blur(int sigma) {
        this->sigma = sigma;
    }

    void Blur::operator()(cv::Mat& src, cv::Mat &dst) const {
        cv::blur(src, dst, cv::Size(sigma, sigma));
    }

    int Blur::getSigma() {
        return sigma;
    }

    void Blur::setSigma(int other) {
        sigma = other;
    }

    /*##################################*/
    /*##GRAYSCALE FILTER IMPLEMENTATION#*/
    /*##################################*/

    GrayScale::GrayScale() {}

    void GrayScale::operator()(cv::Mat& src, cv::Mat &dst) const {
        cvtColor(src, dst, CV_BGR2GRAY);
    }
}
