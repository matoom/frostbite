#ifndef EXPWINDOW_H
#define EXPWINDOW_H

#include <QObject>
#include <QDockWidget>
#include <QTableWidget>
#include <QMap>
#include <QAction>
#include <textutils.h>

class MainWindow;
class WindowFacade;
class GridWindow;
class GridWriterThread;
class GameDataContainer;

typedef QMap<QString, QString> GridItems;

class GridWriterThread;

class ExpWindow : public QObject {
    Q_OBJECT

public:
    ExpWindow(QObject *parent = 0);
    ~ExpWindow();

    QDockWidget* getDockWidget();    

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GridWindow* window;
    GridWriterThread* writer;
    GameDataContainer* gameDataContainer;
    QAction* gainedAct;

    QDockWidget* dock;
    QTableWidget* table;

    bool showGained;

    void addContextMenu();
    void addGainedIndicator(QString key, QString &text);
    void writeRow(QString key, QString value, int row);
    void refresh();

public slots:
    void write(QString name, QString text);
    void writeExpWindow(GridItems items);
    void changeShowGained();
};

#endif // EXPWINDOW_H
