#include "keyboardfilter.h"
#include "commandline.h"

/* add all keys to map:key, modifier, flags and action text */
KeyboardFilter::KeyboardFilter(QObject *parent) : QObject(parent) {
}

bool KeyboardFilter::eventFilter(QObject *object, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        commandLine = (CommandLine*)object;

        QKeyEvent *keyEvent = (QKeyEvent*)event;

        if(keyEvent->modifiers() != Qt::KeypadModifier) {
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
                    commandLine->stopScript();
                break;
                default:
                    return false;
                break;
            }
        } else if (keyEvent->modifiers() == Qt::KeypadModifier) {
            switch(keyEvent->key()) {
                case Qt::Key_Home:
                    // NUMPAD 7
                    commandLine->clear();
                    commandLine->writeCommand("nw");
                    break;
                case Qt::Key_Up:
                    // NUMPAD 8
                    commandLine->clear();                    
                    commandLine->writeCommand("n");
                    break;
                case Qt::Key_PageUp:
                    // NUMPAD 9
                    commandLine->clear();
                    commandLine->writeCommand("ne");
                    break;
                case Qt::Key_Left:
                    // NUMPAD 4
                    commandLine->clear();
                    commandLine->writeCommand("w");
                    break;
                case Qt::Key_Clear:
                    // NUMPAD 5
                    commandLine->clear();
                    commandLine->writeCommand("out");
                    break;
                case Qt::Key_Right:
                    // NUMPAD 6
                    commandLine->clear();
                    commandLine->writeCommand("e");
                    break;
                case Qt::Key_End:
                    // NUMPAD 1
                    commandLine->clear();
                    commandLine->writeCommand("sw");
                    break;
                case Qt::Key_Down:
                    // NUMPAD 2
                    commandLine->clear();
                    commandLine->writeCommand("s");
                    break;
                case Qt::Key_PageDown:
                    // NUMPAD 3
                    commandLine->clear();
                    commandLine->writeCommand("se");
                    break;
                default:
                    return false;
                    break;
            }
            return true;
        }
        return false;
    }
    return false;
}
