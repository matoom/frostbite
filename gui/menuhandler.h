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

class ConnectWizard;
class MainWindow;
class HighlightDialog;
class AppearanceDialog;
class AboutDialog;
class ScriptEditDialog;

class MenuHandler : public QObject {
    Q_OBJECT

public:
    MenuHandler(QObject *parent = 0);
    ~MenuHandler();

private:
    MainWindow* mainWindow;
    ConnectWizard* connectWizard;
    HighlightDialog* highlightDialog;
    MacroDialog* macroDialog;
    AppearanceDialog* appearanceDialog;
    AboutDialog* aboutDialog;
    ScriptEditDialog* scriptEditDialog;

signals:

public slots:
    void menuTriggered(QAction*);

};

#endif // MENUHANDLER_H
