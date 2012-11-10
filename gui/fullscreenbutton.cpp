#include "fullscreenbutton.h"

FullscreenButton::FullscreenButton(QWidget *parent) : QToolButton(parent) {
    mainWindow = (MainWindow*)parent;

    this->init();

    connect(this, SIGNAL(clicked()), this, SLOT(clicked()));
}

void FullscreenButton::init() {    
    setStyleSheet("QToolButton {background: none;border: none;}");
    setIconSize(QSize(28, 28));
    setObjectName("Fullscreen");
    setCursor(Qt::PointingHandCursor);

    setButtonMode(mainWindow->isFullScreen());
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
