#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    genericWindowFactory = new GenericWindowFactory(parent);
    navigationDisplay = new NavigationDisplay(parent);
    gameDataContainer = GameDataContainer::Instance();
    clientSettings = ClientSettings::Instance();
    highlighter = new Highlighter(parent);
    settings = new HighlightSettings();
}

QPlainTextEdit* WindowManager::getGameWindow() {
    return this->gameWindow;
}

QString WindowManager::textColor(QString name, QString defaultValue) {
    return settings->getSingleParameter(
        "GeneralHighlight/" + name + "/color", defaultValue).value<QColor>().name();
}

void WindowManager::setGameWindowFont(QFont font) {
    gameWindow->setFont(font);
}

void WindowManager::setGameWindowFontColor(QColor color) {
    QPalette p = gameWindow->viewport()->palette();
    p.setColor(QPalette::Text, color);

    gameWindow->viewport()->setPalette(p);    
}

void WindowManager::setDockFontColor(QColor fontColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {        
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Text, fontColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }
}

void WindowManager::setDockBackground(QColor backgroundColor) {
    QPalette p;
    foreach(QDockWidget* dock, dockWindows) {
        p = ((QPlainTextEdit*)dock->widget())->viewport()->palette();
        p.setColor(QPalette::Base, backgroundColor);
        ((QPlainTextEdit*)dock->widget())->viewport()->setPalette(p);
    }
}

void WindowManager::setDockFont(QFont font) {    
    foreach(QDockWidget* dock, dockWindows) {
        ((QPlainTextEdit*)dock->widget())->setFont(font);
    }
}

void WindowManager::updateWindowStyle() {
    QString style = "#_SPEECH {color: " + textColor(SPEECH, SPEECH_COLOR_HEX) + ";}"
        "#_WHISPER {color: " + textColor(WHISPER, WHISPER_COLOR_HEX) + ";}"
        "#_BONUS {color: " + textColor(BONUS, BOOST_COLOR_HEX) + ";}"
        "#_PENALTY {color: " + textColor(PENALTY, PENALTY_COLOR_HEX) + ";}"
        "#_THINKING {color: " + textColor(THINKING, THINKING_COLOR_HEX) + ";}"
        "#_ROOM_NAME {color: " + textColor(ROOM_NAME, ROOM_NAME_COLOR_HEX) + ";}"
        "#_ECHO {color: " + textColor(ECHO, ECHO_COLOR_HEX) + ";}"
        "#_SCRIPT {color: " + textColor(SCRIPT, SCRIPT_COLOR_HEX) + ";}"
        "#_BOLD {color: " + textColor(GAME_MESSAGE, GAME_MESSAGE_COLOR_HEX) + ";}";

    foreach(QDockWidget* dock, dockWindows) {
        ((QPlainTextEdit*)dock->widget())->document()->setDefaultStyleSheet(style);
    }

    ((GameWindow*)this->gameWindow)->document()->setDefaultStyleSheet(style);
}

void WindowManager::loadWindows() {                
    gameWindow = (QPlainTextEdit*)new GameWindow(mainWindow);
    mainWindow->addWidgetMainLayout(gameWindow);

    roomWindow = genericWindowFactory->createWindow("Room");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, roomWindow);
    dockWindows << roomWindow;

    //qDebug() << ((QPlainTextEdit*)roomWindow->widget())->toPlainText();

    //https://bugreports.qt-project.org/browse/QTBUG-16252

    arrivalsWindow = genericWindowFactory->createWindow("Arrivals");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, arrivalsWindow);
    dockWindows << arrivalsWindow;
    connect(arrivalsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(arrivalsVisibility(bool)));


    deathsWindow = genericWindowFactory->createWindow("Deaths");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, deathsWindow);
    dockWindows << deathsWindow;
    connect(deathsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(deathsVisibility(bool)));

    thoughtsWindow = genericWindowFactory->createWindow("Thoughts");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, thoughtsWindow);
    dockWindows << thoughtsWindow;
    connect(thoughtsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(thoughtsVisibility(bool)));

    expWindow = genericWindowFactory->createWindow("Experience");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, expWindow);
    dockWindows << expWindow;

    conversationsWindow = genericWindowFactory->createWindow("Conversations");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, conversationsWindow);
    dockWindows << conversationsWindow;
    connect(conversationsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(conversationsVisibility(bool)));

    if(!clientSettings->hasValue("MainWindow/state")) {
        mainWindow->tabifyDockWidget(thoughtsWindow, arrivalsWindow);
        mainWindow->tabifyDockWidget(arrivalsWindow, deathsWindow);
        mainWindow->tabifyDockWidget(roomWindow, conversationsWindow);
    }
    this->updateWindowStyle();
}

void WindowManager::thoughtsVisibility(bool visibility) {
    thoughtsWindow->setWindowTitle(DOCK_TITLE_THOUGHTS);
    thoughtsVisible = visibility;
    //https://bugreports.qt-project.org/browse/QTBUG-840
}

void WindowManager::deathsVisibility(bool visibility) {
    deathsWindow->setWindowTitle(DOCK_TITLE_DEATHS);
    deathsVisible = visibility;
}

void WindowManager::arrivalsVisibility(bool visibility) {
    arrivalsWindow->setWindowTitle(DOCK_TITLE_ARRIVALS);
    arrivalsVisible = visibility;
}

void WindowManager::conversationsVisibility(bool visibility) {
    conversationsWindow->setWindowTitle(DOCK_TITLE_CONVERSATIONS);
    conversationsVisible = visibility;
}

void WindowManager::setVisibilityIndicator(QDockWidget* widget, bool visible, QString title) {
    if(visible) {
        widget->setWindowTitle(title);
    } else {
        widget->setWindowTitle(title + " *");
    }
}

