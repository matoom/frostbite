#include "commandline.h"

CommandLine::CommandLine(QWidget *parent) : QLineEdit(parent) {
    mainWindow = (MainWindow*)parent;
    roundtimeDisplay = new RoundTimeDisplay(parent);
    windowManager = mainWindow->getWindowManager();

    historyCounter = -1;

    this->setFont(QFont("Fixedsys", 12));

    this->setStyleSheet("QLineEdit { min-width: 50em;"
                        "padding: 2px;"
                        "padding-bottom: 6px;"
                        "padding-top: 10px;}");

    connect(this, SIGNAL(returnPressed()), this, SLOT(sendCommand()));

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

    /* temp init timer */
    roundtimeDisplay->setTimer((qrand()%10) + 1);
}

void CommandLine::insertRtIndicator(QPixmap segmentDisplay, QPixmap numericDisplay) {
    QPalette pal;

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

void CommandLine::writeCommand() {
    mainWindow->getConnectionManager()->writeCommand(this->text());

    QTextCursor cursor(windowManager->getGameWindow()->textCursor());
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::PreviousCharacter);
    cursor.select(QTextCursor::WordUnderCursor);

    if(cursor.selectedText() == ">") {
        cursor.movePosition(QTextCursor::EndOfLine);
        windowManager->getGameWindow()->setTextCursor(cursor);
        windowManager->getGameWindow()->insertPlainText(this->text());
    } else {
        windowManager->getGameWindow()->append(this->text());
    }

    this->clear();
}

void CommandLine::sendCommand() {
    if (!this->text().isEmpty()){
        /* add command to history */
        this->addHistory();
        /* write command to tcp socket and game window */
        this->writeCommand();
    }
}

CommandLine::~CommandLine() {
    delete roundtimeDisplay;
}
