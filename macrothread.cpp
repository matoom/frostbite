#include "macrothread.h"

MacroThread::MacroThread() {
}

void MacroThread::init(QHash<QString, QStringList> macro, int time) {
    this->macro = macro;
    this->sequenceTime = time;

    if(sequenceTime <= 0) {
        sequenceTime = 1000;
    }
}

void MacroThread::run() {
    for(int i = 0; i < macro["commands"].size(); i++) {
        if(i < macro["actions"].size()) {
            if(macro["actions"].at(i) == "s") {
                emit writeCommand(macro["commands"].at(i));
                QCoreApplication::processEvents();
                msleep(sequenceTime);
            } else if(macro["actions"].at(i) == "n") {
                emit writeCommand(macro["commands"].at(i));
            }
        } else {                           
            int cursorPos = macro["commands"].at(i).indexOf("@");
            if(cursorPos == -1) {
                emit setText(macro["commands"].at(i));
            } else {
                QString command = macro["commands"].at(i);
                command.remove(cursorPos, cursorPos);

                emit setText(command);
                emit setCursor(cursorPos);
            }
        }
    }
}
