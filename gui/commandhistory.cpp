#include "commandhistory.h"

CommandHistory::CommandHistory(QWidget *parent) : QLineEdit(parent) {
    historyCounter = -1;
}

void CommandHistory::keyPressEvent(QKeyEvent *event) {
    if(event->key() == Qt::Key_Up && (event->modifiers() != Qt::KeypadModifier)) {
        // move back in history
        historyBack();
    } else if(event->key() == Qt::Key_Down && (event->modifiers() != Qt::KeypadModifier)) {
        // move forward in history
        historyForward();
    } else if(event->key() == Qt::Key_Escape && (event->modifiers() != Qt::KeypadModifier)) {
        this->clear();
        historyCounter = -1;
    }
}

void CommandHistory::historyBack() {
    if(history.size() > 0) {
        historyCounter++;
        if(historyCounter >= history.size()) {
            historyCounter = history.size() - 1;
        }
        this->setText(history.at(historyCounter));
    }
}

void CommandHistory::historyForward() {
    if(history.size() > 0) {
        historyCounter--;
        if(historyCounter < 0) {
            historyCounter = -1;
            this->clear();
        } else {
            this->setText(history.at(historyCounter));
        }
    }
}
