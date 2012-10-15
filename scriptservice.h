#ifndef SCRIPTSERVICE_H
#define SCRIPTSERVICE_H

#include <QObject>

#include <mainwindow.h>
#include <commandline.h>
#include <script.h>
#include <expmodel.h>
#include <gamedatacontainer.h>
#include <dataconverterservice.h>

class MainWindow;
class CommandLine;
class Script;
class GameDataContainer;
class DataConverterService;


class ScriptService : public QObject {
    Q_OBJECT

public:
    explicit ScriptService(QObject *parent = 0);

    void writeOutgoingMessage(QByteArray);
    void writeGameWindow(QByteArray);
    void processCommand(QByteArray);
    void runScript(QString);
    void terminateScript();
    void abortScript();
    void scriptFinished();
    void scriptEnded();
    bool isScriptActive();

private:
    MainWindow* mainWindow;
    CommandLine* commandLine;
    Script* script;
    WindowManager* windowManager;
    ToolbarManager* toolbarManager;
    GameDataContainer* gameDataContainer;
    DataConverterService* dataConverterService;
    QElapsedTimer timer;
    bool terminateFlag;

public slots:


};

#endif // SCRIPTSERVICE_H
