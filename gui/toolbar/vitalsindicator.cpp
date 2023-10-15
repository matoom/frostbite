#include "vitalsindicator.h"

#include "toolbar/toolbardefines.h"
#include "defaultvalues.h"

VitalsIndicator::VitalsIndicator(QObject *parent) : QObject(parent) {
}

QProgressBar* VitalsIndicator::vitalsProgress(const char* obName, const char* img, qint8 value) {
    QProgressBar* bar = new QProgressBar;
    bar->setObjectName(obName);
    bar->setOrientation(Qt::Vertical);
    bar->setTextVisible(false);
    bar->setStyleSheet("QProgressBar::chunk {"
                       "border-image: url(" + QString(img) + ");"
                       "}"
                       "QToolTip {"
                       "color: #F8F8F8;"
                       "font: 11pt \"" TOOLBAR_FONT "\";"
                       "background-color: #383533;"
                       "border: 2px outset #2a82da;"
                       "padding: 2px; "
                       "}");
    bar->setValue(value);
    bar->setToolTip(QString(obName) + " " + QString::number(value) + "%");

    return bar;
}

QFrame* VitalsIndicator::vitalsFrame(QProgressBar* bar) {
    QFrame* bgFrame = new QFrame();
    bgFrame->setFixedWidth(T_VITALS_W);
    bgFrame->setFixedHeight(T_VITALS_H);
    bgFrame->setStyleSheet("QFrame {"
                           "padding: 2px;"
                           "border-image: url(:/images/vitals_frame.png);"
                           "}");
    QHBoxLayout* bgFLay = new QHBoxLayout(bgFrame);
    bgFLay->setContentsMargins(0, 0, 0, 0);
    bgFLay->addWidget(bar);

    return bgFrame;
}

void VitalsIndicator::setScale(float scale) {
    healthIndicator->setFixedWidth(T_VITALS_W * scale);
    healthIndicator->setFixedHeight(T_VITALS_H * scale);

    manaIndicator->setFixedWidth(T_VITALS_W * scale);
    manaIndicator->setFixedHeight(T_VITALS_H * scale);

    concentrationIndicator->setFixedWidth(T_VITALS_W * scale);
    concentrationIndicator->setFixedHeight(T_VITALS_H * scale);

    fatigueIndicator->setFixedWidth(T_VITALS_W * scale);
    fatigueIndicator->setFixedHeight(T_VITALS_H * scale);

    spiritIndicator->setFixedWidth(T_VITALS_W * scale);
    spiritIndicator->setFixedHeight(T_VITALS_H * scale);
}

QWidget* VitalsIndicator::create() {
    vitalsWidget = new QWidget;
    QHBoxLayout* hLayout = new QHBoxLayout(vitalsWidget);
    hLayout->setContentsMargins(20, 0, 20, 0);
    hLayout->setSpacing(2);

    healthBar = vitalsProgress("health", HEALTH, 100);
    healthIndicator = vitalsFrame(healthBar);
    hLayout->addWidget(healthIndicator);

    manaBar = vitalsProgress("mana", MANA_BG, 100);
    manaIndicator = vitalsFrame(manaBar);
    hLayout->addWidget(manaIndicator);

    concentrationBar = vitalsProgress("concentration", CONCENTRATION_BG, 100);
    concentrationIndicator = vitalsFrame(concentrationBar);
    hLayout->addWidget(concentrationIndicator);

    fatigueBar = vitalsProgress("fatigue", FATIGUE_BG, 100);
    fatigueIndicator = vitalsFrame(fatigueBar);
    hLayout->addWidget(fatigueIndicator);

    spiritBar = vitalsProgress("spirit", SPIRIT_BG, 100);
    spiritIndicator = vitalsFrame(spiritBar);
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