QDockWidget* WindowManager::getRoomWindow() {
    return this->roomWindow;
}

QDockWidget* WindowManager::getArrivalsWindow() {
    return this->arrivalsWindow;
}

QDockWidget* WindowManager::getThoughtsWindow() {
    return this->thoughtsWindow;
}

QDockWidget* WindowManager::getExpWindow() {
    return this->expWindow;
}

QDockWidget* WindowManager::getDeathsWindow() {
    return this->deathsWindow;
}

QDockWidget* WindowManager::getConversationsWindow() {
    return this->conversationsWindow;
}

void WindowManager::copyDock() {
    foreach(QDockWidget* dock, dockWindows) {
        QTextCursor cursor = ((QPlainTextEdit*)dock->widget())->textCursor();
        if(cursor.hasSelection()) {
            ((QPlainTextEdit*)dock->widget())->copy();
            break;
        }
    }
}

void WindowManager::updateNavigationDisplay(DirectionsList directions) {
    navigationDisplay->updateState(directions);
    this->paintNavigationDisplay();
}

void WindowManager::scriptRunning(bool state) {
    navigationDisplay->setAutoPilot(state);
    this->paintNavigationDisplay();
}

/* paints a full screen image to background */
void WindowManager::paintNavigationDisplay() {
    QPixmap image = navigationDisplay->paint();

    QPixmap collage(gameWindow->width(), gameWindow->height());
    collage.fill(Qt::transparent);

    QPainter painter(&collage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    int x = gameWindow->width() - image.width() - 25;
    int y = gameWindow->height() - image.height() - 5;

    painter.drawPixmap(QRectF(x, y, image.width(), image.height()), image,
                       QRectF(0, 0, image.width(), image.height()));

    QPalette palette = gameWindow->viewport()->palette();
    palette.setBrush(QPalette::Base, QBrush(collage));
    gameWindow->viewport()->setPalette(palette);
}

void WindowManager::updateExpWindow() {
    QPlainTextEdit *text = (QPlainTextEdit*)expWindow->widget();
    QHash<QString, QString> exp = gameDataContainer->getExp();

    QString expString = "";
    foreach (QString value, exp) {
        expString += highlighter->highlight(value + "\n");
    }

    text->clear();
    text->appendHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + expString + "</SPAN>");
}

void WindowManager::updateConversationsWindow(QString conversationText) {
    setVisibilityIndicator(conversationsWindow, conversationsVisible, DOCK_TITLE_CONVERSATIONS);

    QPlainTextEdit *text = (QPlainTextEdit*)conversationsWindow->widget();
    text->appendHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + conversationText.trimmed() + "</SPAN>");
}

void WindowManager::updateDeathsWindow(QString deathText) {
    setVisibilityIndicator(deathsWindow, deathsVisible, DOCK_TITLE_DEATHS);

    QPlainTextEdit *text = (QPlainTextEdit*)deathsWindow->widget();
    text->appendHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + deathText.trimmed() + "</SPAN>");
}

void WindowManager::updateThoughtsWindow(QString thoughtText) {
    setVisibilityIndicator(thoughtsWindow, thoughtsVisible, DOCK_TITLE_THOUGHTS);

    QPlainTextEdit *text = (QPlainTextEdit*)thoughtsWindow->widget();
    text->appendHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + thoughtText.trimmed() + "</SPAN>");
}

void WindowManager::updateArrivalsWindow(QString arrivalText) {
    setVisibilityIndicator(arrivalsWindow, arrivalsVisible, DOCK_TITLE_ARRIVALS);

    QPlainTextEdit *text = (QPlainTextEdit*)arrivalsWindow->widget();
    text->appendHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + arrivalText.trimmed() + "</SPAN>");
}

void WindowManager::updateRoomWindow() {
    QPlainTextEdit *text = (QPlainTextEdit*)roomWindow->widget();
    RoomModel* room = gameDataContainer->getRoom();    

    QString roomText = "<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">";
    roomText += highlighter->highlight(room->getDesc() + "\n");
    roomText += room->getObjs().isEmpty() ? "" : highlighter->highlight(room->getObjs() + "\n");
    roomText += room->getPlayers().isEmpty() ? "" : highlighter->highlight(room->getPlayers() + "\n");
    roomText += room->getExits().isEmpty() ? "" : highlighter->highlight(room->getExits() + "\n");
    roomText += "</SPAN>";

    text->clear();
    text->appendHtml(roomText);
}

void WindowManager::updateRoomWindowTitle(QString title) {
    roomWindow->setWindowTitle("Room " + title);
}

void WindowManager::writePromptGameWindow(QByteArray text) {
    QTextCursor cursor(gameWindow->textCursor());
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);

    if(!cursor.selectedText().contains(">")) {
        gameWindow->appendHtml("<SPAN STYLE=\"WHITE-SPACE:PRE;\" ID=\"_BODY\">" + text + "</SPAN>");
    }
}

void WindowManager::writeGameText(QByteArray text) {
    if(!text.isEmpty()) {
        QXmlStreamReader xml(text);
        QString textString;
        while (!xml.atEnd()) {
            if ( xml.readNext() == QXmlStreamReader::Characters ) {
                textString += xml.text();
            }
        }
        mainWindow->getScriptService()->writeOutgoingMessage(textString.toLocal8Bit());
    }
    gameWindow->appendHtml(text);
}

void WindowManager::writeGameWindow(QByteArray text) {
    gameWindow->appendHtml(text);
}

WindowManager::~WindowManager() {
    delete genericWindowFactory;
    delete gameWindow;
    delete navigationDisplay;
    delete roomWindow;
    delete arrivalsWindow;
    delete thoughtsWindow;
    delete deathsWindow;
    delete conversationsWindow;
    delete settings;
}
