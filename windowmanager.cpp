#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    genericWindow = new GenericWindow(parent);
    navigationDisplay = new NavigationDisplay(parent);
    gameDataContainer = GameDataContainer::Instance();
}

QTextEdit* WindowManager::getGameWindow() {
    return this->gameWindow;
}

void WindowManager::loadWindows() {
    gameWindow = (QTextEdit*)new GameWindow(mainWindow);
    mainWindow->addWidgetMainLayout(gameWindow);

    roomWindow = genericWindow->createWindow("Room");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, roomWindow);
    //qDebug() << ((QTextEdit*)roomWindow->widget())->toPlainText();

    arrivalsWindow = genericWindow->createWindow("Arrivals");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, arrivalsWindow);

    deathsWindow = genericWindow->createWindow("Deaths");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, deathsWindow);

    thoughtsWindow = genericWindow->createWindow("Thoughts");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, thoughtsWindow);

    expWindow = genericWindow->createWindow("Experience");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, expWindow);
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
        expString += value->getExpString() + "\n";
        text->setPlainText(expString);
    }
}

void WindowManager::updateDeathsWindow(QString deathText) {
    QTextEdit *text = (QTextEdit*)deathsWindow->widget();
    text->append(deathText.trimmed());
}

void WindowManager::updateThoughtsWindow(QString thoughtText) {
    QTextEdit *text = (QTextEdit*)thoughtsWindow->widget();
    text->append(thoughtText.trimmed());
}

void WindowManager::updateArrivalsWindow(QString arrivalText) {
    QTextEdit *text = (QTextEdit*)arrivalsWindow->widget();
    text->append(arrivalText.trimmed());
}

void WindowManager::updateRoomWindow() {
    QTextEdit *text = (QTextEdit*)roomWindow->widget();
    RoomModel* room = gameDataContainer->getRoom();
    text->setPlainText(room->getDesc() + "\n" + room->getObjs() + "\n" + room->getPlayers() + room->getExits());
    text->verticalScrollBar()->triggerAction(QAbstractSlider::SliderToMaximum);
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

    //qDebug() << text;

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
