#include "spellindicator.h"

SpellIndicator::SpellIndicator(QObject *parent) : QObject(parent) {
}

QLabel *SpellIndicator::spellImageLabel(const char* img) {
    imageLabel = new QLabel;
    imageLabel->setObjectName("image");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("QToolTip {color: #ffffff;"
                             "background-color: #383533;"
                             "border: 2px outset #2a82da;}");
    imageLabel->setPixmap(QPixmap::fromImage(QImage(img)));

    return imageLabel;
}

QLabel* SpellIndicator::spellTextLabel() {
    textLabel = new QLabel;
    textLabel->setObjectName("text");
    textLabel->setFixedWidth(150);
    textLabel->setFixedHeight(34);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);

    textLabel->setStyleSheet("QLabel {border: 1px solid rgb(190, 190, 190);"
                             "background: #383533;"
                             "padding-right: 5px;"
                             "padding-left: 5px;"
                             "color: #E0E0E0;"
                             "border-radius: 5px; }"
                             "QToolTip {color: #ffffff;"
                             "background-color: #383533;"
                             "border: 2px outset #2a82da;}");
    this->setText("None");
    return textLabel;
}

void SpellIndicator::setToolTip(QString text) {
    imageLabel->setToolTip(text);
    textLabel->setToolTip(text);
}

void SpellIndicator::setText(QString text) {
    this->setToolTip("<table style='margin: 2px;'><tr><td>" + text + "</td></tr></table>");
    textLabel->setText(text);
}

QWidget* SpellIndicator::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(0, 10, 20, 10);

    hLayout->addWidget(this->spellImageLabel(BOOK_ICO));
    hLayout->addWidget(this->spellTextLabel());

    widget->setLayout(hLayout);    
    return widget;
}

SpellIndicator::~SpellIndicator() {
    delete textLabel;
    delete imageLabel;
}
