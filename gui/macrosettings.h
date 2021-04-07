#ifndef MACROSETTINGS_H
#define MACROSETTINGS_H

#include <QSettings>

class ClientSettings;

class MacroSettings {
    friend class MacroSettingsInstance;

public:
    static MacroSettings* getInstance();
    ~MacroSettings();

    void reInit();

    void init();
    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    bool hasValue(QString);

private:
    explicit MacroSettings();
    ClientSettings* clientSettings;
    QSettings* settings;

signals:
    
public slots:
    
};

class MacroSettingsInstance : public MacroSettings {
};

#endif // MACROSETTINGS_H
