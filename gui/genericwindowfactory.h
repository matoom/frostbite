#ifndef WINDOWFACTORY_H
#define WINDOWFACTORY_H

#include <QObject>
#include <QDockWidget>
#include <QTextEdit>

#include <mainwindow.h>
#include <clientsettings.h>
#include <genericwindow.h>

class MainWindow;
class GenericWindow;

class GenericWindowFactory: public QObject {
    Q_OBJECT

public:
    GenericWindowFactory(QObject *parent = 0);

    QDockWidget* createWindow(const char*);

private:
    QPalette palette();
    QPlainTextEdit* textBox(QString);

    MainWindow* mainWindow;
    ClientSettings* settings;

private slots:
};

#endif // WINDOWFACTORY_H
