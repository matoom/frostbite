#ifndef SCRIPTSERVICE_H
#define SCRIPTSERVICE_H

#include <QObject>

#include <mainwindow.h>
#include <commandline.h>
#include <script.h>
#include <textutils.h>
#include <scriptwriterthread.h>

class MainWindow;
class CommandLine;
class Script;
class TextUtils;
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
    TextUtils* textUtils;
    ScriptWriterThread* scriptWriter;
    MainWindow* mainWindow;
    CommandLine* commandLine;
    WindowFacade* windowFacade;
    Script* script;
    QElapsedTimer timer;
    bool terminateFlag;

public slots:
    void writeScriptText(QByteArray);
    void writeOutgoingMessage(QByteArray);
};

#endif // SCRIPTSERVICE_H
