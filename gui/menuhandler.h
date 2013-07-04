#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include <QObject>
#include <QAction>
#include <QDebug>

#include <mainwindow.h>
#include <connectwizard.h>
#include <highlightdialog.h>
#include <macrodialog.h>
#include <appearancedialog.h>
#include <aboutdialog.h>
#include <scripteditdialog.h>
#include <profileadddialog.h>
#include <clientsettings.h>

class ConnectWizard;
class MainWindow;
class HighlightDialog;
class AppearanceDialog;
class AboutDialog;
class ScriptEditDialog;
class ProfileAddDialog;

class MenuHandler : public QObject {
    Q_OBJECT

public:
    MenuHandler(QObject *parent = 0);
    ~MenuHandler();

    void updateDialogSettings();
    void openConnectDialog();

private:
    MainWindow* mainWindow;
    ClientSettings* clientSettings;

    ConnectWizard* connectWizard;
    HighlightDialog* highlightDialog;
    MacroDialog* macroDialog;
    AppearanceDialog* appearanceDialog;
    AboutDialog* aboutDialog;
    ScriptEditDialog* scriptEditDialog;
    ProfileAddDialog* profileAddDialog;

    QMenu* profilesMenu;
    QAction* action;

    void loadLoggingMenu();

signals:

public slots:
    void menuTriggered(QAction*);
    void menuHovered(QAction*);
    void loadProfilesMenu();

};

#endif // MENUHANDLER_H
