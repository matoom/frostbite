#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <QObject>
#include <QDir>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <QDebug>
#include <QTextDocumentFragment>
#include <QApplication>

class WindowInterface;

class Snapshot : public QObject {
    Q_OBJECT

public:
    Snapshot(WindowInterface *window = 0);
    
    void save();

private:
    WindowInterface* window;

signals:
    
public slots:
    
};

#endif // SNAPSHOT_H
