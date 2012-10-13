#ifndef MACRO_H
#define MACRO_H

#include <QObject>
#include <QDebug>
#include <QStringList>

#include <commandline.h>
#include <mainwindow.h>

class CommandLine;
class MainWindow;

class Macro : public QObject {
    Q_OBJECT

public:
    explicit Macro(QObject *parent = 0);
    
    bool execute(QString);

private:
    MainWindow* mainWindow;

signals:
    
public slots:
    
};

#endif // MACRO_H
