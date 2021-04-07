#ifndef ROOMWINDOW_H
#define ROOMWINDOW_H

#include <QObject>
#include <QDockWidget>

class MainWindow;
class WindowFacade;
class GenericWindow;
class WindowWriterThread;
class GameDataContainer;

class RoomWindow : public QObject {
    Q_OBJECT

public:
    RoomWindow(QObject *parent = 0);
    ~RoomWindow();

    QDockWidget* getDockWidget();

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GenericWindow* window;
    WindowWriterThread* writer;
    GameDataContainer* gameDataContainer;

    QDockWidget* dock;

public slots:
    void write();
    void setTitle(QString title);

};

#endif // ROOMWINDOW_H
