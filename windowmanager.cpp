#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    genericWindow = new GenericWindow(parent);
    navigationDisplay = new NavigationDisplay(parent);
    gameDataContainer = GameDataContainer::Instance();
    clientSettings = ClientSettings::Instance();
    highlighter = new Highlighter(parent);
    settings = HighlightSettings::Instance();
}

QTextEdit* WindowManager::getGameWindow() {
    return this->gameWindow;
}

QString WindowManager::getColor(QString name, QString defaultValue) {
    return settings->getSingleParameter("GeneralHighlight/" + name + "/color", defaultValue).value<QColor>().name();
}

void WindowManager::updateWindowStyle() {
    QString style = "#_BODY {color: " + QString(BODY_COLOR_HEX) + "; font-family: Consolas;}"
        "#_SPEECH {color: " + getColor("speech", SPEECH_COLOR_HEX) + "; font-family: Consolas;}"
        "#_BONUS {color: #00ff00; font-family: Consolas;}"
        "#_PENALTY {color: #800000; font-family: Consolas;}"
        "#_THINKING {color: " + getColor("thinking", THINKING_COLOR_HEX) + "; font-family: Consolas;}"
        "#_ROOM_NAME {color: " + getColor("roomName", ROOM_NAME_COLOR_HEX) + "; font-family: Consolas;}"
        "#_BOLD {color: " + getColor("gameMessage", GAME_MESSAGE_COLOR_HEX) + "; font-family: Consolas;}";

    ((QTextEdit*)deathsWindow->widget())->document()->setDefaultStyleSheet(style);
    ((QTextEdit*)thoughtsWindow->widget())->document()->setDefaultStyleSheet(style);
    ((QTextEdit*)arrivalsWindow->widget())->document()->setDefaultStyleSheet(style);
    ((QTextEdit*)roomWindow->widget())->document()->setDefaultStyleSheet(style);
    ((GameWindow*)this->gameWindow)->document()->setDefaultStyleSheet(style);
}

void WindowManager::loadWindows() {
    gameWindow = (QTextEdit*)new GameWindow(mainWindow);
    mainWindow->addWidgetMainLayout(gameWindow);

    roomWindow = genericWindow->createWindow("Room");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, roomWindow);
    //qDebug() << ((QTextEdit*)roomWindow->widget())->toPlainText();

    /*QSizePolicy policy = roomWindow->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);*/

    //roomWindow->size();

    arrivalsWindow = genericWindow->createWindow("Arrivals");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, arrivalsWindow);

    deathsWindow = genericWindow->createWindow("Deaths");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, deathsWindow);

    thoughtsWindow = genericWindow->createWindow("Thoughts");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, thoughtsWindow);

    expWindow = genericWindow->createWindow("Experience");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, expWindow);

    if(!clientSettings->hasValue("MainWindow/state")) {
        mainWindow->tabifyDockWidget(thoughtsWindow, arrivalsWindow);
        mainWindow->tabifyDockWidget(arrivalsWindow, deathsWindow);
    }

    this->updateWindowStyle();
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

void WindowManager::updateNavigationDisplay(QList<QString> directions) {
    navigationDisplay->updateState(directions);
    this->paintNavigationDisplay();
}

/* paints a full screen image to background */
void WindowManager::paintNavigationDisplay() {
    QPixmap image = navigationDisplay->paint();

    QPalette palette;

    QPixmap collage(gameWindow->width(), gameWindow->height());
    collage.fill(Qt::transparent);

    QPainter painter(&collage);
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

    int x = gameWindow->width() - image.width() - 25;
    int y = gameWindow->height() - image.height() - 5;

    painter.drawPixmap(QRectF(x, y, image.width(), image.height()), image,
                       QRectF(0, 0, image.width(), image.height()));

    palette.setBrush(QPalette::Base, QBrush(collage));
    gameWindow->viewport()->setPalette(palette);
}

void WindowManager::updateExpWindow() {
    QTextEdit *text = (QTextEdit*)expWindow->widget();
    QHash<QString, ExpModel*> exp = gameDataContainer->getExp();

    QString expString = "";
    foreach (ExpModel *value, exp) {
        expString += highlighter->highlight(value->getExpString() + "\n");
    }

    text->clear();
    text->append("<span style=\"white-space:pre;\" id=\"body\">" + expString + "</span>");
}

void WindowManager::updateDeathsWindow(QString deathText) {
    QTextEdit *text = (QTextEdit*)deathsWindow->widget();
    text->append("<span style=\"white-space:pre;\" id=\"body\">" + deathText.trimmed() + "</span>");
}

void WindowManager::updateThoughtsWindow(QString thoughtText) {
    QTextEdit *text = (QTextEdit*)thoughtsWindow->widget();
    text->append("<span style=\"white-space:pre;\" id=\"body\">" + thoughtText.trimmed() + "</span>");
}

void WindowManager::updateArrivalsWindow(QString arrivalText) {
    QTextEdit *text = (QTextEdit*)arrivalsWindow->widget();
    text->append("<span style=\"white-space:pre;\" id=\"body\">" + arrivalText.trimmed() + "</span>");
}

void WindowManager::updateRoomWindow() {
    QTextEdit *text = (QTextEdit*)roomWindow->widget();
    RoomModel* room = gameDataContainer->getRoom();    

    QString roomText = "<span style=\"white-space:pre;\" id=\"body\">";
    roomText += highlighter->highlight(room->getDesc() + "\n");
    roomText += room->getObjs().isEmpty() ? "" : highlighter->highlight(room->getObjs() + "\n");
    roomText += room->getPlayers().isEmpty() ? "" : highlighter->highlight(room->getPlayers() + "\n");
    roomText += room->getExits().isEmpty() ? "" : highlighter->highlight(room->getExits() + "\n");
    roomText += "</span>";

    text->clear();
    text->append(roomText);
}

void WindowManager::updateRoomWindowTitle(QString title) {
    roomWindow->setWindowTitle("Room " + title);
}

void WindowManager::writePromptGameWindow(QByteArray text) {
    QTextCursor cursor(gameWindow->textCursor());
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::PreviousCharacter);
    cursor.select(QTextCursor::WordUnderCursor);

    if(cursor.selectedText() == ">") {
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.removeSelectedText();
    }
    gameWindow->append(text);
}

void WindowManager::writeGameWindow(QByteArray text) {
    // if script is running
    //mainWindow->getScriptService()->writeOutgoingMessage("game_text#" + text);

    //qDebug() text;

    gameWindow->append(text);
}

WindowManager::~WindowManager() {
    delete genericWindow;
    delete gameWindow;
    delete navigationDisplay;
    delete roomWindow;
    delete arrivalsWindow;
    delete thoughtsWindow;
    delete deathsWindow;
}
