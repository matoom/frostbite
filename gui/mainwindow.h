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


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static bool DEBUG;

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

    void setToolbarAllowedAreas(Qt::ToolBarAreas);
    void connectEnabled(bool);    
    void toggleFullScreen();
    void toggleMaximized();
    void updateProfileSettings(QString, QString);        
    void openConnectDialog();
    void openConnection(QString host, QString port, QString key);
    void openLocalConnection(QString port);
    void openAppearanceDialog();
    void saveWindow();

    void setCompassLocked(bool);
    void setCompassAnchored(bool);
    void setCompassVisible(bool);

    void reloadMaps();
    void showMaps();

    void enableMapsMenu(bool enabled);

    MenuHandler* getMenuHandler();
    WindowFacade* getWindowFacade();
    Toolbar* getToolbar();
    VitalsBar* getVitalsBar();
    TcpClient* getTcpClient();
    CommandLine* getCommandLine();
    ScriptService* getScriptService();
    DictionaryService* getDictionaryService();
    TimerBar* getTimerBar();
    Tray* getTray();

private:
    Ui::MainWindow* ui;
    WindowFacade* windowFacade;
    Toolbar* toolBar;
    TcpClient* tcpClient;
    ClientSettings* settings;
    GeneralSettings* generalSettings;
    CommandLine* cmdLine;
    MenuHandler* menuHandler;
    ScriptService* scriptService;
    ScriptApiServer* scriptApiServer;
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

public slots:
    void setMainTitle(QString);
    void handleAppMessage(const QString&);
    void updateScriptSettings();    
    void menuVolumeChanged(int);
    void menuVolumeMuted(bool);
    void reloadSettings();
    void actionCommand(const QString&);
    void actionCommands(const QStringList&);
};

#endif // MAINWINDOW_H
