#ifndef THOUGHTSWINDOW_H
#define THOUGHTSWINDOW_H

#include <QObject>
#include <mainwindow.h>
#include <windowfacade.h>

class ThoughtsWindow : public QObject {
    Q_OBJECT

public:
    ThoughtsWindow(QObject *parent = 0);
    ~ThoughtsWindow();

    QDockWidget* getDockWidget();

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GenericWindow* window;
    QDockWidget* dock;
    WindowWriterThread* writer;
    ThoughtsLogger* thoughtsLogger;
    ClientSettings* clientSettings;

    bool visible;

    void log(QString text);

public slots:
    void setVisible(bool);
    void write(QString text);

};

#endif // THOUGHTSWINDOW_H
