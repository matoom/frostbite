#include "timerbar.h"

TimerBar::TimerBar(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;

    timer = new QTimer;
    timer->setInterval(1000);

    increment = 0;

    connect(timer, SIGNAL(timeout()), this, SLOT(intervalEvent()));
}

bool TimerBar::isActive() {
    return timer->isActive();
}

void TimerBar::setTimer(int seconds) {
    if(seconds > 0) {
        timerProgress->setValue(0);

        increment = 100 / seconds;

        if(!timer->isActive()) {
            timer->start();
        } else {
            timer->stop();
            timer->start();
        }
    }
}

void TimerBar::intervalEvent() {
    int value = timerProgress->value() + increment;
    if(value >= 100) {
        timerProgress->setValue(100);
        timer->stop();
    }
    timerProgress->setValue(value);
}

void TimerBar::loadProgressbar() {
    timerProgress = new QProgressBar();
    timerProgress->setValue(100);
    timerProgress->setMaximumHeight(2);
    timerProgress->setTextVisible(false);

    mainWindow->addWidgetMainLayout(timerProgress);
}

TimerBar::~TimerBar() {
    delete timerProgress;
}
