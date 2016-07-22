#include "roundtimedisplay.h"

RoundTimeDisplay::RoundTimeDisplay(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    gameDataContainer = GameDataContainer::Instance();

    timer = new QTimer;
    timer->setInterval(1000);

    time = 0;

    settings = new GeneralSettings();
    color = this->getColor();

    connect(timer, SIGNAL(timeout()), this, SLOT(intervalEvent()));
    connect(mainWindow, SIGNAL(profileChanged()), this, SLOT(reloadSettings()));
}

void RoundTimeDisplay::reloadSettings() {
    settings->init();
    color = this->getColor();
}

void RoundTimeDisplay::setTimer(int seconds) {
    time = seconds;
    gameDataContainer->setRt(time);

    if(!timer->isActive()) {
        timer->start();
    } else {
        timer->stop();
        timer->start();
    }

    paint(time);
}

void RoundTimeDisplay::intervalEvent() {
    gameDataContainer->setRt(--time);
    if(time < 1) timer->stop();
    paint(time);
}

void RoundTimeDisplay::repaint() {
    paint(time);
}

void RoundTimeDisplay::paint(int seconds) {
    if(seconds < 1) {
        mainWindow->getCommandLine()->clearRt();
        return;
    }
    mainWindow->getCommandLine()->insertRtIndicator(segmentDisplay(seconds), numericDisplay(seconds));
}

QPixmap RoundTimeDisplay::segmentDisplay(int seconds) {
    QPixmap collage(30 * seconds, 10);
    collage.fill(Qt::transparent);

    QPainter painter(&collage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.setBrush(color);
    painter.setPen(color);

    int x = 5;
    for(int i = 0; i < seconds; i++) {
        painter.drawRect(QRect(x, 4, 25, 3));
        x += 30;
    }
    return collage;
}

QPixmap RoundTimeDisplay::numericDisplay(int seconds) {
    QPixmap collage(40, 40);
    collage.fill(Qt::transparent);

    QPainter painter(&collage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.setBrush(color);
    painter.setPen(color);

    painter.setFont(QFont("Fixedsys", 12));
    painter.drawText(QRect(0, 0, 40, 40), Qt::AlignCenter, QString::number(seconds));

    return collage;
}

QColor RoundTimeDisplay::getColor() {
    QColor color = settings->gameWindowBackground();
    if(color == Qt::white) {
        return QColor(159, 50, 50);
    } else {
        return QColor(color.rgba()^0xffffff);
    }
}

RoundTimeDisplay::~RoundTimeDisplay() {
    delete timer;
}
