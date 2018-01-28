#include "compassview.h"

CompassView::CompassView(QWidget *parent) : QWidget(((MainWindow*)parent)->getWindowFacade()->getGameWindow(), Qt::FramelessWindowHint) {
    mainWindow = (MainWindow*)parent;

    this->setFixedWidth(120);
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
    QPlainTextEdit* gameWindow = mainWindow->getWindowFacade()->getGameWindow();
    this->move(gameWindow->width() / 2, gameWindow->height() / 2);
    this->raise();
}

void CompassView::paint(Compass* comp) {
    compass = comp->paint();
    imageLabel->setPixmap(compass);
}

void CompassView::mousePressEvent(QMouseEvent* event) {
    oldPos = event->globalPos();
}

void CompassView::mouseMoveEvent(QMouseEvent* event) {
    const QPoint delta = event->globalPos() - oldPos;
    if (!locked) {
        this->move(x() + delta.x(), y() + delta.y());
        oldPos = event->globalPos();
    }
}

void CompassView::gameWindowResizeEvent(GameWindow* gameWindow) {
    if(anchored) {
        QRect rect = gameWindow->geometry();
        QPoint adjust(width() + 5, height() - 10);
        this->move(rect.bottomRight() - adjust);
    }
}

CompassView::~CompassView()  {
    delete imageLabel;
}
