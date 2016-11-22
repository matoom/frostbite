#include "highlightsettingsentry.h"

HighlightSettingsEntry::HighlightSettingsEntry() {
}

HighlightSettingsEntry::HighlightSettingsEntry(const int &id, const QString &value,
    const QString &group, const QColor &color, const bool &alert, const QString &alertValue,
    const bool &timer, const int &timerValue, const QString &timerAction, const QBitArray &options) {

    this->id = id;
    this->value = value;
    this->group = group;
    this->color = color;
    this->timer = timer;
    this->timerValue = timerValue;
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
