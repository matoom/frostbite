#include "keyboardfilter.h"
#include "commandline.h"

/* add all keys to map:key, modifier, flags and action text */
KeyboardFilter::KeyboardFilter(QObject *parent) : QObject(parent) {
    macroSettings = MacroSettings::Instance();
}

bool KeyboardFilter::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        commandLine = (CommandLine*)object;        

        QKeyEvent *keyEvent = (QKeyEvent*)event;
        if(keyEvent->modifiers().testFlag(Qt::KeypadModifier)) {
            QString cmd = macroSettings->getParameter("keypad/" +
                QString::number(keyEvent->modifiers() | keyEvent->key()), "").toString();
            return commandLine->runMacro(cmd);
        } else {
            if(keyEvent->modifiers() == Qt::NoModifier) {
                switch(keyEvent->key()) {
                    case Qt::Key_Up:
                        commandLine->historyBack();
                    break;
                    case Qt::Key_Down:
                        commandLine->historyForward();
                    break;
                    case Qt::Key_Escape:
                        commandLine->clear();
                        commandLine->historyCounter = -1;
                        commandLine->abortScript();
                        commandLine->abortSequence();
                    break;
                    case Qt::Key_Tab:
                        commandLine->completeCommand();
                        return true;
                    break;
                    default:
                        QString cmd = macroSettings->getParameter("function/" +
                            QString::number(keyEvent->modifiers() | keyEvent->key()), "").toString();
                        return commandLine->runMacro(cmd);
                    break;
                }
                return false;
            } else {
                if(keyEvent->modifiers() == Qt::ControlModifier) {
                    QString cmd = macroSettings->getParameter("ctrl/" +
                        QString::number(keyEvent->modifiers() | keyEvent->key()), "").toString();
                    return commandLine->runMacro(cmd);
                } else if (keyEvent->modifiers() == Qt::AltModifier) {
                    QString cmd = macroSettings->getParameter("alt/" +
                        QString::number(keyEvent->modifiers() | keyEvent->key()), "").toString();
                    return commandLine->runMacro(cmd);
                }
            }
        }
        return false;
    }
    return false;
}
