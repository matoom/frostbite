#include "keyboardfilter.h"
#include "commandline.h"

/* add all keys to map:key, modifier, flags and action text */
KeyboardFilter::KeyboardFilter(QObject *parent) : QObject(parent) {
    macroSettings = MacroSettings::Instance();        
}

void KeyboardFilter::reloadSettings() {
    macroSettings->init();
}

bool KeyboardFilter::eventFilter(QObject *object, QEvent *event) {
    commandLine = (CommandLine*)object;

    /* workaround to give back focus to command line from alt -> menu */
    if (event->type() == QEvent::KeyRelease) {
        QKeyEvent *keyEvent = (QKeyEvent*)event;
        if(keyEvent->key() == Qt::Key_Alt) {
            commandLine->setFocus();
        }
    }

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent*)event;

        if (keyEvent->matches(QKeySequence::Copy)) {
            commandLine->doCopy();
            return true;
        } else if(keyEvent->modifiers().testFlag(Qt::KeypadModifier)) {
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
