#include "gamewindow.h"

GameWindow::GameWindow(QWidget *parent) : QTextEdit(parent) {
    mainWindow = (MainWindow*)parent;       
    windowManager = mainWindow->getWindowManager();
    settings = HighlightSettings::Instance();

    QPalette palette;
    palette.setColor(QPalette::Text, QColor(192, 192, 192));
    this->setPalette(palette);

    this->setFontWeight(QFont::Normal);
    this->setFont(QFont("Consolas", 12));
    this->setReadOnly(true);
    this->document()->setMaximumBlockCount(1000);

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
