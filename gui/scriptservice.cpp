#include "scriptservice.h"

#include "mainwindow.h"
#include "commandline.h"
#include "script.h"
#include "textutils.h"
#include "scriptwriterthread.h"
#include "windowfacade.h"
#include "defaultvalues.h"
#include "clientsettings.h"
#include "scriptstreamserver.h"

ScriptService::ScriptService(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    commandLine = mainWindow->getCommandLine();
    windowFacade = mainWindow->getWindowFacade();
    scriptWriter = new ScriptWriterThread(this);

    script = new Script(this);
    terminateFlag = false;    

    if(!scriptWriter->isRunning()) {
        scriptWriter->start();
    }

    connect(this, SIGNAL(killScript()), script, SLOT(killScript()));
    connect(this, SIGNAL(sendMessage(QByteArray)), script, SLOT(sendMessage(QByteArray)));
    connect(scriptWriter, SIGNAL(writeText(QByteArray)),
            this, SLOT(writeOutgoingMessage(QByteArray)));
}

bool ScriptService::isScriptActive() {
    return script->isRunning();
}

void ScriptService::runScript(QString input) {
    TextUtils::plainToHtml(input);

    QList<QString> args = input.split(" ");
    QString fileName = args.takeFirst();

    QStringList filter;
    filter << "*.rb";

    QDir myDir(ClientSettings::getInstance()->getQStringNotBlank("Script/scriptPath", SCRIPT_PATH));
    QStringList fileList = myDir.entryList(filter, QDir::Files, QDir::Name);

    if(fileList.contains(fileName + ".rb")) {
        if(!script->isRunning()) {
            windowFacade->scriptRunning(true);
            windowFacade->writeGameWindow("[Executing script: " +
                                           fileName.toLocal8Bit() +
                                           ".rb, Press ESC to abort.]");
            timer.start();
            terminateFlag = false;
            script->execute(fileName, args);
        } else {
            windowFacade->writeGameWindow("[Script " +
                                           script->currentFileName().toLocal8Bit() +
                                           ".rb already executing.]");
        }
    } else {
        windowFacade->writeGameWindow("[Script not found.]");
    }
}

void ScriptService::terminateScript() {
    if(script->isRunning()) {
        emit killScript();
        windowFacade->writeGameWindow("[Script terminated after " +
            TextUtils::msToMMSS(timer.elapsed()).toLocal8Bit() + ".]");
        terminateFlag = false;
    }
}

void ScriptService::abortScript() {
    if(script->isRunning()) {
        if(!terminateFlag) {
            emit sendMessage("exit#\n");
            windowFacade->writeGameWindow("[Script aborted after " +
                TextUtils::msToMMSS(timer.elapsed()).toLocal8Bit() + ".]");
            terminateFlag = true;
        } else {
            this->terminateScript();
        }
    }
}

void ScriptService::scriptFinished() {
    windowFacade->writeGameWindow("[Script finished, Execution time - " +
        TextUtils::msToMMSS(timer.elapsed()).toLocal8Bit() + ".]");
    timer.invalidate();
    terminateFlag = false;
}

void ScriptService::scriptEnded() {
    windowFacade->scriptRunning(false);
}

void ScriptService::writeGameWindow(QByteArray command) {
    windowFacade->writeGameWindow(command);
}

void ScriptService::writeScriptText(QByteArray text) {
    if (!text.isEmpty()) {
        // Script Service delivers script text to both streaming
        // server and the running script
        mainWindow->getScriptStreamServer()->writeData(text);
        if (this->isScriptActive()) {
            scriptWriter->addData(text.data());
        }
    }
}

void ScriptService::writeOutgoingMessage(QByteArray message) {
    if(script != NULL && script->isRunning()) {
        emit sendMessage("game_text#" + message + "\n");
    }
}

void ScriptService::processCommand(QByteArray msg) {
    QList<QByteArray> msgLines = msg.trimmed().split('\n');
    foreach (QByteArray line, msgLines) {
        if(!line.isEmpty()){
            if(line.startsWith("put#")) {
                commandLine->writeCommand(line.mid(4).trimmed(), "script");
            } else if (line.startsWith("echo#")) {
                windowFacade->writeGameWindow("<span class=\"echo\">" + line.mid(5).trimmed() + "</span>");
            }
        }
    }
}

ScriptService::~ScriptService() {
    delete script;
    delete scriptWriter;
}
