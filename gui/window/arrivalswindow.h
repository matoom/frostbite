#ifndef ARRIVALSWINDOW_H
#define ARRIVALSWINDOW_H

#include <QObject>
#include <mainwindow.h>
#include <windowfacade.h>

class ArrivalsWindow : public QObject {
    Q_OBJECT

public:
    ArrivalsWindow(QObject *parent = 0);
    ~ArrivalsWindow();

    QDockWidget* getDockWidget();

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GenericWindow* window;
    QDockWidget* dock;
    WindowWriterThread* writer;
    ArrivalsLogger* arrivalsLogger;
    ClientSettings* clientSettings;

    bool visible;

    void log(QString text);

public slots:
    void setVisible(bool);
    void write(QString text);
};

#endif // ARRIVALSWINDOW_H
