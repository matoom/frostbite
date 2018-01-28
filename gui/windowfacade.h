#ifndef WindowFacade_H
#define WindowFacade_H

#include <QDockWidget>

#include <mainwindow.h>
#include <genericwindowfactory.h>
#include <gridwindowfactory.h>
#include <gamewindow.h>
#include <compass.h>
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
#include <compass/compassview.h>

#include <gridwindow.h>

#include <maps/mapfacade.h>

#include <window/roomwindow.h>
#include <window/arrivalswindow.h>
#include <window/deathswindow.h>
#include <window/thoughtswindow.h>
#include <window/expwindow.h>
#include <window/conversationswindow.h>
#include <window/familiarwindow.h>
#include <window/spellwindow.h>
#include <window/atmosphericswindow.h>

#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>

class MainWindow;
class GameWindow;
class GenericWindowFactory;
class NavigationDisplay;
class GameDataContainer;
class ClientSettings;
class Highlighter;
class MainLogger;
class MapFacade;
class CompassView;

class RoomWindow;
class ArrivalsWindow;
class DeathsWindow;
class ThoughtsWindow;
class ExpWindow;
class ConversationsWindow;
class FamiliarWindow;
class SpellWindow;
class AtmosphericsWindow;


typedef QList<QString> DirectionsList;
typedef QMap<QString, QString> GridItems;

class WindowFacade : public QObject {
    Q_OBJECT

public:
    WindowFacade(QObject *parent = 0);
    ~WindowFacade();

    void loadWindows();
    QPlainTextEdit* getGameWindow();
    void paintCompass();
    void gameWindowResizeEvent(GameWindow*);
    void scriptRunning(bool);
    void updateWindowStyle();
    QString getStyle();
    void initWindowWriters();
    void initLoggers();
    void updateWindowColors();
    void setGameWindowFont(QFont);
    void setGameWindowFontColor(QColor);    
    void setGameWindowBackground(QColor color);
    void setDockFontColor(QColor);
    void setDockBackground(QColor);
    void setDockFont(QFont font);
    void copyDock();
    void saveArrivals();
    void reloadHighlighterSettings();    

    QList<QDockWidget*> getDockWindows();
    RoomWindow* getRoomWindow();
    ArrivalsWindow* getArrivalsWindow();
    ThoughtsWindow* getThoughtsWindow();
    ExpWindow* getExpWindow();
    DeathsWindow* getDeathsWindow();
    ConversationsWindow* getConversationsWindow();
    FamiliarWindow* getFamiliarWindow();
    SpellWindow* getSpellWindow();
    AtmosphericsWindow* getAtmosphericsWindow();

    QStringList getWindowNames();

    MapFacade* getMapFacade();    
    CompassView* getCompassView();

    bool writePrompt;

    static QStringList staticWindows;

public slots:
    void writeGameText(QByteArray, bool);
    void writeGameWindow(QByteArray);
    void logGameText(QByteArray, char type = '\0');

    void updateNavigationDisplay(DirectionsList);
    void updateMapWindow(QString hash);

    void registerStreamWindow(QString id, QString title);
    void removeStreamWindow(QString id);
    void writeStreamWindow(QString id, QString text);
    void clearStreamWindow(QString id);
    QList<QString> getStreamWindowNames();

    void reloadSettings();

    void lockWindows();
    void unlockWindows();

private slots:

private:
    GenericWindowFactory* genericWindowFactory;

    MainWindow* mainWindow;
    QPlainTextEdit* gameWindow;
    Compass* compass;
    GameDataContainer* gameDataContainer;
    ClientSettings* clientSettings;
    Highlighter* highlighter;
    HighlightSettings* settings;
    GeneralSettings* generalSettings;

    RoomWindow* roomWindow;
    ArrivalsWindow* arrivalsWindow;
    DeathsWindow* deathsWindow;
    ThoughtsWindow* thoughtsWindow;
    ExpWindow* expWindow;
    ConversationsWindow* conversationsWindow;
    FamiliarWindow* familiarWindow;
    SpellWindow* spellWindow;
    AtmosphericsWindow* atmosphericsWindow;

    QList<QDockWidget*> dockWindows;
    QHash<QString, QDockWidget*> streamWindows;

    CompassView* compassView;

    MapFacade* mapFacade;

    QRegExp rxRemoveTags;

    WindowWriterThread* mainWriter;
    QHash<QString, WindowWriterThread*> streamWriters;

    MainLogger* mainLogger;

    QString style;

    QString textColor(QString, QString);
    void setVisibilityIndicator(QDockWidget*, bool, QString);

signals:
    void updateWindowSettings();
};

#endif // WindowFacade_H
