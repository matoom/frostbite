#ifndef MACROSETTINGS_H
#define MACROSETTINGS_H

#include <QSettings>

#include <clientsettings.h>

class MacroSettings {

public:
    MacroSettings();
    ~MacroSettings();

    void init();
    void setParameter(QString name, QVariant value);
    QVariant getParameter(QString name, QVariant defaultValue);

    bool hasValue(QString);

private:
    ClientSettings* clientSettings;
    QSettings* settings;

signals:
    
public slots:
    
};

#endif // MACROSETTINGS_H
