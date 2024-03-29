#ifndef WindowFacade_H
#define WindowFacade_H

#include <QDockWidget>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include <QPlainTextEdit>

class MainWindow;
class GameWindow;
class GenericWindowFactory;
class NavigationDisplay;
class GameDataContainer;
class ClientSettings;
class GeneralSettings;
class Highlighter;
class HighlightSettings;
class MainLogger;
class MapFacade;
class CompassView;
class Compass;
class WindowWriterThread;

class RoomWindow;
class ArrivalsWindow;
class DeathsWindow;
class ThoughtsWindow;
class ExpWindow;
class ConversationsWindow;
class FamiliarWindow;
class SpellWindow;
class AtmosphericsWindow;
class GroupWindow;
class CombatWindow;
class DictionaryWindow;

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
    void reloadWindowSettings();

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
    GroupWindow* getGroupWindow();
    CombatWindow* getCombatWindow();
    DictionaryWindow* getDictionaryWindow();
    
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
    GroupWindow* groupWindow;
    CombatWindow* combatWindow;
    DictionaryWindow* dictionaryWindow;

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
    QString bgColor(QString);
    void setVisibilityIndicator(QDockWidget*, bool, QString);

signals:
    void updateWindowSettings();
};

#endif // WindowFacade_H
