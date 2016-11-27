#include "roundtimedisplay.h"

RoundTimeDisplay::RoundTimeDisplay(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    gameDataContainer = GameDataContainer::Instance();

    timer = new QTimer;
    timer->setInterval(1000);

    settings = new GeneralSettings();

    rtColor = this->getRtColor();
    ctColor = this->getCtColor();

    roundTime = 0;
    castTime = 0;

    connect(timer, SIGNAL(timeout()), this, SLOT(intervalEvent()));
    connect(mainWindow, SIGNAL(profileChanged()), this, SLOT(reloadSettings()));
    connect(this, SIGNAL(callPaint(int, int)), this, SLOT(paint(int, int)));
}

void RoundTimeDisplay::reloadSettings() {
    settings->init();
    rtColor = this->getRtColor();
    ctColor = this->getCtColor();
}

void RoundTimeDisplay::setTimer(int seconds) {
    roundTime = seconds;
    gameDataContainer->setRt(roundTime);

    if(!timer->isActive()) {
        timer->start();
    } else {
        timer->stop();
        timer->start();
    }
    emit callPaint(roundTime, this->castTime);
}

void RoundTimeDisplay::setCastTimer(int seconds) {
    castTime = seconds;
    gameDataContainer->setCt(castTime);

    if(!timer->isActive()) {
        timer->start();
    } else {
        timer->stop();
        timer->start();
    }
    emit callPaint(this->roundTime, castTime);
}

void RoundTimeDisplay::intervalEvent() {
    gameDataContainer->setRt(--roundTime);
    gameDataContainer->setCt(--castTime);

    if(roundTime < 1 && castTime < 1) timer->stop();

    emit callPaint(roundTime, castTime);
}

void RoundTimeDisplay::repaint() {
    emit callPaint(roundTime, castTime);
}

void RoundTimeDisplay::paint(int rt, int ct) {
    CommandLine* cmd = mainWindow->getCommandLine();
    if(rt < 1 && ct < 1) {
        cmd->clearRt();
        return;
    }
    cmd->insertRtIndicator(segmentDisplay(rt, ct), numericDisplay(rt));
}

QPixmap RoundTimeDisplay::segmentDisplay(int rt, int ct) {
    int max = qMax(rt, ct);
    int min = qMin(rt, ct);

    QPixmap collage(RT_SEGMENT_WIDTH * max, 10);
    collage.fill(Qt::transparent);

    QPainter painter(&collage);
    if(painter.isActive()) {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.setBrush(rtColor);
        painter.setPen(rtColor);

        int x = 5;
        for(int i = 0; i < min; i++) {
            painter.drawRect(QRect(x, 4, 12, 4));
            x += RT_SEGMENT_WIDTH;
        }
        painter.setBrush(ctColor);
        painter.setPen(ctColor);

        x = 19;
        for(int i = 0; i < min; i++) {
            painter.drawRect(QRect(x, 4, 11, 4));
            x += RT_SEGMENT_WIDTH;
        }

        if(rt > ct) {
            painter.setBrush(rtColor);
            painter.setPen(rtColor);
        } else {
            painter.setBrush(ctColor);
            painter.setPen(ctColor);
        }

        x = 5 + (30 * min);
        for(int i = 0; i < max - min; i++) {
            painter.drawRect(QRect(x, 4, 25, 4));
            x += RT_SEGMENT_WIDTH;
        }
    }
    return collage;
}

QPixmap RoundTimeDisplay::numericDisplay(int seconds) {
    QPixmap collage(40, 40);
    collage.fill(Qt::transparent);

    if(seconds < 1) return collage;

    QPainter painter(&collage);
    if(painter.isActive()) {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.setBrush(rtColor);
        painter.setPen(rtColor);

        painter.setFont(QFont("Fixedsys", 12));
        painter.drawText(QRect(0, 0, 40, 40), Qt::AlignCenter, QString::number(seconds));
    }
    return collage;
}

QColor RoundTimeDisplay::getRtColor() {
    return QColor("#E86850");
    /*QColor color = settings->gameWindowBackground();
    if(color == Qt::white) {
        return QColor(159, 50, 50);
    } else {
        return QColor(color.rgba()^0xffffff);
    }*/
}

QColor RoundTimeDisplay::getCtColor() {
    return QColor("#66A7C5");
    /*QColor color = settings->gameWindowBackground();
    if(color == Qt::white) {
        return QColor(51, 153, 255);
    } else {
        return QColor(color.rgba()^0xffffff);
    }*/
}

RoundTimeDisplay::~RoundTimeDisplay() {
    delete timer;
}
