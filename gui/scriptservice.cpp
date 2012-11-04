#include "scriptservice.h"

ScriptService::ScriptService(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    commandLine = mainWindow->getCommandLine();
    windowManager = mainWindow->getWindowManager();
    dataConverterService = DataConverterService::Instance();

    script = new Script(this);
    terminateFlag = false;    
}

bool ScriptService::isScriptActive() {
    return script->isRunning();
}

void ScriptService::runScript(QString input) {
    QList<QString> args = input.split(" ");
    QString fileName = args.takeFirst();

    QStringList filter;
    filter << "*.rb";

    QDir myDir(QDir::currentPath() + "/scripts");
    QStringList fileList = myDir.entryList(filter, QDir::Files, QDir::Name);

    if(fileList.contains(fileName + ".rb")) {
        if(!script->isRunning()) {
            windowManager->scriptRunning(true);
            windowManager->writeGameWindow("[Executing script: " + fileName.toLocal8Bit() + ".rb, Press ESC to abort.]");
            timer.start();
            terminateFlag = false;
            script->execute(fileName, args);
        } else {
            windowManager->writeGameWindow("[Script already executing.]");
        }
    } else {
        windowManager->writeGameWindow("[Script not found.]");
    }
}

void ScriptService::terminateScript() {
    if(script->isRunning()) {
        script->killScript();
        windowManager->writeGameWindow("[Script terminated after " +
            dataConverterService->msToMMSS(timer.elapsed()).toLocal8Bit() + ".]");
        terminateFlag = false;
    }
}

void ScriptService::abortScript() {
    if(script->isRunning()) {
        if(!terminateFlag) {
            script->sendMessage("exit#\n");
            windowManager->writeGameWindow("[Script aborted after " +
                dataConverterService->msToMMSS(timer.elapsed()).toLocal8Bit() + ".]");
            terminateFlag = true;
        } else {
            this->terminateScript();
        }
    }
}

void ScriptService::scriptFinished() {
    windowManager->writeGameWindow("[Script finished, Execution time - " +
        dataConverterService->msToMMSS(timer.elapsed()).toLocal8Bit() + ".]");
    timer.invalidate();
    terminateFlag = false;
}

void ScriptService::scriptEnded() {
    windowManager->scriptRunning(false);
}

void ScriptService::writeGameWindow(QByteArray command) {
    windowManager->writeGameWindow(command);
}

void ScriptService::writeOutgoingMessage(QByteArray message) {
    if(script != NULL && script->isRunning()) {
        script->sendMessage("game_text#" + message + "\n");
    }
}

void ScriptService::processCommand(QByteArray msg) {
    QList<QByteArray> msgLines = msg.trimmed().split('\n');
    foreach (QByteArray line, msgLines) {
        if(!line.isEmpty()){
            if(line.startsWith("put#")) {
                commandLine->writeCommand(line.mid(4).trimmed());
            } else if (line.startsWith("echo#")) {
                windowManager->writeGameWindow(line.mid(5).trimmed());
            } else if (line.startsWith("end#")) {
                script->sendMessage("end#\n");
            }
        }
    }
}

ScriptService::~ScriptService() {
    delete script;
}
