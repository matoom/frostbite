#ifndef GRIDHIGHLIGHTERTHREAD_H
#define GRIDHIGHLIGHTERTHREAD_H

#include <QObject>
#include <QThread>

#include <mainwindow.h>

class Highlighter;

struct GridEntry {
    QString name;
    QString text;
};

typedef QMap<QString, QString> GridItems;

class GridWriterThread : public QThread {
    Q_OBJECT

public:
    explicit GridWriterThread(QObject *parent);
    ~GridWriterThread();

    virtual void run();

private:
    QQueue<GridEntry> dataQueue;
    Highlighter* highlighter;
    MainWindow* mainWindow;
    QMutex mMutex;
    bool append;
    QRegExp rxRemoveTags;
    GridEntry localData;
    QMap<QString, QString> highlightedItems;

    void process(GridEntry);

    bool exit;

public slots:
    void addItem(QString, QString);
    void updateSettings();

signals:
    void writeGrid(GridItems);

};

#endif // GRIDHIGHLIGHTERTHREAD_H
