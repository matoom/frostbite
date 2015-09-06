#ifndef SCRIPTSERVICE_H
#define SCRIPTSERVICE_H

#include <QObject>

#include <mainwindow.h>
#include <commandline.h>
#include <script.h>
#include <dataconverterservice.h>
#include <scriptwriterthread.h>

class MainWindow;
class CommandLine;
class Script;
class DataConverterService;
class ScriptWriterThread;

class ScriptService : public QObject {
    Q_OBJECT

public:
    explicit ScriptService(QObject *parent = 0);
    ~ScriptService();

    void writeGameWindow(QByteArray);    
    void processCommand(QByteArray);
    void runScript(QString);
    void terminateScript();
    void abortScript();
    void scriptFinished();
    void scriptEnded();
    bool isScriptActive();

private:
    DataConverterService* dataConverterService;
    ScriptWriterThread* scriptWriter;
    MainWindow* mainWindow;
    CommandLine* commandLine;
    WindowManager* windowManager;
    Script* script;
    QElapsedTimer timer;
    bool terminateFlag;

public slots:
    void writeScriptText(QByteArray);
    void writeOutgoingMessage(QByteArray);
};

#endif // SCRIPTSERVICE_H
