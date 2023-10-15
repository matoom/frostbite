#include "spellindicator.h"

#include "toolbar/toolbardefines.h"
#include "defaultvalues.h"

SpellIndicator::SpellIndicator(QObject *parent) : QObject(parent) {
}

QLabel *SpellIndicator::spellImageLabel(const char* img) {
    imageLabel = new QLabel;
    imageLabel->setObjectName("image");
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setStyleSheet("QToolTip {"
                              "color: #ffffff;"
                              "font: 11pt \"" TOOLBAR_FONT "\";"
                              "background-color: #383533;"
                              "border: 2px outset #2a82da;"
                              "padding: 2px;"
                              "}");
    imageLabel->setPixmap(QPixmap::fromImage(QImage(img)));

    return imageLabel;
}

QLabel* SpellIndicator::spellTextLabel() {
    textLabel = new QLabel;
    textLabel->setObjectName("spells");
    textLabel->setFixedWidth(T_SPELL_W);
    textLabel->setFixedHeight(T_SPELL_H);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignCenter);
    textLabel->setTextFormat(Qt::RichText);

    textLabel->setStyleSheet("QLabel {"
                             "border: 1px solid rgb(190, 190, 190);"
                             "font: 9pt \"" TOOLBAR_FONT "\";"
                             "background: #383533;"
                             "padding-right: 5px;"
                             "padding-left: 5px;"
                             "color: #E0E0E0;"
                             "border-radius: 5px; "
                             "}"
                             "QToolTip {"
                             "color: #ffffff;"
                             "font: 11pt \"" TOOLBAR_FONT "\";"
                             "background-color: #383533;"
                             "border: 2px outset #2a82da;"
                             "padding: 2px;"
                             "}");
    this->setText("None");
    return textLabel;
}

void SpellIndicator::setToolTip(QString text) {
    imageLabel->setToolTip(text);
    textLabel->setToolTip(text);
}

void SpellIndicator::setText(QString text) {
    this->setToolTip(text);
    textLabel->setText(text);
}

void SpellIndicator::setScale(float scale) {
    textLabel->setFixedWidth(T_SPELL_W * scale);
    textLabel->setFixedHeight(T_SPELL_H * scale);
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
