#include "activespellindicator.h"

ActiveSpellIndicator::ActiveSpellIndicator(QObject *parent) : QObject(parent) {    
}

QLabel* ActiveSpellIndicator::createSpellLabel() {
    spellLabel = new QLabel;
    spellLabel->setToolTip("None");
    spellLabel->setObjectName("activeSpells");
    spellLabel->setAlignment(Qt::AlignCenter);
    spellLabel->setStyleSheet("QLabel {border: 1px solid rgb(190, 190, 190);"
                             "background: #383533;"
                             "padding-right: 5px;"
                             "padding-left: 5px;"
                             "color: #E0E0E0;"
                             "min-width: 23px;"
                             "border-radius: 5px; }"
                             "QToolTip {color: #F8F8F8;"
                             "background-color: #383533;"
                             "border: 2px outset #2a82da;}");

    QFont f(DEFAULT_FONT, 14, QFont::Bold);
    spellLabel->setFont(f);

    this->setText("-");
    this->setToolTip("None");

    spellLabel->setCursor(Qt::WhatsThisCursor);

    return spellLabel;
}

QWidget* ActiveSpellIndicator::create() {
    QWidget *widget = new QWidget;
    QHBoxLayout *hLayout = new QHBoxLayout(widget);
    hLayout->setContentsMargins(0, 10, 20, 10);

    hLayout->addWidget(this->createSpellLabel());
    widget->setLayout(hLayout);

    return widget;
}

void ActiveSpellIndicator::setToolTip(QString text) {
    spellLabel->setToolTip(text);
}

void ActiveSpellIndicator::setText(QString text) {
    spellLabel->setText(text);
}

ActiveSpellIndicator::~ActiveSpellIndicator() {
    delete spellLabel;
}
