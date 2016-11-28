#include "vitalsindicator.h"

VitalsIndicator::VitalsIndicator(QObject *parent) : QObject(parent) {

}

QProgressBar* VitalsIndicator::vitalsProgress(const char* obName, const char* img, qint8 value) {
    QProgressBar* bar = new QProgressBar;
    bar->setObjectName(obName);
    bar->setOrientation(Qt::Vertical);
    bar->setTextVisible(false);
    bar->setFixedWidth(15);
    bar->setFixedHeight(30);
    bar->setStyleSheet("QProgressBar::chunk {"
                       "background-image: url(" + QString(img) + ");"
                       "background-position: bottom 100%;"
                       "background-repeat: no-repeat;"
                       "}");
    bar->setValue(value);
    bar->setToolTip(QString::number(value) + "%");

    return bar;
}

QFrame* VitalsIndicator::vitalsFrame(QProgressBar* bar) {
    QFrame* bgFrame = new QFrame();
    bgFrame->setStyleSheet("QFrame {"
                           "margin: -7px;"
                           "background-image: url(:/images/vitals_frame.png);"
                           "background-position: center center;"
                           "background-repeat: no-repeat;}");
    QHBoxLayout* bgFLay = new QHBoxLayout(bgFrame);
    bgFLay->addWidget(bar);

    return bgFrame;
}

QWidget* VitalsIndicator::create() {
    vitalsWidget = new QWidget;
    QHBoxLayout* hLayout = new QHBoxLayout(vitalsWidget);
    hLayout->setContentsMargins(25, 0, 25, 0);

    healthBar = vitalsProgress("health", HEALTH, 100);
    QFrame* healthIndicator = vitalsFrame(healthBar);
    hLayout->addWidget(healthIndicator);

    manaBar = vitalsProgress("mana", MANA_BG, 100);
    QFrame* manaIndicator = vitalsFrame(manaBar);
    hLayout->addWidget(manaIndicator);

    concentrationBar = vitalsProgress("concentration", CONCENTRATION_BG, 100);
    QFrame* concentrationIndicator = vitalsFrame(concentrationBar);
    hLayout->addWidget(concentrationIndicator);

    fatigueBar = vitalsProgress("fatigue", FATIGUE_BG, 100);
    QFrame* fatigueIndicator = vitalsFrame(fatigueBar);
    hLayout->addWidget(fatigueIndicator);

    spiritBar = vitalsProgress("spirit", SPIRIT_BG, 100);
    QFrame* spiritIndicator = vitalsFrame(spiritBar);
    hLayout->addWidget(spiritIndicator);

    vitalsWidget->setLayout(hLayout);

    return vitalsWidget;
}

VitalsIndicator::~VitalsIndicator() {
    delete healthBar;
    delete manaBar;
    delete concentrationBar;
    delete fatigueBar;
    delete spiritBar;
    delete vitalsWidget;
}
