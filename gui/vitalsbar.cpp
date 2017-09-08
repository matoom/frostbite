#include "vitalsbar.h"

VitalsBar::VitalsBar(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    clientSettings = ClientSettings::getInstance();
}

void VitalsBar::add() {
    mainWindow->addWidgetMainLayout(vitalsBar);
}

void VitalsBar::addToMenu() {
    mainWindow->addWindowMenuAction(action);
}

void VitalsBar::toggle(bool checked) {
    if(vitalsBar != NULL) {
        vitalsBar->setVisible(checked);
        clientSettings->setParameter("Window/vitalsBar", checked);
    }
}

QProgressBar* VitalsBar::toolBar(const char* obName, QString bgColor) {
    QProgressBar* progressbar = new QProgressBar();
    progressbar->setObjectName(obName);
    progressbar->setValue(100);
    progressbar->setMaximum(100);
    progressbar->setMaximumHeight(10);
    progressbar->setTextVisible(true);
    progressbar->setFormat(QString(obName[0]).toUpper() + ": " + QString::number(100) + "%");
    progressbar->setStyleSheet("QProgressBar {"
                               "font-size: 10px;"
                               "color: #000000;"
                               "background-color: white;}");

    QPalette p = progressbar->palette();
    p.setColor(QPalette::Highlight, bgColor);
    progressbar->setPalette(p);

    return progressbar;
}

void VitalsBar::load() {    
    vitalsBar = new QWidget;
    vitalsBar->setStyleSheet("background-color: #f0f0f0;");

    QHBoxLayout* hLayout = new QHBoxLayout(vitalsBar);
    hLayout->setSpacing(10);
    hLayout->setMargin(4);

    health = this->toolBar("health", "#9BCA3E");
    hLayout->addWidget(this->addFrame(health));

    mana = this->toolBar("mana", "#0099CC");
    hLayout->addWidget(this->addFrame(mana));

    concentration = this->toolBar("concentration", "#66CCFF");
    hLayout->addWidget(this->addFrame(concentration));

    fatigue = this->toolBar("fatigue", "#F6D55D");
    hLayout->addWidget(this->addFrame(fatigue));

    spirit = this->toolBar("spirit", "#B58AA5");
    hLayout->addWidget(this->addFrame(spirit));

    vitalsBar->setLayout(hLayout);

    bool visible = clientSettings->getParameter("Window/vitalsBar", false).toBool();
    vitalsBar->setVisible(visible);
    this->createMenuEntry(visible);

}

QFrame* VitalsBar::addFrame(QProgressBar* bar) {
    QFrame* bgFrame = new QFrame();
    bgFrame->setStyleSheet("QFrame {"
                           "background: white;"
                           "border-radius: 2px;"
                           "}");
    QHBoxLayout* bgFLay = new QHBoxLayout(bgFrame);
    bgFLay->setContentsMargins(0, 0, 0, 0);
    bgFLay->addWidget(bar);

    return bgFrame;
}

void VitalsBar::createMenuEntry(bool checked) {
    action = new QAction("Vitals bar", mainWindow);
    action->setCheckable(true);
    action->setData("vitalsBar");
    action->setChecked(checked);
    connect(action, SIGNAL(toggled(bool)), this, SLOT(toggle(bool)));
}

void VitalsBar::updateVitals(QString name, QString value) {
    int intValue = value.toInt();
    if(name == "health") {
        QString color = "#9BCA3E";
        if(intValue < 80) color = "#FEEB51";
        if(intValue < 50) color = "#FFB92A";
        if(intValue < 30) color = "#ED5314";

        QPalette p = health->palette();
        p.setColor(QPalette::Highlight, color);
        health->setPalette(p);

        health->setValue(intValue);
        health->setFormat("H: " + QString::number(intValue) + "%");
        health->setToolTip("Health: " + value + "%");
        health->repaint();
    } else if(name == "concentration") {
        concentration->setValue(intValue);
        concentration->setFormat("C: " + QString::number(intValue) + "%");
        concentration->setToolTip("Concentration: " + value + "%");
        concentration->repaint();
    } else if(name == "stamina") {
        fatigue->setValue(intValue);
        fatigue->setFormat("F: " + QString::number(intValue) + "%");
        fatigue->setToolTip("Fatigue: " + value + "%");
        fatigue->repaint();
    } else if(name == "spirit") {
        spirit->setValue(intValue);
        spirit->setFormat("S: " + QString::number(intValue) + "%");
        spirit->setToolTip("Spirit: " + value + "%");
        spirit->repaint();
    } else if(name == "mana") {
        mana->setValue(intValue);
        mana->setFormat("M: " + QString::number(intValue) + "%");
        mana->setToolTip("Mana: " + value + "%");
        mana->repaint();
    }
}

VitalsBar::~VitalsBar() {
}
