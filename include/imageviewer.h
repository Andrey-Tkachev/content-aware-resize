#pragma once

#include <QMainWindow>
#include <QImage>

#include "resizable_label.h"

class QAction;
class QMenu;

class ImageViewer : public QMainWindow {
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
