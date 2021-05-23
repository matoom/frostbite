#include "dictionarysettings.h"

#include <QSettings>
#include <QGlobalStatic>
#include <clientsettings.h>
#include <defaultvalues.h>

Q_GLOBAL_STATIC(DictionarySettingsInstance, uniqueInstance)

namespace {
const char* DICTIONARY_NAME_PATH = "Dictionary/Command";
const char* DICTIONARY_ARGS_PATH = "Dictionary/Arguments";
const char* DICTIONARY_DBLCLICK_PATH = "Dictionary/DoubleClickEnabled";
const char* DICTIONARY_DBLCLICK_MOD_PATH = "Dictionary/DoubleClickModifier";
}


DictionarySettings* DictionarySettings::getInstance() {
    return uniqueInstance;
}

DictionarySettings::DictionarySettings() : clientSettings(ClientSettings::getInstance()) {
}

DictionarySettings::~DictionarySettings() {
}

QString DictionarySettings::getDictCommand() const {
    return clientSettings->getParameter(DICTIONARY_NAME_PATH,
                                        DEFAULT_DICT_CMD).toString();
}

QString DictionarySettings::getDictArguments() const {
    return clientSettings->getParameter(DICTIONARY_ARGS_PATH,
                                        DEFAULT_DICT_ARGS).toString();
}

bool DictionarySettings::getDoubleClickEnabled() const {
    return clientSettings->getParameter(DICTIONARY_DBLCLICK_PATH,
                                        DEFAULT_DICT_DBLCLK_ENABLED).toBool();
}

Qt::KeyboardModifier DictionarySettings::getDoubleClickModifier() const {
    bool ok;
    uint modifierVal = clientSettings->getParameter(DICTIONARY_DBLCLICK_MOD_PATH,
                                                    DEFAULT_DICT_DBLCLK_MOD).toUInt(&ok);
    // Accept only known modifiers
    Qt::KeyboardModifier modifier = DEFAULT_DICT_DBLCLK_MOD;
    if (ok) {
        switch (modifierVal) {
        case Qt::ShiftModifier:
        case Qt::ControlModifier:
        case Qt::AltModifier:
            modifier = static_cast<Qt::KeyboardModifier>(modifierVal);
            break;
        default:
            break;
        };
    }
    return modifier;
}


bool DictionarySettings::getEnableToolTip() const {
    return true;
}

DictionarySettings& DictionarySettings::setDictCommand(const QString &cmd) {
    clientSettings->setParameter(DICTIONARY_NAME_PATH, cmd);
    return *this;
}

DictionarySettings& DictionarySettings::setDictArguments(const QString& args) {
    clientSettings->setParameter(DICTIONARY_ARGS_PATH, args);
    return *this;
}

DictionarySettings& DictionarySettings::setDoubleClickEnabled(bool enabled) {
    clientSettings->setParameter(DICTIONARY_DBLCLICK_PATH, enabled);
    return *this;
}

DictionarySettings& DictionarySettings::setDoubleClickModifier(Qt::KeyboardModifier modifier) {
    clientSettings->setParameter(DICTIONARY_DBLCLICK_MOD_PATH, modifier);
    return *this;
}

DictionarySettings& DictionarySettings::setEnableToolTip(bool enabled) {
    return *this;
}
