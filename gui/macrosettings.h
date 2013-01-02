#ifndef MACROSETTINGS_H
#define MACROSETTINGS_H

#include <QSettings>

#include <clientsettings.h>

class MacroSettings {

public:
    static MacroSettings* Instance();

    void init();
    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    bool hasValue(QString);

private:
    MacroSettings();
    MacroSettings(MacroSettings const& copy);
    MacroSettings& operator = (MacroSettings const& copy);
    static MacroSettings* m_pInstance;
    QSettings* settings;
    
signals:
    
public slots:
    
};

#endif // MACROSETTINGS_H
