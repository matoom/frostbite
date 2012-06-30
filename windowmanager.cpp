#include "windowmanager.h"

WindowManager::WindowManager(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    genericWindow = new GenericWindow(parent);
    navigationDisplay = new NavigationDisplay(parent);
}

QTextEdit* WindowManager::getGameWindow() {
    return this->gameWindow;
}

void WindowManager::loadWindows() {
    gameWindow = (QTextEdit*)new GameWindow(mainWindow);
    mainWindow->addWidgetMainLayout(gameWindow);

    room = genericWindow->createWindow("Room");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, room);
    //qDebug() << ((QTextEdit*)room->widget())->toPlainText();

    arrival = genericWindow->createWindow("Arrival");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, arrival);

    thoughts = genericWindow->createWindow("Thoughts");
    mainWindow->addDockWidgetMainWindow(Qt::RightDockWidgetArea, thoughts);
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
    gameWindow->append(text);
}

WindowManager::~WindowManager() {
    delete genericWindow;
    delete gameWindow;
    delete navigationDisplay;
    delete room;
    delete arrival;
    delete thoughts;
}
