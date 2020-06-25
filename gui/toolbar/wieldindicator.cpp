#include "wieldindicator.h"

WieldIndicator::WieldIndicator(QObject *parent, const char* icon) : QObject(parent) {
    this->icon = icon;
}

QLabel *WieldIndicator::wieldTextLabel(const char* text) {
    textLabel = new QLabel;
    textLabel->setToolTip("Empty");
    textLabel->setObjectName("text");
    textLabel->setFixedWidth(150);
    textLabel->setFixedHeight(34);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setStyleSheet("QLabel {"
                             "color: #d7d7d7;"
                             "font: 9pt \"" TOOLBAR_FONT "\";"
                             "font-weight: normal;"
                             "background: #383533;"
                             "border: 1px solid rgb(190, 190, 190);"
                             "border-bottom-right-radius: 10px"
                             "}"
                             "QToolTip {"
                             "color: #F8F8F8;"
                             "font: 11pt \"" TOOLBAR_FONT "\";"
                             "background-color: #383533;"
                             "border: 2px outset #2a82da;"
                             "padding: 2px;"
                             "}");

    textLabel->setText(text);

    return textLabel;
}

QLabel *WieldIndicator::wieldImageLabel(const char* img) {
    imageLabel = new QLabel;
    imageLabel->setObjectName("image");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setPixmap(QPixmap::fromImage(QImage(img)));

    return imageLabel;
}

QWidget *WieldIndicator::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(0, 10, 10, 10);

    hLayout->addWidget(this->wieldImageLabel(icon));
    hLayout->addWidget(this->wieldTextLabel("Empty"));

    widget->setLayout(hLayout);

    return widget;
}

WieldIndicator::~WieldIndicator() {

}
