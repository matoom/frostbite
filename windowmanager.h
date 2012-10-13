#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QDockWidget>

#include <mainwindow.h>
#include <genericwindow.h>
#include <gamewindow.h>
#include <navigationdisplay.h>
#include <gamedatacontainer.h>
#include <highlighter.h>
#include <defaultvalues.h>

class MainWindow;
class GameWindow;
class GenericWindow;
class NavigationDisplay;
class GameDataContainer;
class ClientSettings;
class Highlighter;

class WindowManager : public QObject {
    Q_OBJECT

public:
    WindowManager(QObject *parent = 0);
    ~WindowManager();

    void loadWindows();
    QTextEdit* getGameWindow();
    void paintNavigationDisplay();
    void writePromptGameWindow(QByteArray);
    void writeGameWindow(QByteArray);
    void updateNavigationDisplay(QList<QString>);
    void updateExpWindow();
    void updateRoomWindow();
    void updateDeathsWindow(QString);
    void updateRoomWindowTitle(QString);
    void updateThoughtsWindow(QString);
    void updateArrivalsWindow(QString);
    void updateWindowStyle();

    QDockWidget *getRoomWindow();
    QDockWidget *getArrivalsWindow();
    QDockWidget *getThoughtsWindow();
    QDockWidget *getExpWindow();
    QDockWidget *getDeathsWindow();

public slots:


private:
    GenericWindow* genericWindow;
    MainWindow* mainWindow;
    QTextEdit* gameWindow;
    NavigationDisplay *navigationDisplay;
    GameDataContainer* gameDataContainer;
    ClientSettings* clientSettings;
    Highlighter* highlighter;
    HighlightSettings* settings;

    QDockWidget *roomWindow;
    QDockWidget *arrivalsWindow;
    QDockWidget *thoughtsWindow;
    QDockWidget *expWindow;
    QDockWidget *deathsWindow;

    QString getColor(QString, QString);
};

#endif // WINDOWMANAGER_H
