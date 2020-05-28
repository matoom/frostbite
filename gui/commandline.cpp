#include "commandline.h"

CommandLine::CommandLine(QWidget *parent) : QLineEdit(parent) {
    mainWindow = (MainWindow*)parent;
    roundtimeDisplay = new RoundTimeDisplay(parent);
    macroService = new MacroService(parent);
    windowFacade = mainWindow->getWindowFacade();
    wordCompleter = new WordCompleter(this);
    keyboardFilter = new KeyboardFilter(this);
    settings = GeneralSettings::getInstance();

    genieUtils = new GenieUtils(mainWindow);

    historyCounter = -1;

    this->setStyleSheet("QLineEdit { min-width: 50em;"
                        "padding: 2px;"
                        "padding-bottom: 6px;"
                        "padding-top: 12px;}");

    connect(this, SIGNAL(returnPressed()), this, SLOT(sendCommand()));
    connect(this, SIGNAL(textEdited(const QString&)), this, SLOT(resetCompleter(const QString&)));
    connect(mainWindow, SIGNAL(profileChanged()), this, SLOT(reloadSettings()));
    connect(mainWindow, SIGNAL(profileChanged()), keyboardFilter, SLOT(reloadSettings()));

    this->setAutoFillBackground(true);
    this->loadSettings();

    this->installEventFilter(keyboardFilter);
}

void CommandLine::reloadSettings() {
    settings = GeneralSettings::getInstance();
    this->loadSettings();
}

void CommandLine::loadSettings() {
    QFont font = settings->cmdFont();
    font.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(font);

    QPalette p = this->palette();

    QColor fontColor = settings->cmdFontColor();
    p.setColor(QPalette::Text, fontColor);

    backgroundColor = settings->cmdBackground();
    p.setColor(QPalette::Base, backgroundColor);

    this->setPalette(p);
}

void CommandLine::setCmdFont(QFont font) {
    font.setStyleStrategy(QFont::PreferAntialias);
    this->setFont(font);
}

void CommandLine::setCmdFontColor(QColor color) {
    QPalette p = this->palette();
    p.setColor(QPalette::Text, color);
    this->setPalette(p);
}

void CommandLine::setCmdBgColor(QColor color) {
    this->backgroundColor = color;
    QPalette p = this->palette();
    p.setColor(QPalette::Base, color);
    this->setPalette(p);
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

void CommandLine::clearRt() {
    QPalette pal = this->palette();
    pal.setBrush(QPalette::Base, backgroundColor);
    this->setPalette(pal);
}

void CommandLine::insertRt(QPixmap segmentDisplay, QPixmap numericDisplay) {
    QPalette pal = this->palette();

    QPixmap collage(this->width(), this->height());
    collage.fill(backgroundColor);

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
    QString text = this->text();
    if(text.startsWith("{") && text.endsWith("}")) return;

    if(history.isEmpty()) {
        history.prepend(text);
    } else {
        if(history.first() != text){
            if(history.size() >= MAX_HISTORY_SIZE) {
                history.remove(history.size() - 1);
            }
            history.prepend(text);
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
        QTextCursor mainCursor = windowFacade->getGameWindow()->textCursor();
        if(mainCursor.hasSelection()) {
            windowFacade->getGameWindow()->copy();
        } else {
            windowFacade->copyDock();
        }
    }
}

void CommandLine::windowControl(int key) {
    QPlainTextEdit* gameWindow = mainWindow->getWindowFacade()->getGameWindow();
    QScrollBar* scrollBar = gameWindow->verticalScrollBar();
    switch(key) {
        case Qt::Key_End:
            scrollBar->setValue(scrollBar->maximum());
        break;
        case Qt::Key_Home:
            scrollBar->setValue(0);
        break;
        case Qt::Key_PageUp:
            scrollBar->setValue(scrollBar->value() - scrollBar->pageStep());
        break;
        case Qt::Key_PageDown:
            scrollBar->setValue(scrollBar->value() + scrollBar->pageStep());
        break;
    }
}

void CommandLine::writeCommand(QString text, QString style) {
    if(this->filterCommand(text)) return;    
    this->write(text, style);
}

void CommandLine::write(QString text, QString style) {
    mainWindow->getTcpClient()->writeCommand(text);
    QTextCursor cursor(windowFacade->getGameWindow()->textCursor());
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::PreviousCharacter);
    cursor.select(QTextCursor::WordUnderCursor);

    QString html = "<span class=\"" + style + "\">" + text + "</span>";

    if(cursor.selectedText() == ">") {
        cursor.movePosition(QTextCursor::PreviousRow);
        cursor.movePosition(QTextCursor::EndOfLine);
        cursor.insertHtml(html);
    } else {
        windowFacade->getGameWindow()->appendHtml(html);
    }

    windowFacade->logGameText(text.toLocal8Bit(), MainLogger::COMMAND);
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
    if (this->text().isEmpty()){
        this->writeCommand("");
    } else {
        /* add command to history */
        this->addHistory();
        /* write command to tcp socket and game window */
        this->writeCommand(this->text());
        this->clear();
    }
}

bool CommandLine::filterCommand(QString text) {
    if(text.startsWith(".")){
        if(text.size() > 1 && text.size() < MAX_FILENAME_SIZE) {
            mainWindow->getScriptService()->runScript(text.mid(1));
            this->clear();
            return true;
        }
    } else if(text.startsWith("#")) {
        if(text.startsWith("#profile")) {
            mainWindow->updateProfileSettings(text.mid(8).trimmed(), "L");
        } else if(text.startsWith("#showMap")) {
            mainWindow->getWindowFacade()->getMapFacade()->showMapDialog();
        } else if(text.startsWith("#hideMap")) {
            mainWindow->getWindowFacade()->getMapFacade()->hideMapDialog();
        } else if(text.startsWith("#highlight")) {
            genieUtils->importHighlights(text);
        }
        this->clear();
        return true;
    } else if(text.startsWith("{") && text.endsWith("}")) {
        if(text.startsWith("{RepeatLast}")) {
            if(history.size() > 0) {
               this->write(history.first());
            }
        } else if(text.startsWith("{RepeatSecondToLast}")) {
            if(history.size() > 1) {
               this->write(history.begin()[1]);
            }
        } else if(text.startsWith("{ReturnOrRepeatLast}")) {
            if(this->text().isEmpty() && history.size() > 0) {
                this->write(history.first());
            } else {
                this->addHistory();
                this->write(this->text());
                return true;
            }
        }
        this->clear();
        return true;
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
    delete keyboardFilter;
}
