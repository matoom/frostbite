#ifndef ATMOSPHERICSWINDOW_H
#define ATMOSPHERICSWINDOW_H

#include <QObject>
#include <QDockWidget>

class MainWindow;
class WindowFacade;
class GenericWindow;
class WindowWriterThread;

class AtmosphericsWindow : public QObject {
    Q_OBJECT

public:
    AtmosphericsWindow(QObject *parent = 0);
    ~AtmosphericsWindow();

    QDockWidget* getDockWidget();

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GenericWindow* window;
    QDockWidget* dock;
    WindowWriterThread* writer;

    bool visible;

public slots:
    void setVisible(bool);
    void write(QString text);

};

#endif // ATMOSPHERICSWINDOW_H
