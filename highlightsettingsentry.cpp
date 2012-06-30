#include "highlightsettingsentry.h"

HighlightSettingsEntry::HighlightSettingsEntry(const QString &id, const QString &value, const QString &group, const QColor &color,
                                               const QString &alert, const int &timer, const QString &timerAction, const int &options) {
    this->id = id;
    this->value = value;
    this->group = group;
    this->color = color;
    this->timer = timer;
    this->alert = alert;
    this->timerAction = timerAction;
    this->options = options;
}
