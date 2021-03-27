#ifndef DICTIONARYSETTINGS_H
#define DICTIONARYSETTINGS_H

#include <QVariant>

class QSettings;
class ClientSettings;

class DictionarySettings {
    friend class DictionarySettingsInstance;

public:
    static DictionarySettings* getInstance();
 
    ~DictionarySettings();

    void reInit();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue) const;

    QString getDictCommand() const;
    QString getDictArguments() const;

    DictionarySettings& setDictCommand(const QString& cmd);
    DictionarySettings& setDictArguments(const QString& args);
    
private:
    explicit DictionarySettings();

    ClientSettings* clientSettings;
};


class DictionarySettingsInstance : public DictionarySettings {
};

#endif // DICTIONARYSETTINGS_H
