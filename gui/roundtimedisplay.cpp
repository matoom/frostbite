#include "roundtimedisplay.h"

RoundTimeDisplay::RoundTimeDisplay(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    data = GameDataContainer::Instance();

    timer = new QTimer;
    timer->setInterval(RT_INTERVAL_MS);

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

void RoundTimeDisplay::initTimer() {
    if(!timer->isActive()) {
        timer->start();
    } else {
        timer->stop();
        timer->start();
    }
}

void RoundTimeDisplay::setTimer(int ms) {
    roundTime = ms;
    data->setRt(toSeconds(ms));

    this->initTimer();
    emit callPaint(roundTime, this->castTime);
}

void RoundTimeDisplay::setCastTimer(int ms) {
    castTime = ms;
    data->setCt(toSeconds(ms));

    this->initTimer();
    emit callPaint(this->roundTime, castTime);
}

void RoundTimeDisplay::intervalEvent() {    
    roundTime = roundTime - RT_INTERVAL_MS;
    castTime = castTime - RT_INTERVAL_MS;

    data->setRt(toSeconds(roundTime));
    data->setCt(toSeconds(castTime));

    if(roundTime < RT_INTERVAL_MS &&
            castTime < RT_INTERVAL_MS) {
        timer->stop();
    }

    emit callPaint(roundTime, castTime);
}

void RoundTimeDisplay::repaint() {
    emit callPaint(roundTime, castTime);
}

void RoundTimeDisplay::paint(int rt, int ct) {
    CommandLine* cmd = mainWindow->getCommandLine();
    if(rt < RT_INTERVAL_MS && ct < RT_INTERVAL_MS) {
        cmd->clearRt();
    } else {
        int rt_s = toSeconds(rt);
        int ct_s = toSeconds(ct);
        cmd->insertRt(segmentDisplay(rt_s, ct_s),
                      numericDisplay(rt_s));
    }
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

QPixmap RoundTimeDisplay::numericDisplay(int rt) {
    QPixmap collage(40, 40);
    collage.fill(Qt::transparent);

    if(rt < 0) return collage;

    QPainter painter(&collage);
    if(painter.isActive()) {
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

        painter.setBrush(rtColor);
        painter.setPen(rtColor);
        painter.setFont(settings->gameWindowFont());

        QString text = QString::number(rt);
        painter.drawText(QRect(0, 0, 40, 40), Qt::AlignCenter, text);
    }
    return collage;
}

int RoundTimeDisplay::toSeconds(int ms) {
    return ceil((double)ms / 1000);
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
