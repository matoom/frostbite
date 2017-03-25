#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include <QObject>
#include <QAction>
#include <QDebug>

#include <mainwindow.h>
#include <connectwizard.h>
#include <text/highlight/highlightdialog.h>
#include <macrodialog.h>
#include <text/alter/alterdialog.h>
#include <appearancedialog.h>
#include <aboutdialog.h>
#include <scripteditdialog.h>
#include <profileadddialog.h>
#include <clientsettings.h>

class ConnectWizard;
class MainWindow;
class HighlightDialog;
class AppearanceDialog;
class AlterDialog;
class AboutDialog;
class ScriptEditDialog;
class ProfileAddDialog;

#define USER_GUIDE_URL "http://matoom.github.com/frostbite"

class MenuHandler : public QObject {
    Q_OBJECT

public:
    MenuHandler(QObject *parent = 0);
    ~MenuHandler();

    void updateDialogSettings();
    void openConnectDialog();
    void openAppearanceDialog();

private:
    MainWindow* mainWindow;
    ClientSettings* clientSettings;

    ConnectWizard* connectWizard;
    HighlightDialog* highlightDialog;
    MacroDialog* macroDialog;
    AlterDialog* alterDialog;
    AppearanceDialog* appearanceDialog;
    AboutDialog* aboutDialog;
    ScriptEditDialog* scriptEditDialog;
    ProfileAddDialog* profileAddDialog;

    QMenu* profilesMenu;
    QAction* action;

    void loadLoggingMenu();
    void loadToolbarMenu();

signals:

public slots:
    void menuTriggered(QAction*);
    void menuHovered(QAction*);
    void profileTriggered(QAction* action);
    void loadProfilesMenu();    
};

#endif // MENUHANDLER_H
