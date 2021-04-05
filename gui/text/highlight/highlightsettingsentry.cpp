#include "highlightsettingsentry.h"

HighlightSettingsEntry::HighlightSettingsEntry() {
}

HighlightSettingsEntry::HighlightSettingsEntry(const int &id, const QString &value,
    const QString &group, const QColor &color, const QColor &bgColor, const bool &alert, const QString &alertValue,
    const bool &timer, const int &timerValue, const QString &timerAction, const bool command, const QString &commandValue, const QBitArray &options) {

    this->id = id;
    this->value = value;
    this->group = group;
    this->color = color;
    this->bgColor = bgColor;
    this->timer = timer;
    this->timerValue = timerValue;
    this->command = command;
    this->commandValue = commandValue;
    this->alert = alert;
    this->alertValue = alertValue;
    this->timerAction = timerAction;
    this->options = options;
}

const QString HighlightSettingsEntry::toString() {
    return "HighlightSettingsEntry:[ id => " + QString::number(this->id) +
            ", value => " + this->value +
            ", group => " + this->group.toLocal8Bit() + " ]";
}
