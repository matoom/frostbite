#include "activespellindicator.h"
#include "defaultvalues.h"

ActiveSpellIndicator::ActiveSpellIndicator(QObject *parent) : QObject(parent) {    
}

QLabel* ActiveSpellIndicator::createSpellLabel() {
    spellLabel = new QLabel;
    spellLabel->setToolTip("None");
    spellLabel->setObjectName("activeSpells");
    spellLabel->setAlignment(Qt::AlignCenter);
    spellLabel->setTextFormat(Qt::RichText);
    spellLabel->setWordWrap(true);
    spellLabel->setFixedWidth(T_AS_W);
    spellLabel->setFixedHeight(T_AS_H);
    spellLabel->setStyleSheet("QLabel {"
                              "border: 1px solid rgb(190, 190, 190);"
                              "background: #383533;"
                              "padding-right: 5px;"
                              "padding-left: 5px;"
                              "color: #E0E0E0;"
                              "border-radius: 5px;"
                              "}"
                              "QToolTip {"
                              "color: #F8F8F8;"
                              "font: 11pt \"" TOOLBAR_FONT "\";"
                              "background-color: #383533;"
                              "border: 2px outset #2a82da;"
                              "padding: 2px;"
                              "}");
    setFontScale(1);

    this->setText("-");
    this->setToolTip("None");

    spellLabel->setCursor(Qt::WhatsThisCursor);

    return spellLabel;
}

void ActiveSpellIndicator::setScale(float scale) {
    spellLabel->setFixedWidth(T_AS_W * scale);
    spellLabel->setFixedHeight(T_AS_H * scale);
    setFontScale(scale);
}

void ActiveSpellIndicator::setFontScale(float scale) {
    QFont f(DEFAULT_FONT);
    f.setWeight(QFont::Bold);
    f.setPixelSize(T_AS_W * scale / 2);
    spellLabel->setFont(f);
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
    spellLabel->setToolTip("<table style='margin: 2px;'><tr><td>" + text + "</td></tr></table>");
}

void ActiveSpellIndicator::setText(QString text) {
    spellLabel->setText(text);
}

ActiveSpellIndicator::~ActiveSpellIndicator() {
    delete spellLabel;
}
