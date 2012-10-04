#include "scriptservice.h"

ScriptService::ScriptService(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    commandLine = mainWindow->getCommandLine();
    windowManager = mainWindow->getWindowManager();
    toolbarManager = mainWindow->getToolbarManager();
    gameDataContainer = GameDataContainer::Instance();    
    dataConverterService = DataConverterService::Instance();

    script = new Script(this);
}

void ScriptService::runScript(QString fileName) {
    QStringList filter;
    filter << "*.rb";

    QDir myDir(QDir::currentPath() + "/scripts");
    QStringList fileList = myDir.entryList(filter, QDir::Files, QDir::Name);

    if(fileList.contains(fileName + ".rb")) {
        if(!script->isRunning()) {
            windowManager->writeGameWindow("Executing script: " + fileName.toLocal8Bit() + ".rb, Press ESC to abort.");
            timer.start();
            script->execute(fileName);
        } else {
            windowManager->writeGameWindow("Script already executing.");
        }
    } else {
        windowManager->writeGameWindow("Script not found.");
    }
}

void ScriptService::stopScript() {
    if(script->isRunning()) {
        script->killScript();
        windowManager->writeGameWindow("Script terminated after " +
            dataConverterService->msToMMSS(timer.elapsed()).toLocal8Bit() + " seconds.");
        timer.invalidate();
    }
}

void ScriptService::scriptFinished() {
    windowManager->writeGameWindow("Script finished executing after " +
        dataConverterService->msToMMSS(timer.elapsed()).toLocal8Bit() + " seconds.");
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
