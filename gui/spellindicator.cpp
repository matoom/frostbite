#include "spellindicator.h"

SpellIndicator::SpellIndicator(QObject *parent) : QObject(parent) {

}

QLabel* SpellIndicator::wieldImageLabel(const char* img) {
    imageLabel = new QLabel;
    imageLabel->setToolTip("None");
    imageLabel->setObjectName("image");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("QLabel { border: 1px solid rgb(190, 190, 190);"
                             "background: #383533;"
                             "background-image: url(:/images/frame_bg.png);"
                             "padding-right: 5px;"
                             "padding-left: 5px;"
                             "border-radius: 5px}");

    imageLabel->setPixmap(QPixmap::fromImage(QImage(img)));

    return imageLabel;
}

QWidget* SpellIndicator::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(0, 10, 20, 10);

    hLayout->addWidget(this->wieldImageLabel(BOOK_ICO));
    widget->setLayout(hLayout);

    return widget;
}
