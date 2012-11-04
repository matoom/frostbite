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

void WindowManager::setGameWindowFont(QFont font) {
    gameWindow->setFont(font);
}

void WindowManager::setGameWindowFontColor(QColor color) {
    QPalette palette = gameWindow->palette();
    palette.setColor(QPalette::Text, color);
    gameWindow->setPalette(palette);
}

void WindowManager::setDockColor(QColor backgroundColor, QColor fontColor) {    
    foreach(QDockWidget* dock, dockWindows) {
        dock->setStyleSheet(QString("QTextEdit { background: %1;"
            "color: %2; border: 2px inset #b5b5b5;}").arg(backgroundColor.name(), fontColor.name()));
    }
}

void WindowManager::setDockFont(QFont font) {    
    foreach(QDockWidget* dock, dockWindows) {
        ((QTextEdit*)dock->widget())->setFont(font);
    }
}

void WindowManager::updateWindowStyle() {
    QString style = "#_BODY {}"
        "#_SPEECH {color: " + getColor(SPEECH, SPEECH_COLOR_HEX) + "; /*font-family: Consolas;*/}"
        "#_BONUS {color: " + getColor(BONUS, BONUS_COLOR_HEX) + "; /*font-family: Consolas;*/}"
        "#_PENALTY {color: " + getColor(PENALTY, PENALTY_COLOR_HEX) + "; /*font-family: Consolas;*/}"
        "#_THINKING {color: " + getColor(THINKING, THINKING_COLOR_HEX) + "; /*font-family: Consolas;*/}"
        "#_ROOM_NAME {color: " + getColor(ROOM_NAME, ROOM_NAME_COLOR_HEX) + "; /*font-family: Consolas;*/}"
        "#_BOLD {color: " + getColor(GAME_MESSAGE, GAME_MESSAGE_COLOR_HEX) + "; /*font-family: Consolas;*/}";

    foreach(QDockWidget* dock, dockWindows) {
        ((QTextEdit*)dock->widget())->document()->setDefaultStyleSheet(style);
    }

    ((GameWindow*)this->gameWindow)->document()->setDefaultStyleSheet(style);
}

void WindowManager::loadWindows() {                
    gameWindow = (QTextEdit*)new GameWindow(mainWindow);
    mainWindow->addWidgetMainLayout(gameWindow);

    roomWindow = genericWindow->createWindow("Room");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, roomWindow);
    dockWindows << roomWindow;

    //qDebug() << ((QTextEdit*)roomWindow->widget())->toPlainText();

    /*QSizePolicy policy = roomWindow->sizePolicy();
    policy.setHorizontalPolicy(QSizePolicy::Fixed);
    policy.setVerticalPolicy(QSizePolicy::Fixed);*/

    //roomWindow->size();

    arrivalsWindow = genericWindow->createWindow("Arrivals");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, arrivalsWindow);
    dockWindows << arrivalsWindow;

    deathsWindow = genericWindow->createWindow("Deaths");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, deathsWindow);
    dockWindows << deathsWindow;

    thoughtsWindow = genericWindow->createWindow("Thoughts");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, thoughtsWindow);
    dockWindows << thoughtsWindow;
    connect(thoughtsWindow, SIGNAL(visibilityChanged(bool)), this, SLOT(thoughtsVisibility(bool)));

    expWindow = genericWindow->createWindow("Experience");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, expWindow);
    dockWindows << expWindow;

    conversationsWindow = genericWindow->createWindow("Conversations");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, conversationsWindow);
    dockWindows << conversationsWindow;

    if(!clientSettings->hasValue("MainWindow/state")) {
        mainWindow->tabifyDockWidget(thoughtsWindow, arrivalsWindow);
        mainWindow->tabifyDockWidget(arrivalsWindow, deathsWindow);
        mainWindow->tabifyDockWidget(roomWindow, conversationsWindow);
    }
    this->updateWindowStyle();
}

void WindowManager::thoughtsVisibility(bool visibility) {
    thoughtsVisible = visibility;

    //https://bugreports.qt-project.org/browse/QTBUG-840

    //qDebug() << thoughtsWindow->style();
    //qDebug() << visibility;
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

void WindowManager::updateNavigationDisplay(QList<QString> directions) {
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
    QHash<QString, QString> exp = gameDataContainer->getExp();

    QString expString = "";
    foreach (QString value, exp) {
        expString += highlighter->highlight(value + "\n");
    }

    text->clear();
    text->append("<span style=\"white-space:pre;\" id=\"_BODY\">" + expString + "</span>");
}

void WindowManager::updateConversationsWindow(QString conversationText) {
    QTextEdit *text = (QTextEdit*)conversationsWindow->widget();
    text->append("<span style=\"white-space:pre;\" id=\"_BODY\">" + conversationText.trimmed() + "</span>");
}

void WindowManager::updateDeathsWindow(QString deathText) {
    QTextEdit *text = (QTextEdit*)deathsWindow->widget();
    text->append("<span style=\"white-space:pre;\" id=\"_BODY\">" + deathText.trimmed() + "</span>");
}

void WindowManager::updateThoughtsWindow(QString thoughtText) {
    QTextEdit *text = (QTextEdit*)thoughtsWindow->widget();
    text->append("<span style=\"white-space:pre;\" id=\"_BODY\">" + thoughtText.trimmed() + "</span>");
}

void WindowManager::updateArrivalsWindow(QString arrivalText) {
    QTextEdit *text = (QTextEdit*)arrivalsWindow->widget();
    text->append("<span style=\"white-space:pre;\" id=\"_BODY\">" + arrivalText.trimmed() + "</span>");
}

void WindowManager::updateRoomWindow() {
    QTextEdit *text = (QTextEdit*)roomWindow->widget();
    RoomModel* room = gameDataContainer->getRoom();    

    QString roomText = "<span style=\"white-space:pre;\" id=\"_BODY\">";
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
    gameWindow->append("<span style=\"white-space:pre;\" id=\"_BODY\">" + text + "</span>");
}

void WindowManager::writeGameWindow(QByteArray text) {
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
    delete conversationsWindow;
}
