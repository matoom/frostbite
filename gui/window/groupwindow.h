#ifndef GROUPWINDOW_H
#define GROUPWINDOW_H

#include <QObject>
#include <mainwindow.h>
#include <windowfacade.h>

class GroupWindow : public QObject {
    Q_OBJECT

public:
    GroupWindow(QObject *parent = 0);
    ~GroupWindow();

    QDockWidget* getDockWidget();

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GenericWindow* window;
    WindowWriterThread* writer;

    QDockWidget* dock;

    bool visible;

public slots:
    void setVisible(bool);
    void write(QString text);

};

#endif // GROUPWINDOW_H
