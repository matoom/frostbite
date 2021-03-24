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

DictionarySettings::DictionarySettings() {
    clientSettings = ClientSettings::getInstance();
    this->init();
}

DictionarySettings::~DictionarySettings() {    
    delete settings;
}


void DictionarySettings::init() {    
    settings = new QSettings(clientSettings->profilePath() + "dict.ini", QSettings::IniFormat);
}

void DictionarySettings::reInit() {
    delete settings;
    this->init();
}

void DictionarySettings::setParameter(QString name, QVariant value) {
    settings->setValue(name, value);
}

QVariant DictionarySettings::getParameter(QString name, QVariant defaultValue) const {
    return settings->value(name, defaultValue);
}


QString DictionarySettings::getDictCommand() const {
    return settings->value(DICTIONARY_NAME_PATH,
        DEFAULT_DICT_CMD).value<QString>();
}

QString DictionarySettings::getDictArguments() const {
    return settings->value(DICTIONARY_ARGS_PATH,
        DEFAULT_DICT_ARGS).value<QString>();
}

DictionarySettings& DictionarySettings::setDictCommand(const QString &cmd) {
    settings->setValue(DICTIONARY_NAME_PATH, cmd);
    return *this;
}

DictionarySettings& DictionarySettings::setDictArguments(const QString& args) {
    settings->setValue(DICTIONARY_ARGS_PATH, args);
    return *this;
}

