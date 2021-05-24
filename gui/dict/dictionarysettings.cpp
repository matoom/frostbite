#include "dictionarysettings.h"
#include "gui/clientsettings.h"

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
const char* DICTIONARY_OUTPUTTYPE_PATH = "Dictionary/OutputType";

DictionarySettings::OutputType outputTypeFromInt(int value) {
    switch (value) {
    case 1:
        return DictionarySettings::OutputType::Window;
    case 2:
        return DictionarySettings::OutputType::Tooltip;
    default:
        return DictionarySettings::OutputType::Disabled;
    }
    return DictionarySettings::OutputType::Disabled;
}

int outputTypeToInt(DictionarySettings::OutputType type) {
    switch (type) {
    case DictionarySettings::OutputType::Disabled:
        return 0;
    case DictionarySettings::OutputType::Window:
        return 1;
    case DictionarySettings::OutputType::Tooltip:
        return 2;
    }
    return 0;
}
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

DictionarySettings::OutputType DictionarySettings::getDictOutputType() {
    // fallback for the old setting
    if (clientSettings->hasValue(DICTIONARY_DBLCLICK_PATH)) {
        bool dblClickEnabled =
                clientSettings->getParameter(DICTIONARY_DBLCLICK_PATH, DEFAULT_DICT_DBLCLK_ENABLED)
                        .toBool();
        // remove the old setting and set the new style setting
        clientSettings->remove(DICTIONARY_DBLCLICK_PATH);
        OutputType outputType = dblClickEnabled ? OutputType::Window : OutputType::Disabled;
        setDictOutputType(outputType);
        return outputType;
    }
    return outputTypeFromInt(clientSettings
                                     ->getParameter(DICTIONARY_OUTPUTTYPE_PATH,
                                                    outputTypeToInt(OutputType::Disabled))
                                     .toInt());
}

Qt::KeyboardModifier DictionarySettings::getDoubleClickModifier() const {
    bool ok;
    uint modifierVal =
            clientSettings->getParameter(DICTIONARY_DBLCLICK_MOD_PATH, DEFAULT_DICT_DBLCLK_MOD)
                    .toUInt(&ok);
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

DictionarySettings& DictionarySettings::setDictCommand(const QString &cmd) {
    clientSettings->setParameter(DICTIONARY_NAME_PATH, cmd);
    return *this;
}

DictionarySettings& DictionarySettings::setDictArguments(const QString& args) {
    clientSettings->setParameter(DICTIONARY_ARGS_PATH, args);
    return *this;
}

DictionarySettings& DictionarySettings::setDoubleClickModifier(Qt::KeyboardModifier modifier) {
    clientSettings->setParameter(DICTIONARY_DBLCLICK_MOD_PATH, modifier);
    return *this;
}

DictionarySettings& DictionarySettings::setDictOutputType(DictionarySettings::OutputType type) {
    clientSettings->setParameter(DICTIONARY_OUTPUTTYPE_PATH, outputTypeToInt(type));
    return *this;
}
