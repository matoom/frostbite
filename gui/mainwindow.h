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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addDockWidgetMainWindow(Qt::DockWidgetArea, QDockWidget*);
    void addWidgetMainLayout(QWidget*);
    void addToolbarAction(QAction*);
    void addToolbarSeparator();
    void addToolbarSpacer(int);
    void addToolbarWidget(QWidget*);
    void setToolbarAllowedAreas(Qt::ToolBarAreas);
    void setMainTitle(QString);
    void connectEnabled(bool);
    void setBackgroundColor(QColor);
    void toggleFullScreen();
    void toggleMaximized();

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
    CommandLine* cmdLine;
    MenuHandler* menuHandler;
    ScriptService* scriptService;
    TimerBar* timerBar;

    void initSettings();
    void loadClient();
    void appSetup();

protected:
    void closeEvent(QCloseEvent*);

/*private slots:
    void menuTriggered(QAction*);*/

};

#endif // MAINWINDOW_H
