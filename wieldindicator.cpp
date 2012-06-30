#include "wieldindicator.h"

WieldIndicator::WieldIndicator(QObject *parent, const char* icon) : QObject(parent) {
    this->icon = icon;
}

QLabel *WieldIndicator::wieldTextLabel(const char* text) {
    textLabel = new QLabel;
    textLabel->setObjectName("text");
    textLabel->setFixedWidth(150);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet("color: #1c4b68;"
                             "font: 10pt \"MS UI Gothic\";"
                             "font-weight: normal;"
                             "border: 1px solid rgb(190, 190, 190);"
                             "margin-left: -1px;"
                             "margin-top: -1;"
                             "border-bottom-right-radius: 10px");
    textLabel->setText(text);

    return textLabel;
}

QLabel *WieldIndicator::wieldImageLabel(const char* img) {
    imageLabel = new QLabel;
    imageLabel->setObjectName("image");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("border:1px solid rgb(190, 190, 190);"
                              "margin-left: -1px;"
                              "margin-top: -1;");
    imageLabel->setPixmap(QPixmap::fromImage(QImage(img)));

    return imageLabel;
}

QWidget *WieldIndicator::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(20, 10, 20, 10);

    hLayout->addWidget(this->wieldImageLabel(icon));
    hLayout->addWidget(this->wieldTextLabel(""));

    widget->setLayout(hLayout);

    return widget;
}

WieldIndicator::~WieldIndicator() {

}
