#ifndef GRIDHIGHLIGHTERTHREAD_H
#define GRIDHIGHLIGHTERTHREAD_H

#include <QObject>
#include <QThread>

#include <mainwindow.h>
#include <text/alter/alter.h>

class Highlighter;
class GridWindow;

struct GridEntry {
    QString name;
    QString text;
};

typedef QMap<QString, QString> GridItems;

class GridWriterThread : public QThread {
    Q_OBJECT

public:
    explicit GridWriterThread(QObject *parent, GridWindow* window);
    ~GridWriterThread();

    virtual void run();

private:
    QQueue<GridEntry> dataQueue;

    Highlighter* highlighter;
    Alter* alter;

    MainWindow* mainWindow;
    QMutex mMutex;
    bool append;
    QRegExp rxRemoveTags;
    GridEntry localData;
    QMap<QString, QString> highlightedItems;

    GridWindow* window;

    void write(GridEntry);
    QString process(QString text, QString win);

    bool exit;

public slots:
    void addItem(QString, QString);
    void updateSettings();

signals:
    void writeGrid(GridItems);

};

#endif // GRIDHIGHLIGHTERTHREAD_H
