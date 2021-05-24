#ifndef DICTIONARYSETTINGS_H
#define DICTIONARYSETTINGS_H

#include <QVariant>

class QSettings;
class ClientSettings;

class DictionarySettings {
    friend class DictionarySettingsInstance;
public:
    enum class OutputType {
        Disabled,
        Window,
        Tooltip
    };
public:
    static DictionarySettings* getInstance();
 
    ~DictionarySettings();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue) const;

    QString getDictCommand() const;
    QString getDictArguments() const;
    OutputType getDictOutputType();
    Qt::KeyboardModifier getDoubleClickModifier() const;

    DictionarySettings& setDictCommand(const QString& cmd);
    DictionarySettings& setDictArguments(const QString& args);
    DictionarySettings& setDoubleClickModifier(Qt::KeyboardModifier modifier);
    DictionarySettings& setDictOutputType(OutputType type);

private:
    explicit DictionarySettings();

    ClientSettings* clientSettings;
};


class DictionarySettingsInstance : public DictionarySettings {
};

#endif // DICTIONARYSETTINGS_H
