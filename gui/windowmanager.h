#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QDockWidget>

#include <mainwindow.h>
#include <genericwindowfactory.h>
#include <gamewindow.h>
#include <navigationdisplay.h>
#include <gamedatacontainer.h>
#include <highlighter.h>
#include <defaultvalues.h>

class MainWindow;
class GameWindow;
class GenericWindowFactory;
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
    QPlainTextEdit* getGameWindow();
    void paintNavigationDisplay();
    void writePromptGameWindow(QByteArray);
    void writeGameWindow(QByteArray);
    void updateNavigationDisplay(QList<QString>);
    void scriptRunning(bool);
    void updateExpWindow();
    void updateRoomWindow();
    void updateConversationsWindow(QString);
    void updateDeathsWindow(QString);
    void updateRoomWindowTitle(QString);
    void updateThoughtsWindow(QString);
    void updateArrivalsWindow(QString);
    void updateWindowStyle();
    void setGameWindowFont(QFont);
    void setGameWindowFontColor(QColor);    
    void setDockFontColor(QColor);
    void setDockBackground(QColor);
    void setDockFont(QFont font);
    void copyDock();

    QDockWidget* getRoomWindow();
    QDockWidget* getArrivalsWindow();
    QDockWidget* getThoughtsWindow();
    QDockWidget* getExpWindow();
    QDockWidget* getDeathsWindow();
    QDockWidget* getConversationsWindow();

    bool thoughtsVisible;

public slots:

private slots:
    void thoughtsVisibility(bool);

private:
    GenericWindowFactory* genericWindowFactory;
    MainWindow* mainWindow;
    QPlainTextEdit* gameWindow;
    NavigationDisplay *navigationDisplay;
    GameDataContainer* gameDataContainer;
    ClientSettings* clientSettings;
    Highlighter* highlighter;
    HighlightSettings* settings;

    QDockWidget* roomWindow;
    QDockWidget* arrivalsWindow;
    QDockWidget* thoughtsWindow;
    QDockWidget* expWindow;
    QDockWidget* deathsWindow;
    QDockWidget* conversationsWindow;
    QList<QDockWidget*> dockWindows;

    QString textColor(QString, QString);
};

#endif // WINDOWMANAGER_H
