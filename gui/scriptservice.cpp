#include "scriptservice.h"

ScriptService::ScriptService(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    commandLine = mainWindow->getCommandLine();
    windowManager = mainWindow->getWindowManager();
    dataConverterService = DataConverterService::Instance();
    scriptWriter = new ScriptWriterThread(this);

    script = new Script(this);
    terminateFlag = false;    

    if(!scriptWriter->isRunning()) {
        Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info("scriptwriter start..");
        scriptWriter->start();
    }
}

bool ScriptService::isScriptActive() {
    return script->isRunning();
}

void ScriptService::runScript(QString input) {
    QList<QString> args = input.split(" ");
    QString fileName = args.takeFirst();

    QStringList filter;
    filter << "*.rb";

    QDir myDir(QApplication::applicationDirPath() + "/scripts");
    QStringList fileList = myDir.entryList(filter, QDir::Files, QDir::Name);

    if(fileList.contains(fileName + ".rb")) {
        if(!script->isRunning()) {
            windowManager->scriptRunning(true);
            windowManager->writeGameWindow("[Executing script: " +
                                           fileName.toLocal8Bit() +
                                           ".rb, Press ESC to abort.]");
            timer.start();
            terminateFlag = false;
            script->execute(fileName, args);
        } else {
            windowManager->writeGameWindow("[Script " +
                                           script->currentFileName().toLocal8Bit() +
                                           ".rb already executing.]");
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

void ScriptService::writeScriptText(QByteArray text) {
    Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info("empty: " + QString(text.isEmpty() ? "true" : "false") + " active: " + QString(this->isScriptActive() ? "true" : "false") + " - " + QString(text));

    if(!text.isEmpty() && this->isScriptActive()) {
        scriptWriter->addText(text.data());

        Log4Qt::Logger::logger(QLatin1String("ErrorLogger"))->info("scriptwriter IsRunning: " + QString(scriptWriter->isRunning() ? "true" : "false"));
    }
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
                commandLine->writeCommand(line.mid(4).trimmed(), "script");
            } else if (line.startsWith("echo#")) {
                windowManager->writeGameWindow("<span class=\"echo\">" +
                    line.mid(5).trimmed() + "</span>");
            } else if (line.startsWith("end#")) {
                script->sendMessage("end#\n");
            }
        }
    }
}

ScriptService::~ScriptService() {
    delete script;
    delete scriptWriter;
}
