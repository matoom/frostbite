#include "ignoresettings.h"

IgnoreSettings::IgnoreSettings() {
    this->init();
}

void IgnoreSettings::init() {
    QString path = ClientSettings::Instance()->profilePath();
    settings = new QSettings(path + "ignores.ini", QSettings::IniFormat);
}

void IgnoreSettings::setSettings(QList<AlterSettingsEntry> entries) {
    settings->remove("ignore");
    settings->beginWriteArray("ignore");

    for (int i = 0; i < entries.size(); ++i) {
        AlterSettingsEntry entry = entries.at(i);

        settings->setArrayIndex(i);
        settings->setValue("pattern", entry.pattern);
        settings->setValue("target", entry.targetList);
    }
    settings->endArray();
}

void IgnoreSettings::addParameter(AlterSettingsEntry entry) {
    int id = settings->value("ignore/size").toInt();

    settings->beginWriteArray("ignore");
    settings->setArrayIndex(id);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("target", entry.targetList);
    settings->endArray();
}

void IgnoreSettings::setParameter(AlterSettingsEntry entry) {
    int size = settings->value("ignore/size").toInt();

    settings->beginWriteArray("ignore");
    settings->setArrayIndex(entry.id);
    settings->setValue("pattern", entry.pattern);
    settings->setValue("target", entry.targetList);
    settings->endArray();

    settings->setValue("ignore/size", size);
}

QList<AlterSettingsEntry> IgnoreSettings::getIgnores() {
    int size = settings->beginReadArray("ignore");

    QList<AlterSettingsEntry> entries;
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        entries.append(AlterSettingsEntry((const int&)i,
                (const QString&)settings->value("pattern", "").toString(),
                (const QStringList&)settings->value("target", QStringList()).value<QStringList>()));
    }
    settings->endArray();
    return entries;
}

IgnoreSettings::~IgnoreSettings() {
    delete settings;
}
