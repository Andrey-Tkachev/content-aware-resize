#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include <QImage>
#include <QLabel>
#include <QResizeEvent>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "core.h"
#include "filters.h"

    QImage Mat2QImage(cv::Mat& src)
{
    QImage dest((const uchar *) src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);
    dest.bits(); // enforce deep copy, see documentation
    // of QImage::QImage ( const uchar * data, int width, int height, Format format )
    return dest;
}

cv::Mat QImage2Mat(QImage const& src)
{
    cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(tmp, result,CV_BGR2RGB);
    return result;
}

class QAction;
class QMenu;
class ResizableQLabel : public QLabel
{
    public:
        cv::Mat image_cv;
    void resizeEvent(QResizeEvent* event){
        if (image_cv.cols == 0) {
            QLabel::resizeEvent(event);
            return;
        }
        const QSize ns = event->size();
        cv::Mat out;

        filter::GrayScale *gs = new filter::GrayScale();
        filter::Blur *blur = new filter::Blur(/*sigma = */ 3);
        filter::Canny *canny = new filter::Canny(/*low_threshold = */ 30, /*ratio = */ 3, /*kernel_size = */ 3);
        std::vector<filter::Filter *> filters = {gs, blur, canny};
        filter::Compose compose(filters);

        core::resize_to_fit(image_cv, out, cv::Size(ns.width(), ns.height()), compose);
        QPixmap pmp = QPixmap::fromImage(Mat2QImage(out));
        QLabel::setPixmap(pmp);
        QLabel::resizeEvent(event);
    }
    void setPixmap(const QPixmap &pixmap) {
        this->image_cv = QImage2Mat(pixmap.toImage().convertToFormat(QImage::Format_RGB888));
        QLabel::setPixmap(pixmap);
    }
};

class ImageViewer : public QMainWindow
{
    //Q_OBJECT

public:
    ImageViewer();
    bool loadFile(const QString &);

private slots:
    void open();
    void saveAs();
    void copy();
    void paste();

    void about();

private:
    void createActions();
    void createMenus();
    void updateActions();
    bool saveFile(const QString &fileName);
    void setImage(const QImage &newImage);

    QImage image;
    ResizableQLabel *imageLabel;

    QAction *saveAsAct;
    QAction *copyAct;
};

#endif