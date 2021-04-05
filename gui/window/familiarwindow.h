#ifndef FAMILIARWINDOW_H
#define FAMILIARWINDOW_H

#include <QObject>
#include <QDockWidget>

class MainWindow;
class WindowFacade;
class GenericWindow;
class WindowWriterThread;

class FamiliarWindow : public QObject{        
    Q_OBJECT

public:
    FamiliarWindow(QObject *parent = 0);
    ~FamiliarWindow();

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

#endif // FAMILIARWINDOW_H
