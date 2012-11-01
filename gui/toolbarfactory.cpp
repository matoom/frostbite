#include "toolbarfactory.h"

ToolbarFactory::ToolbarFactory(QObject *parent) : QObject(parent) {

}
/*
QLabel *ToolbarFactory::holdTextLabel(const char* text) {
    QLabel *textLabel = new QLabel;
    textLabel->setStyleSheet("color: #1c4b68;font: 10pt \"MS UI Gothic\";font-weight: bold;border: 1px solid rgb(190, 190, 190); margin-left:-1px;margin-top:-1;border-bottom-right-radius: 10px");
    textLabel->setFixedWidth(150);
    textLabel->setWordWrap(true);
    textLabel->setText(text);

    return textLabel;
}

QLabel *ToolbarFactory::holdImageLabel(const char* img) {
    QLabel *imageLabel = new QLabel();
    imageLabel->setStyleSheet("border: 1px solid rgb(190, 190, 190); margin-left:-1px;margin-top:-1;");
    imageLabel->setPixmap(QPixmap::fromImage(QImage(img)));
    imageLabel->setAlignment(Qt::AlignCenter);

    return imageLabel;
}

QWidget *ToolbarFactory::createHoldBar(const char* img, const char* text) {
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(20, 10, 20, 10);

    hLayout->addWidget(this->holdImageLabel(img));
    hLayout->addWidget(this->holdTextLabel(text));

    QWidget *widget = new QWidget;
    widget->setLayout(hLayout);

    return widget;
}
*/
/*
QLabel *ToolbarFactory::playerStatusLabel(const char* oName, const char* img, bool show) {
    QImage statusImage(img);

    QLabel* statusLabel = new QLabel();
    statusLabel->setObjectName(oName);

    if(show) {
        statusLabel->setPixmap(QPixmap::fromImage(statusImage));
    }

    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setFixedWidth(32);
    statusLabel->setStyleSheet("border: 1px solid rgb(190, 190, 190); margin-left:-1px;margin-right:-1px;margin-top:-1;");

    return statusLabel;
}

QWidget *ToolbarFactory::createStatusBar() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(20, 10, 20, 10);

    hLayout->addWidget(this->playerStatusLabel("invisible", INVISIBLE_ICO, false));
    hLayout->addWidget(this->playerStatusLabel("dead", DEAD_ICO, false));
    hLayout->addWidget(this->playerStatusLabel("immobile", IMMOBILE_ICO, false));
    hLayout->addWidget(this->playerStatusLabel("hidden", HIDDEN_ICO, false));
    hLayout->addWidget(this->playerStatusLabel("condition", STUNNED_ICO, false));
    hLayout->addWidget(this->playerStatusLabel("posture", STANDING_ICO, true));

    widget->setLayout(hLayout);

    return widget;
}
*/
/*
QToolButton *ToolbarFactory::quickActionButton(const char* obName, const char* icon, const char* toolTip) {
    QToolButton *toolButton = new QToolButton();
    toolButton->setIconSize(QSize(32, 32));
    toolButton->setObjectName(obName);
    toolButton->setIcon(QIcon(icon));
    toolButton->setToolTip(QObject::tr(toolTip));
    toolButton->setStatusTip(QObject::tr(toolTip));

    return toolButton;
}

QWidget *ToolbarFactory::createQuickButtonBar() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(20, 0, 20, 0);

    hLayout->addWidget(quickActionButton("sword", SWORD_ICO, "take my sword from my pack"));
    hLayout->addWidget(quickActionButton("bow", BOW_ICO, "take my bow from my pack"));
    hLayout->addWidget(quickActionButton("shield", SHIELD_ICO, "remove my shield"));
    hLayout->addWidget(quickActionButton("bag", BAG_ICO, "look in my pack"));
    hLayout->addWidget(quickActionButton("xsword", XSWORD_ICO, "put my sword i my pack"));
    hLayout->addWidget(quickActionButton("xbow", XBOW_ICO, "put my bow in my pack"));
    hLayout->addWidget(quickActionButton("xshield", XSHIELD_ICO, "wear my shield"));

    widget->setLayout(hLayout);

    return widget;
}
*/
/*
QProgressBar *ToolbarFactory::vitalsProgress(const char* obName, const char* img, int value) {
    QProgressBar *progressBar = new QProgressBar;
    bar->setObjectName(obName);
    bar->setOrientation(Qt::Vertical);
    bar->setStyleSheet("QProgressBar::chunk { background-image: url(" + QString(img) + ");background-position: bottom 100%;background-repeat: no-repeat; }");
    bar->setTextVisible(false);
    bar->setFixedWidth(15);
    bar->setFixedHeight(30);
    bar->setValue(value);

    return progressBar;
}

QWidget* ToolbarFactory::createVitalsBar() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(20, 0, 20, 0);

    hLayout->addWidget(vitalsProgress("health", HEALTH_BG_100, 90));
    hLayout->addWidget(vitalsProgress("concentration", CONCENTRATION_BG, 30));
    hLayout->addWidget(vitalsProgress("fatigue", FATIGUE_BG, 100));
    hLayout->addWidget(vitalsProgress("spirit", SPIRIT_BG, 60));

    widget->setLayout(hLayout);

    return widget;
}
*/


