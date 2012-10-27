#ifndef MACROSERVICE_H
#define MACROSERVICE_H

#include <QObject>
#include <QDebug>
#include <QStringList>

#include <commandline.h>
#include <mainwindow.h>
#include <macrothread.h>
#include <macrosettings.h>

class CommandLine;
class MainWindow;
class MacroThread;
class MacroSettings;

class MacroService : public QObject {
    Q_OBJECT

public:
    explicit MacroService(QObject *parent = 0);
    ~MacroService();
    
    bool execute(QString);
    void abortSequence();

private:
    MainWindow* mainWindow;
    MacroThread* macroThread;
    MacroSettings* macroSettings;

    QHash<QString, QStringList>  processCommands(QString);

signals:
    void init(QHash<QString, QStringList>, int);
    void exit();
    
public slots:
    void setText(QString);
    void setCursor(int);
    void writeCommand(QString);
};

#endif // MACROSERVICE_H
