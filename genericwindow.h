#ifndef WINDOWFACTORY_H
#define WINDOWFACTORY_H

#include <QObject>
#include <QDockWidget>
#include <QTextEdit>

#include <mainwindow.h>

class MainWindow;

class GenericWindow: public QObject {
    Q_OBJECT

public:
    GenericWindow(QObject *parent = 0);

    QDockWidget* createWindow(const char*);

private:
    QPalette palette();
    QTextEdit* textBox(QDockWidget*);

    MainWindow *mw;

};

#endif // WINDOWFACTORY_H
