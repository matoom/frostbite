#include "timerbar.h"

#include "mainwindow.h"

TimerBar::TimerBar(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;

    timer = new QTimer;
    timer->setInterval(1000);

    connect(timer, SIGNAL(timeout()), this, SLOT(intervalEvent()));
}

bool TimerBar::isActive() {
    QReadLocker locker(&lock);
    return timer->isActive();
}

void TimerBar::setTimer(int seconds) {
    if(seconds > 0) {
        timerProgress->setValue(0);

        timerProgress->setRange(0, seconds);
        maxValue = seconds;

        if(!timer->isActive()) {
            timer->start();
        } else {
            timer->stop();
            timer->start();
        }
    }
}

void TimerBar::intervalEvent() {
    int value = timerProgress->value() + 1;
    if(value >= maxValue) {
        timerProgress->setValue(maxValue);
        timer->stop();
    }
    timerProgress->setValue(value);
}

void TimerBar::load() {
    timerProgress = new QProgressBar();
    timerProgress->setValue(100);
    timerProgress->setMaximumHeight(2);
    timerProgress->setTextVisible(false);
}

void TimerBar::add() {
    mainWindow->addWidgetMainLayout(timerProgress);
}

TimerBar::~TimerBar() {
    delete timer;
    delete timerProgress;
}
