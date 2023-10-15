#include "fullscreenbutton.h"

#include "toolbar/toolbar.h"
#include "mainwindow.h"
#include "defaultvalues.h"

FullscreenButton::FullscreenButton(QWidget *parent) : QToolButton(parent) {
    mainWindow = (MainWindow*)parent;

    this->init();

    connect(this, SIGNAL(clicked()), this, SLOT(clicked()));
}

void FullscreenButton::init() {    
    setStyleSheet("QToolButton {"
                  "background: none; border: none;"
                  "}"
                  "QToolTip {"
                  "color: #F8F8F8;"
                  "font: 11pt \"" TOOLBAR_FONT "\";"
                  "background-color: #383533;"
                  "border: 2px outset #2a82da;"
                  "padding: 2px; "
                  "}");
    setIconSize(QSize(T_FS_W, T_FS_H));
    setObjectName("Fullscreen");
    setCursor(Qt::PointingHandCursor);

    setButtonMode(mainWindow->isFullScreen());
}

void FullscreenButton::setScale(float scale) {
    setIconSize(QSize(T_FS_W * scale, T_FS_H * scale));
}

void FullscreenButton::setButtonMode(bool fullScreen) {
    if(fullScreen) {
        setIcon(QIcon(":/images/fullscreen_exit.png"));
        setToolTip("Press to exit full screen.");
    } else {
        setIcon(QIcon(":/images/fullscreen.png"));
        setToolTip("Press to enter full screen.");
    }
}

void FullscreenButton::clicked() {
    setButtonMode(!mainWindow->isFullScreen());

    if(mainWindow->isFullScreen()) {
        mainWindow->toggleMaximized();
    } else {
        mainWindow->toggleFullScreen();
    }
}
