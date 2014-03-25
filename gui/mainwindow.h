#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

#include <windowmanager.h>
#include <connectionmanager.h>
#include <toolbarmanager.h>
#include <clientsettings.h>
#include <commandline.h>
#include <menuhandler.h>
#include <scriptservice.h>
#include <timerbar.h>
#include <generalsettings.h>

#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#include "cleanlooks/qcleanlooksstyle.h"
#endif

/* test*/
#include <QtXml>

QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QTextBrowser;
class QTextLine;
QT_END_NAMESPACE

namespace Ui {
    class MainWindow;
}

class WindowManager;
class ConnectionManager;
class ToolbarManager;
class GameWindow;
class CommandLine;
class MenuHandler;
class ScriptService;
class TimerBar;
class GeneralSettings;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static bool DEBUG;

    void addDockWidgetMainWindow(Qt::DockWidgetArea, QDockWidget*);
    void addWidgetMainLayout(QWidget*);
    void addToolbarAction(QAction*);
    void addToolbarSeparator();
    void addToolbarSpacer(int);
    void addToolbarWidget(QWidget*);
    void insertProfilesMenu(QMenu*);
    void setLogMain(bool);
    void setLogThoughts(bool);
    void setLogConversations(bool);
    void setLogArrivals(bool);
    void setLogDeaths(bool);
    void setLogDebug(bool);
    void setToolbarAllowedAreas(Qt::ToolBarAreas);
    void connectEnabled(bool);
    void setBackgroundColor(QColor);
    QColor getBackgroundColor();
    void toggleFullScreen();
    void toggleMaximized();
    void updateProfileSettings();
    void openConnectDialog();

    WindowManager* getWindowManager();
    ToolbarManager* getToolbarManager();
    ConnectionManager* getConnectionManager();
    CommandLine* getCommandLine();
    ScriptService* getScriptService();
    TimerBar* getTimerBar();

private:
    Ui::MainWindow* ui;
    WindowManager* windowManager;
    ToolbarManager* tbm;
    ConnectionManager* cm;
    ClientSettings* settings;
    GeneralSettings* generalSettings;
    CommandLine* cmdLine;
    MenuHandler* menuHandler;
    ScriptService* scriptService;
    TimerBar* timerBar;
    QReadWriteLock lock;

    void initSettings();
    void loadClient();
    void appSetup();

protected:
    void closeEvent(QCloseEvent*);

public slots:
    void setMainTitle(QString);

/*private slots:
    void menuTriggered(QAction*);*/

};

#endif // MAINWINDOW_H
