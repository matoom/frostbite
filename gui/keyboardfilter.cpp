#include "keyboardfilter.h"
#include "commandline.h"

/* add all keys to map:key, modifier, flags and action text */
KeyboardFilter::KeyboardFilter(QObject *parent) : QObject(parent) {
    macroSettings = new MacroSettings();
}

void KeyboardFilter::reloadSettings() {
    delete macroSettings;
    macroSettings = new MacroSettings();
}

bool KeyboardFilter::eventFilter(QObject *object, QEvent *event) {
    commandLine = (CommandLine*)object;

    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = (QKeyEvent*)event;

        if(keyEvent->modifiers().testFlag(Qt::KeypadModifier)) {
            #ifdef Q_OS_MAC
            switch(keyEvent->key()) {
                case Qt::Key_Up:
                    commandLine->historyBack();
                break;
                case Qt::Key_Down:
                    commandLine->historyForward();
                break;
            }
            #endif
            QString cmd = macroSettings->getParameter("keypad/" +
                QString::number(keyEvent->modifiers() | keyEvent->key()), "").toString();
            return commandLine->runMacro(cmd);
        } else {
            if(keyEvent->modifiers() == Qt::NoModifier) {
                switch(keyEvent->key()) {
                    #ifndef Q_OS_MAC
                    case Qt::Key_Up:
                        commandLine->historyBack();
                    break;
                    case Qt::Key_Down:
                        commandLine->historyForward();
                    break;
                    #endif
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
                    if(cmd.isEmpty() && keyEvent->matches(QKeySequence::Copy)) {
                        commandLine->doCopy();
                        return true;
                    } else {
                        return commandLine->runMacro(cmd);
                    }
                } else if (keyEvent->modifiers() == Qt::AltModifier) {
                    QString cmd = macroSettings->getParameter("alt/" +
                        QString::number(keyEvent->modifiers() | keyEvent->key()), "").toString();
                    return commandLine->runMacro(cmd);
                }
            }
        }
        return false;
    }

    /* workaround to give back focus to
     * command line from alt -> menu */
    if (event->type() == QEvent::FocusOut) {
        commandLine->setFocus();
        return true;
    }

    return false;
}

KeyboardFilter::~KeyboardFilter() {
    delete macroSettings;
}
