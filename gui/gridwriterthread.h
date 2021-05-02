#ifndef GRIDHIGHLIGHTERTHREAD_H
#define GRIDHIGHLIGHTERTHREAD_H

#include <QString>
#include <QMap>
#include "workqueuethread.h"

class Highlighter;
class GridWindow;
class MainWindow;
class Alter;

struct GridEntry {
    QString name;
    QString text;
};

typedef QMap<QString, QString> GridItems;

class GridWriterThread : public WorkQueueThread<GridEntry> {
    Q_OBJECT
    using Parent = WorkQueueThread<GridEntry>;
public:
    explicit GridWriterThread(QObject *parent, GridWindow* window);
    ~GridWriterThread() = default;

protected:
    void onProcess(const GridEntry& data) override;
    
private:
    Highlighter* highlighter;
    Alter* alter;

    MainWindow* mainWindow;
    bool append;
    QRegExp rxRemoveTags;
    QMap<QString, QString> highlightedItems;

    GridWindow* window;

    void write(GridEntry);
    QString process(QString text, QString win);

public slots:
    void addItem(QString, QString);
    void updateSettings();

signals:
    void writeGrid(GridItems);

};

#endif // GRIDHIGHLIGHTERTHREAD_H
