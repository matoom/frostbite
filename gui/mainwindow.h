#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtXml>
#include <QReadWriteLock>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTextBrowser;
class QTextLine;
class QStringList;
class QVBoxLayout;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

class WindowFacade;
class TcpClient;
class Toolbar;
class GameWindow;
class CommandLine;
class MenuHandler;
class ScriptService;
class TimerBar;
class VitalsBar;
class GeneralSettings;
class Tray;
class ScriptApiServer;
class DictionaryService;
class ClientSettings;
class HyperlinkService;
class Session;
class ScriptStreamServer;
class GameWindow;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addDockWidgetMainWindow(Qt::DockWidgetArea, QDockWidget*);
    void removeDockWidgetMainWindow(QDockWidget* dock);
    void addWindowMenuAction(QAction* action);
    void addWidgetMainLayout(QWidget*);
    void addToolbarAction(QAction*);
    void addToolbarSeparator();
    void addToolbarSpacer(int);
    QAction* addToolbarWidget(QWidget*);
    void insertProfilesMenu(QMenu*);

    void setLogMain(bool);
    void setLogThoughts(bool);
    void setLogConversations(bool);
    void setLogArrivals(bool);
    void setLogDeaths(bool);
    void setLogDebug(bool);
    void setLogAuth(bool);

    void setWindowLocked(bool enabled);

    void setMenuWieldLeftVisible(bool enabled);
    void setMenuWieldRightVisible(bool enabled);
    void setMenuSpellVisible(bool enabled);
    void setMenuActiveSpellsVisible(bool enabled);
    void setMenuStatusVisible(bool enabled);
    void setMenuButtonsVisible(bool enabled);
    void setMenuVitalsVisible(bool enabled);
    void setMenuMutedVisible(bool enabled);
    void setFullScreenVisible(bool enabled);
    void setToolSize(QString size);

    void setToolbarAllowedAreas(Qt::ToolBarAreas);
    void toggleFullScreen();
    void toggleMaximized();
    void updateProfileSettings(QString, QString);        
    void openConnectDialog();
    void openConnection(QString host, QString port, QString key);
    void openLocalConnection(QString port);
    void openAppearanceDialog();
    void saveWindow();

    void restoreLayout(QString state);
    void saveLayout();

    void setCompassLocked(bool);
    void setCompassAnchored(bool);
    void setCompassVisible(bool);

    void reloadMaps();
    void showMaps();

    void enableMapsMenu(bool enabled);
    void enableConnectButton(bool enabled);

    void setToolbarScale(float scale);

    MenuHandler* getMenuHandler();
    WindowFacade* getWindowFacade();
    Toolbar* getToolbar();
    VitalsBar* getVitalsBar();
    TcpClient* getTcpClient();
    Session* getSession();
    CommandLine* getCommandLine();
    ScriptService* getScriptService();
    ScriptStreamServer* getScriptStreamServer();
    DictionaryService* getDictionaryService();
    TimerBar* getTimerBar();
    Tray* getTray();
    
public:
    static bool DEBUG;

private:
    Ui::MainWindow* ui;
    QWidget* mainWidget;
    QVBoxLayout* mainWidgetLayout;
    struct {
        Qt::WindowFlags mainWidgetWindowFlags;
        bool vitalsBarVisible;
        bool compassVisible;
    } distractionFreeModeParams;
    WindowFacade* windowFacade;
    Toolbar* toolBar;
    Session* session;
    ClientSettings* settings;
    GeneralSettings* generalSettings;
    CommandLine* cmdLine;
    MenuHandler* menuHandler;
    ScriptService* scriptService;
    ScriptApiServer* scriptApiServer;
    ScriptStreamServer* scriptStreamServer;
    DictionaryService* dictionaryService;
    HyperlinkService* hyperlinkService;
    QMenu* commandMenu;

    Tray* tray;
    TimerBar* timerBar;
    VitalsBar* vitalsBar;

    QReadWriteLock lock;

    void initSettings();
    void loadClient();
    void appSetup();

protected:
    void closeEvent(QCloseEvent*);

signals:
    void profileChanged();
    void volumeChanged(int);
    void volumeMuted(bool);
    void writeMainWindow(QByteArray);

public slots:
    void setMainTitle(QString);
    void handleAppMessage(const QString&);
    void updateScriptSettings();    
    void menuVolumeChanged(int);
    void menuVolumeMuted(bool);
    void reloadSettings();
    void actionCommand(const QString&);
    void actionCommands(const QStringList&);
    void toggleDistractionFreeMode();
};

#endif // MAINWINDOW_H
