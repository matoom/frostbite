#include "dictionarysettings.h"

#include <QSettings>
#include <QGlobalStatic>
#include <clientsettings.h>
#include <defaultvalues.h>

Q_GLOBAL_STATIC(DictionarySettingsInstance, uniqueInstance)

namespace {
const char* DICTIONARY_NAME_PATH = "Dictionary/Command";
const char* DICTIONARY_ARGS_PATH = "Dictionary/Arguments";
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

DictionarySettings& DictionarySettings::setDictCommand(const QString &cmd) {
    clientSettings->setParameter(DICTIONARY_NAME_PATH, cmd);
    return *this;
}

DictionarySettings& DictionarySettings::setDictArguments(const QString& args) {
    clientSettings->setParameter(DICTIONARY_ARGS_PATH, args);
    return *this;
}

