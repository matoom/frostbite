#include "substitutionsettings.h"

SubstitutionSettings::SubstitutionSettings() {
    clientSettings = new ClientSettings();
    this->init();
}
void SubstitutionSettings::init() {
    settings = new QSettings(clientSettings->profilePath() + "substitutes.ini", QSettings::IniFormat);
}

void SubstitutionSettings::setSettings(QList<AlterSettingsEntry> entries) {
    settings->remove("substitution");
    settings->beginWriteArray("substitution");

    for (int i = 0; i < entries.size(); ++i) {
        AlterSettingsEntry entry = entries.at(i);

        settings->setArrayIndex(i);
        settings->setValue("enabled", entry.enabled);
        settings->setValue("pattern", entry.pattern);
        settings->setValue("substitute", entry.substitute);
        settings->setValue("target", entry.targetList);
    }
    settings->endArray();
}

void SubstitutionSettings::addParameter(AlterSettingsEntry entry) {
    int id = settings->value("substitution/size").toInt();

    settings->beginWriteArray("substitution");
    settings->setArrayIndex(id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("substitute", entry.substitute);
    settings->setValue("target", entry.targetList);
    settings->endArray();
}

void SubstitutionSettings::setParameter(AlterSettingsEntry entry) {
    int size = settings->value("substitution/size").toInt();

    settings->beginWriteArray("substitution");
    settings->setArrayIndex(entry.id);
    settings->setValue("enabled", entry.enabled);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("substitute", entry.substitute);
    settings->setValue("target", entry.targetList);
    settings->endArray();

    settings->setValue("substitution/size", size);
}

QList<AlterSettingsEntry> SubstitutionSettings::getSubstitutions() {
    int size = settings->beginReadArray("substitution");

    QList<AlterSettingsEntry> entries;
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        entries.append(AlterSettingsEntry((const int&)i,
                (const bool&)settings->value("enabled", "").toBool(),
                (const QString&)settings->value("pattern", "").toString(),
                (const QString&)settings->value("substitute", "").toString(),
                (const QStringList&)settings->value("target", QStringList()).value<QStringList>()));
    }
    settings->endArray();
    return entries;
}

SubstitutionSettings::~SubstitutionSettings() {
    delete clientSettings;
    delete settings;
}
