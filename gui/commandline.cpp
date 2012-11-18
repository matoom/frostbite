#include "commandline.h"

CommandLine::CommandLine(QWidget *parent) : QLineEdit(parent) {
    mainWindow = (MainWindow*)parent;
    roundtimeDisplay = new RoundTimeDisplay(parent);
    macroService = new MacroService(parent);
    windowManager = mainWindow->getWindowManager();
    wordCompleter = new WordCompleter(this);

    historyCounter = -1;

    this->setFont(QFont("Fixedsys", 12));

    this->setStyleSheet("QLineEdit { min-width: 50em;"
                        "padding: 2px;"
                        "padding-bottom: 6px;"
                        "padding-top: 10px;}");

    connect(this, SIGNAL(returnPressed()), this, SLOT(sendCommand()));
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(resetCompleter(const QString&)));

    this->installEventFilter(&keyboardFilter);
}

RoundTimeDisplay* CommandLine::getRoundtimeDisplay() {
    return roundtimeDisplay;
}

void CommandLine::historyBack() {
    if(history.size() > 0) {
        historyCounter++;
        if(historyCounter >= history.size()) {
            historyCounter = history.size() - 1;
        }
        this->setText(history.at(historyCounter));
    }
}

void CommandLine::historyForward() {
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

void CommandLine::resizeEvent(QResizeEvent *event) {
    roundtimeDisplay->repaint();
    QLineEdit::resizeEvent(event);
}

void CommandLine::focus() {
    this->setFocus();
}

void CommandLine::insertRtIndicator(QPixmap segmentDisplay, QPixmap numericDisplay) {
    QPalette pal = this->palette();

    QPixmap collage(this->width(), this->height());
    collage.fill(Qt::white);

    QPainter painter(&collage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    painter.drawPixmap(QRectF(0, 0, segmentDisplay.width(), segmentDisplay.height()), segmentDisplay,
                       QRectF(0, 0, segmentDisplay.width(), segmentDisplay.height()));

    painter.drawPixmap(QRectF(this->width() - 40, 0, numericDisplay.width(), numericDisplay.height()), numericDisplay,
                       QRectF(0, 0, numericDisplay.width(), numericDisplay.height()));

    pal.setBrush(QPalette::Base, QBrush(collage));
    this->setPalette(pal);
}

void CommandLine::addHistory() {
    if(history.isEmpty()) {
        history.prepend(this->text());
    } else {
        if(history.first() != this->text()){
            if(history.size() >= MAX_HISTORY_SIZE) {
                history.remove(history.size() - 1);
            }
            history.prepend(this->text());
        }
    }

    historyCounter = -1;
}

void CommandLine::clearCommand() {
    this->clear();
}

void CommandLine::doCopy() {
    if(this->hasSelectedText()) {
        this->copy();
    } else {
        QTextCursor mainCursor = windowManager->getGameWindow()->textCursor();
        if(mainCursor.hasSelection()) {
            windowManager->getGameWindow()->copy();
        } else {
            windowManager->copyDock();
        }
    }
}

void CommandLine::writeCommand(QString text, QString style) {
    mainWindow->getConnectionManager()->writeCommand(text);

    QTextCursor cursor(windowManager->getGameWindow()->textCursor());
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::PreviousCharacter);
    cursor.select(QTextCursor::WordUnderCursor);

    if(cursor.selectedText() == ">") {
        cursor.movePosition(QTextCursor::PreviousRow);
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.insertHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"" +
            style + "\">" + text + "</SPAN>");
    } else {
        windowManager->getGameWindow()->appendHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"" +
            style + "\">" + text + "</SPAN>");
    }
}

void CommandLine::completeCommand() {
    if(!this->text().isEmpty()) {
        this->setText(wordCompleter->complete(this->text()));
    }
}

void CommandLine::resetCompleter(const QString&) {
   wordCompleter->match = true;
}

void CommandLine::moveCursor(int pos) {
    this->setCursorPosition(pos);
}

bool CommandLine::runMacro(QString cmd) {
    return macroService->execute(cmd);
}

void CommandLine::sendCommand() {
    if (!this->text().isEmpty()){
        /* add command to history */
        this->addHistory();
        /* look for client commands */
        if(!this->filterCommand(this->text())) {
            /* write command to tcp socket and game window */
            this->writeCommand(this->text());
            this->clear();
        }
    }
}

bool CommandLine::filterCommand(QString text) {
    if(text.startsWith(".")){
        if(text.size() > 1 && text.size() < MAX_FILENAME_SIZE) {
            mainWindow->getScriptService()->runScript(text.mid(1));
            this->clear();
            return true;
        }
    }
    return false;
}

void CommandLine::abortScript() {
    mainWindow->getScriptService()->abortScript();
}

void CommandLine::abortSequence() {
    macroService->abortSequence();
}

CommandLine::~CommandLine() {
    delete roundtimeDisplay;
}
