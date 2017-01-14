#include "roundtimedisplay.h"

RoundTimeDisplay::RoundTimeDisplay(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    gameDataContainer = GameDataContainer::Instance();

    timer = new QTimer;
    timer->setInterval(200);

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
    delete settings;
    settings = new GeneralSettings();

    rtColor = this->getRtColor();
    ctColor = this->getCtColor();
}

void RoundTimeDisplay::setTimer(int ms) {
    roundTime = ms;
    gameDataContainer->setRt(roundTime / 1000);

    if(!timer->isActive()) {
        timer->start();
    } else {
        timer->stop();
        timer->start();
    }
    emit callPaint(roundTime, this->castTime);
}

void RoundTimeDisplay::setCastTimer(int ms) {
    castTime = ms;
    gameDataContainer->setCt(castTime / 1000);

    if(!timer->isActive()) {
        timer->start();
    } else {
        timer->stop();
        timer->start();
    }
    emit callPaint(this->roundTime, castTime);
}

void RoundTimeDisplay::intervalEvent() {
    roundTime -= 200;
    castTime -= 200;

    gameDataContainer->setRt(roundTime / 1000);
    gameDataContainer->setCt(castTime / 1000);

    if(roundTime < 1000 && castTime < 1000) timer->stop();

    emit callPaint(roundTime, castTime);
}

void RoundTimeDisplay::repaint() {
    emit callPaint(roundTime, castTime);
}

void RoundTimeDisplay::paint(int rt, int ct) {
    CommandLine* cmd = mainWindow->getCommandLine();
    if(rt < 1000 && ct < 1000) {
        cmd->clearRt();
        return;
    }
    cmd->insertRtIndicator(segmentDisplay(rt, ct), numericDisplay(rt));
}

QPixmap RoundTimeDisplay::segmentDisplay(int rt, int ct) {
    rt = rt / 1000;
    ct = ct / 1000;

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

QPixmap RoundTimeDisplay::numericDisplay(int ms) {
    QPixmap collage(40, 40);
    collage.fill(Qt::transparent);

    if(ms < 1000) return collage;

    QPainter painter(&collage);
    if(painter.isActive()) {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.setBrush(rtColor);
        painter.setPen(rtColor);

        painter.setFont(settings->gameWindowFont());
        painter.drawText(QRect(0, 0, 40, 40), Qt::AlignCenter, QString::number(ms / 1000));
    }
    return collage;
}

QColor RoundTimeDisplay::getRtColor() {
    return QColor(RT_COLOR_HEX);
}

QColor RoundTimeDisplay::getCtColor() {
    return QColor(CT_COLOR_HEX);
}

RoundTimeDisplay::~RoundTimeDisplay() {
    delete timer;
}
