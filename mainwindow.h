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
    void setMainTitle(QString);

    WindowManager* getWindowManager();
    ToolbarManager* getToolbarManager();
    ConnectionManager* getConnectionManager();
    CommandLine* getCommandLine();
    ScriptService* getScriptService();

private:
    Ui::MainWindow *ui;
    WindowManager *wm;
    ToolbarManager *tbm;
    ConnectionManager *cm;    
    ClientSettings *settings;
    CommandLine *cmdLine;
    MenuHandler *menuHandler;
    ScriptService *scriptService;

    void initSettings();
    void loadClient();
    void appSetup();

protected:
    void closeEvent(QCloseEvent*);

/*private slots:
    void menuTriggered(QAction*);*/

};

#endif // MAINWINDOW_H
