#include "macromapper.h"

MacroMapper::MacroMapper(QWidget *parent) : QLineEdit(parent) {
}

void MacroMapper::keyPressEvent(QKeyEvent *event) {
    //commandLine->writeCommand();
    /*if (event->key() == Qt::Key_Home && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 7
            this->clear();
            this->setText("nw");
            this->writeCommand();
        } else if (event->key() == Qt::Key_Up && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 8
            this->clear();
            this->setText("n");
            this->writeCommand();
        } else if (event->key() == Qt::Key_PageUp && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 9
            this->clear();
            this->setText("ne");
            this->writeCommand();
        } else if (event->key() == Qt::Key_Left && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 4
            this->clear();
            this->setText("w");
            this->writeCommand();
        } else if (event->key() == Qt::Key_Clear && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 5
            this->clear();
            this->setText("out");
            this->writeCommand();
        } else if (event->key() == Qt::Key_Right && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 6
            this->clear();
            this->setText("e");
            this->writeCommand();
        } else if (event->key() == Qt::Key_End && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 1
            this->clear();
            this->setText("sw");
            this->writeCommand();
        } else if (event->key() == Qt::Key_Down && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 2
            this->clear();
            this->setText("s");
            this->writeCommand();
        } else if (event->key() == Qt::Key_PageDown && (event->modifiers() == Qt::KeypadModifier)) {
            // NUMPAD 3
            this->clear();
            this->setText("se");
            this->writeCommand();
        }*/
}
