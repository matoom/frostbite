#include "gamewindow.h"

GameWindow::GameWindow(QWidget *parent) : QTextEdit(parent) {
    mainWindow = (MainWindow*)parent;       
    windowManager = mainWindow->getWindowManager();

    QPalette palette;
    palette.setColor(QPalette::Text, QColor(192, 192, 192));

    this->setPalette(palette);

    this->setFontWeight(QFont::Normal);
    this->setFont(QFont("Fixedsys", 12));
    this->setReadOnly(true);
    this->document()->setMaximumBlockCount(5000);
    this->document()->setDefaultStyleSheet("#body {color: #c0c0c0; font-family: fixedsys;}"
                                           "#speech {color: #00ff00; font-family: fixedsys;}"
                                           "#thought {color: #800000; font-family: fixedsys;}"
                                           "#roomName {color: white; font-family: fixedsys;}"
                                           "#youAlsoSee {color: #00ffff; font-family: fixedsys;}"
                                           "#alsoHere {color: #ffff00; font-family: fixedsys;}"
                                           "#bold {color: #ffff00; font-family: fixedsys;}}");

    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(adjustRowMargin()));
}

/* adjust right margin for bottom 5 blocks to clear compass view */
void GameWindow::adjustRowMargin() {
    if(this->verticalScrollBar()->isVisible()) {
        QTextBlockFormat format;
        QTextBlock block = this->document()->findBlockByNumber(this->document()->blockCount() - 1);
        format = block.blockFormat();
        format.setRightMargin(105);

        QTextCursor cur = this->textCursor();
        cur.setPosition(block.position());
        cur.setBlockFormat(format);

        if(this->document()->blockCount() > 5) {
            block = this->document()->findBlockByNumber(this->document()->blockCount() - 5);
            format = block.blockFormat();
            format.setRightMargin(0);

            cur = this->textCursor();
            cur.setPosition(block.position());
            cur.setBlockFormat(format);
        }
    }
}

void GameWindow::contextMenuEvent(QContextMenuEvent *event) {
    QTextEdit::contextMenuEvent(event);
}

void GameWindow::mouseReleaseEvent(QMouseEvent *event) {
    if(event->button() != Qt::RightButton) {
        this->copySelected();
        mainWindow->getCommandLine()->focus();
    }

    QTextEdit::mousePressEvent(event);
}

void GameWindow::resizeEvent(QResizeEvent *event) {
    windowManager->paintNavigationDisplay();
    QTextEdit::resizeEvent(event);
}

void GameWindow::wheelEvent(QWheelEvent  *event) {
    mainWindow->getCommandLine()->focus();
    QTextEdit::wheelEvent(event);
}

void GameWindow::copySelected() {
    this->copy();

    QTextCursor textCursor = this->textCursor();
    textCursor.clearSelection();
    this->setTextCursor(textCursor);
}
