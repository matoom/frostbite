#include "compassview.h"

CompassView::CompassView(QWidget *parent) : QWidget(parent, Qt::FramelessWindowHint) {
    mainWindow = (MainWindow*)parent;

    this->setFixedWidth(110);
    this->setFixedHeight(90);

    settings = ClientSettings::getInstance();

    anchored = settings->getParameter("Compass/anchor", "bottomRight").toString() != "";
    locked = settings->getParameter("Compass/locked", true).toBool();
    visible = settings->getParameter("Compass/visible", true).toBool();

    imageLabel = new QLabel(this);
    imageLabel->setUpdatesEnabled(true);

    QHBoxLayout* layout = new QHBoxLayout;
    layout->addWidget(imageLabel);
    setLayout(layout);

    this->setGeometry(settings->getParameter("Compass/geometry", "").toRect());
    this->setCompassVisible(visible);
}

void CompassView::setCompassAnchored(bool anchored) {
    this->anchored = anchored;
}

void CompassView::setCompassLocked(bool locked) {
    this->locked = locked;
}

void CompassView::setCompassVisible(bool visible) {
    if(visible) {
        this->show();
    } else {
        this->hide();
    }
}

void CompassView::resetCompass() {
    this->move(mainWindow->width() / 2, mainWindow->height() / 2);
}

void CompassView::paint(NavigationDisplay* navigationDisplay) {
    compass = navigationDisplay->paint();
    imageLabel->setPixmap(compass);
    imageLabel->show();
}

void CompassView::mousePressEvent(QMouseEvent *evt) {
    oldPos = evt->globalPos();
}

void CompassView::mouseMoveEvent(QMouseEvent *evt) {
    const QPoint delta = evt->globalPos() - oldPos;
    if (!locked) {
        this->move(x() + delta.x(), y() + delta.y());
        oldPos = evt->globalPos();
    }
}

void CompassView::gameWindowResizeEvent(GameWindow* gameWindow) {
    if(anchored) {
        QRect rect = gameWindow->geometry();
        QPoint adjust(width() + 15, 0);
        this->move(rect.bottomRight() - adjust);
    }
}

CompassView::~CompassView()  {
}
