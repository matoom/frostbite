#include "scriptservice.h"

ScriptService::ScriptService(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    commandLine = mainWindow->getCommandLine();
    windowManager = mainWindow->getWindowManager();
    toolbarManager = mainWindow->getToolbarManager();
    gameDataContainer = GameDataContainer::Instance();    
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

void ScriptService::writeGameWindow(QByteArray command) {
    windowManager->writeGameWindow(command);
}

void ScriptService::writeOutgoingMessage(QByteArray message) {
    if(script->isRunning()) {
        script->sendMessage(message);
    }
}

void ScriptService::processCommand(QByteArray msg) {
    QList<QByteArray> msgLines = msg.trimmed().split('\n');
    foreach (QByteArray line, msgLines) {
        if(!line.isEmpty()){
            if(line.startsWith("put#")) {
                commandLine->writeCommand(line.mid(4).trimmed());
            } else if (line.startsWith("get_vitals#")) {
                script->sendMessage("vitals#" + QString::number(toolbarManager->getHealthValue()).toLocal8Bit() + "|" +
                    QString::number(toolbarManager->getConcentrationValue()).toLocal8Bit() + "|" +
                    QString::number(toolbarManager->getFatigueValue()).toLocal8Bit() + "|" +
                    QString::number(toolbarManager->getSpiritValue()).toLocal8Bit() + "\n");
            } else if (line.startsWith("get_status#")) {
                QHash<QString, bool> status = toolbarManager->getStatus();
                script->sendMessage("status#" + QString(status.value("kneeling")).toLocal8Bit() +
                    QString(status.value("prone")).toLocal8Bit() +
                    QString(status.value("sitting")).toLocal8Bit() +
                    QString(status.value("standing")).toLocal8Bit() +
                    QString(status.value("stunned")).toLocal8Bit() +
                    QString(status.value("dead")).toLocal8Bit() +
                    QString(status.value("bleeding")).toLocal8Bit() +
                    QString(status.value("hidden")).toLocal8Bit() +
                    QString(status.value("invisible")).toLocal8Bit() +
                    QString(status.value("webbed")).toLocal8Bit() +
                    QString(status.value("joined")).toLocal8Bit() + "\n");
            } else if (line.startsWith("get_exp#")) {
                ExpModel *exp = gameDataContainer->getExpField(line.mid(8).trimmed().toLower());
                if(exp != NULL) {
                    script->sendMessage("exp#" + exp->getName().toLocal8Bit() + "|" +
                        QString::number(exp->getRank()).toLocal8Bit() + "|" + exp->getRankProgression().toLocal8Bit() + "|" +
                        exp->getState().toLocal8Bit() + "|" + QString::number(exp->getNumericState()).toLocal8Bit() + "\n");
                } else {
                    script->sendMessage("exp#\n");
                }
            } else if (line.startsWith("get_room#")) {
                RoomModel* room = gameDataContainer->getRoom();
                script->sendMessage("room#" + room->getDesc().toLocal8Bit() + "|" + room->getObjs().toLocal8Bit() + "|" +
                    room->getPlayers().toLocal8Bit() + "|" + room->getExits().toLocal8Bit() + "\n");
            } else if (line.startsWith("get_wield#")) {
                WieldModel* wield = gameDataContainer->getWield();
                script->sendMessage("wield#" + wield->getLeft().toLocal8Bit() + "|" + wield->getLeftNoun().toLocal8Bit() + "|" +
                    wield->getRight().toLocal8Bit() + "|" + wield->getRightNoun().toLocal8Bit() + "\n");
            } else if (line.startsWith("echo#")) {
                windowManager->writeGameWindow(line.mid(5).trimmed());
            }
        }
    }
}
