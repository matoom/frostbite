#include "vitalsindicator.h"

VitalsIndicator::VitalsIndicator(QObject *parent) : QObject(parent) {

}

QProgressBar *VitalsIndicator::vitalsProgress(const char* obName, const char* img, qint8 value) {
    QProgressBar *progressBar = new QProgressBar;
    progressBar->setObjectName(obName);
    progressBar->setOrientation(Qt::Vertical);
    progressBar->setTextVisible(false);
    progressBar->setFixedWidth(15);
    progressBar->setFixedHeight(30);
    progressBar->setStyleSheet("QProgressBar::chunk {"
                               "background-image: url(" + QString(img) + ");"
                               "background-position: bottom 100%;"
                               "background-repeat: no-repeat;"
                               "}");
    progressBar->setValue(value);
    progressBar->setToolTip(QString::number(value) + "%");

    return progressBar;
}

QWidget* VitalsIndicator::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(25, 0, 25, 0);

    healthBar = vitalsProgress("health", HEALTH_BG_100, 100);
    hLayout->addWidget(healthBar);

    /*QFrame* bgFrame = new QFrame();
    bgFrame->setFixedWidth(22);
    bgFrame->setStyleSheet("margin-bottom: 0px;"
                           "margin-top: 5px;"
                           "background-image: url(:/images/health.png);"
                           "background-repeat: no-repeat;");
    QHBoxLayout *bgFLay = new QHBoxLayout(bgFrame);
    //bgFLay->addWidget(healthBar);

    hLayout->addWidget(bgFrame);*/


    concentrationBar = vitalsProgress("concentration", CONCENTRATION_BG, 100);
    hLayout->addWidget(concentrationBar);

    fatigueBar = vitalsProgress("fatigue", FATIGUE_BG, 100);
    hLayout->addWidget(fatigueBar);

    spiritBar = vitalsProgress("spirit", SPIRIT_BG, 100);
    hLayout->addWidget(spiritBar);

    widget->setLayout(hLayout);

    return widget;
}

VitalsIndicator::~VitalsIndicator() {
    delete healthBar;
    delete concentrationBar;
    delete fatigueBar;
    delete spiritBar;
}
