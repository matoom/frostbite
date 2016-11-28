#ifndef WindowFacade_H
#define WindowFacade_H

#include <QDockWidget>

#include <mainwindow.h>
#include <genericwindowfactory.h>
#include <gridwindowfactory.h>
#include <gamewindow.h>
#include <navigationdisplay.h>
#include <gamedatacontainer.h>
#include <defaultvalues.h>
#include <text/highlight/highlighter.h>
#include <text/highlight/highlightsettings.h>
#include <windowwriterthread.h>
#include <gridwriterthread.h>
#include <mainlogger.h>
#include <thoughtslogger.h>
#include <conversationslogger.h>
#include <deathslogger.h>
#include <arrivalslogger.h>

#include <gridwindow.h>

#include <maps/mapfacade.h>

#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>

class MainWindow;
class GameWindow;
class GridWindowFactory;
class GenericWindowFactory;
class NavigationDisplay;
class GameDataContainer;
class ClientSettings;
class Highlighter;
class WindowWriterThread;
class GridWriterThread;
class MainLogger;
class ThoughtsLogger;
class ConversationsLogger;
class DeathsLogger;
class ArrivalsLogger;
class MapFacade;

typedef QList<QString> DirectionsList;
typedef QMap<QString, QString> GridItems;

class WindowFacade : public QObject {
    Q_OBJECT

public:
    WindowFacade(QObject *parent = 0);
    ~WindowFacade();

    void loadWindows();
    QPlainTextEdit* getGameWindow();
    void paintNavigationDisplay();
    void scriptRunning(bool);
    void updateWindowStyle();
    QString getStyle();
    void initWindowWriters();
    void initLoggers();
    void updateWindowColors();
    void setGameWindowFont(QFont);
    void setGameWindowFontColor(QColor);    
    void setTextDockFontColor(QColor);
    void setTextDockBackground(QColor);
    void setTextDockFont(QFont font);
    void setGridDockBackground(QColor);
    void setGridDockFontColor(QColor);
    void setGridDockFont(QFont font);
    void copyDock();
    void saveArrivals();
    void reloadSettings();
    void reloadHighlighterSettings();    

    QDockWidget* getRoomWindow();
    QDockWidget* getArrivalsWindow();
    QDockWidget* getThoughtsWindow();
    QDockWidget* getExpWindow();
    QDockWidget* getDeathsWindow();
    QDockWidget* getConversationsWindow();
    QDockWidget* getFamiliarWindow();

    MapFacade* getMapFacade();

    bool thoughtsVisible;
    bool deathsVisible;
    bool arrivalsVisible;
    bool conversationsVisible;
    bool familiarVisible;
    bool writePrompt;

public slots:
    void updateConversationsWindow(QString);
    void writeGameText(QByteArray, bool);    
    void writeGameWindow(QByteArray);    
    void updateNavigationDisplay(DirectionsList);
    void updateRoomWindowTitle(QString);
    void updateExpWindow(QString name, QString text);
    void updateMapWindow(QString hash);
    void updateRoomWindow();
    void updateDeathsWindow(QString);
    void updateThoughtsWindow(QString);    
    void updateArrivalsWindow(QString);
    void updateFamiliarWindow(QString);

    void logThoughtsText(QString);
    void logConversationsText(QString);
    void logGameText(QByteArray, char type = '\0');
    void logDeathsText(QString);
    void logArrivalsText(QString);

    /*
    void mapsReady();
    void mapSelected(int index);
    void mapLevelSelected(int index);
    void showMap(QString id, int level = 0);
    void selectNode(QString zoneId, int nodeId);
    */

private slots:
    void thoughtsVisibility(bool);    
    void deathsVisibility(bool);
    void arrivalsVisibility(bool);
    void conversationsVisibility(bool);
    void familiarVisibility(bool);
    void writeExpWindow(GridItems);

private:
    GridWindowFactory* gridWindowFactory;
    GenericWindowFactory* genericWindowFactory;

    MainWindow* mainWindow;
    QPlainTextEdit* gameWindow;
    NavigationDisplay* navigationDisplay;
    GameDataContainer* gameDataContainer;
    ClientSettings* clientSettings;
    Highlighter* highlighter;
    HighlightSettings* settings;
    GeneralSettings* generalSettings;

    QDockWidget* roomWindow;
    QDockWidget* arrivalsWindow;
    QDockWidget* thoughtsWindow;
    QDockWidget* expWindow;
    QDockWidget* deathsWindow;
    QDockWidget* conversationsWindow;
    QDockWidget* familiarWindow;
    QList<QDockWidget*> dockWindows;
    QList<QDockWidget*> gridWindows;

    MapFacade* mapFacade;

    QRegExp rxRemoveTags;

    WindowWriterThread* mainWriter;
    WindowWriterThread* roomWriter;
    GridWriterThread* expWriter;
    WindowWriterThread* arrivalsWriter;
    WindowWriterThread* thoughtsWriter;
    WindowWriterThread* deathsWriter;
    WindowWriterThread* conversationsWriter;
    WindowWriterThread* familiarWriter;
    QList<WindowWriterThread*> writers;
    QList<GridWriterThread*> gridWriters;

    MainLogger* mainLogger;
    ThoughtsLogger* thoughtsLogger;
    ConversationsLogger* conversationsLogger;
    DeathsLogger* deathsLogger;
    ArrivalsLogger* arrivalsLogger;

    QString style;

    QString textColor(QString, QString);
    void setVisibilityIndicator(QDockWidget*, bool, QString);

signals:
    void updateGameWindowSettings();
    void updateRoomSettings();
    void updateExpSettings();    
    void updateArrivalsSettings();
    void updateThoughtsSettings();
    void updateDeathsSettings();
    void updateConversationsSettings();
    void updateFamiliarSettings();

    //void setMapLabel(QString text);
    //void nodeSelected(MapZone* zone, int nodeId);
};

#endif // WindowFacade_H
