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
#include <highlightsettings.h>

class MainWindow;
class GameWindow;
class GenericWindowFactory;
class NavigationDisplay;
class GameDataContainer;
class ClientSettings;
class Highlighter;

typedef QList<QString> DirectionsList;

class WindowManager : public QObject {
    Q_OBJECT

public:
    WindowManager(QObject *parent = 0);
    ~WindowManager();

    void loadWindows();
    QPlainTextEdit* getGameWindow();
    void paintNavigationDisplay();
    void scriptRunning(bool);
    void updateWindowStyle();
    void setGameWindowFont(QFont);
    void setGameWindowFontColor(QColor);    
    void setDockFontColor(QColor);
    void setDockBackground(QColor);
    void setDockFont(QFont font);
    void copyDock();
    void saveArrivals();
    void writeGameWindow(QByteArray);

    QDockWidget* getRoomWindow();
    QDockWidget* getArrivalsWindow();
    QDockWidget* getThoughtsWindow();
    QDockWidget* getExpWindow();
    QDockWidget* getDeathsWindow();
    QDockWidget* getConversationsWindow();

    bool thoughtsVisible;
    bool deathsVisible;
    bool arrivalsVisible;
    bool conversationsVisible;

public slots:
    void updateConversationsWindow(QString);
    void writeGameText(QByteArray);
    void writePromptGameWindow(QByteArray);
    void updateNavigationDisplay(DirectionsList);
    void updateRoomWindowTitle(QString);
    void updateExpWindow();
    void updateRoomWindow();
    void updateDeathsWindow(QString);
    void updateThoughtsWindow(QString);
    void updateArrivalsWindow(QString);


private slots:
    void thoughtsVisibility(bool);    
    void deathsVisibility(bool);
    void arrivalsVisibility(bool);
    void conversationsVisibility(bool);

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
    void setVisibilityIndicator(QDockWidget*, bool, QString);
};

#endif // WINDOWMANAGER_H
