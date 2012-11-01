#ifndef MACROSETTINGS_H
#define MACROSETTINGS_H

#include <QSettings>

class MacroSettings : QSettings {
    Q_OBJECT

public:
    static MacroSettings* Instance();

    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    bool hasValue(QString);

private:
    MacroSettings();
    MacroSettings(MacroSettings const& copy);
    MacroSettings& operator = (MacroSettings const& copy);
    static MacroSettings* m_pInstance;
    
signals:
    
public slots:
    
};

#endif // MACROSETTINGS_H
