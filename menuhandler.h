#ifndef MENUHANDLER_H
#define MENUHANDLER_H

#include <QObject>
#include <QAction>
#include <QDebug>

#include <mainwindow.h>
#include <connectdialog.h>
#include <highlightdialog.h>
#include <macrodialog.h>

class ConnectDialog;
class MainWindow;
class HighlightDialog;

class MenuHandler : public QObject {
    Q_OBJECT

public:
    MenuHandler(QObject *parent = 0);
    ~MenuHandler();

private:
    MainWindow *mainWindow;
    ConnectDialog *connectDialog;
    HighlightDialog *highlightDialog;
    MacroDialog* macroDialog;

signals:

public slots:
    void menuTriggered(QAction*);

};

#endif // MENUHANDLER_H
