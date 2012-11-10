#include "roundtimedisplay.h"

RoundTimeDisplay::RoundTimeDisplay(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;

    timer = new QTimer;
    timer->setInterval(1000);

    time = 0;

    connect(timer, SIGNAL(timeout()), this, SLOT(intervalEvent()));
}

void RoundTimeDisplay::setTimer(int seconds) {
    time = seconds;

    if(!timer->isActive()) {
        timer->start();
    } else {
        timer->stop();
        timer->start();
    }

    paint(time);
}

void RoundTimeDisplay::intervalEvent() {
    time--;

    if(time < 1) {
        timer->stop();
    }

    paint(time);

    //qDebug() << time;
}

void RoundTimeDisplay::repaint() {
    paint(time);
}

void RoundTimeDisplay::paint(int seconds) {
    if(seconds < 1) {
        QPalette pal = mainWindow->getCommandLine()->palette();
        pal.setBrush(QPalette::Base, Qt::white);
        mainWindow->getCommandLine()->setPalette(pal);

        return;
    }

    mainWindow->getCommandLine()->insertRtIndicator(segmentDisplay(seconds), numericDisplay(seconds));
}

QPixmap RoundTimeDisplay::segmentDisplay(int seconds) {
    QPixmap collage(30 * seconds, 10);
    collage.fill(Qt::transparent);

    QPainter painter(&collage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    QColor color(159, 50, 50);
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

    QColor color(159, 50, 50);
    painter.setBrush(color);
    painter.setPen(color);

    painter.setFont(QFont("Fixedsys", 12));
    painter.drawText(QRect(0, 0, 40, 40), Qt::AlignCenter, QString::number(seconds));

    return collage;
}

QColor RoundTimeDisplay::getColorRange(int seconds) {
    if(seconds >= 8) {
        return QColor(80, 118, 66);
    } else if(seconds < 8 && seconds >= 6) {
        return QColor(134, 148, 42);
    } else if (seconds < 6 && seconds > 3) {
        return QColor(180, 186, 34);
    } else {
        return QColor(159, 50, 50);
    }
}


RoundTimeDisplay::~RoundTimeDisplay() {
}
