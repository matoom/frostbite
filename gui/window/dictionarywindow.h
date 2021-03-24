#ifndef DICTIONARYWINDOW_H
#define DICTIONARYWINDOW_H

#include <QObject>
#include <mainwindow.h>
#include <windowfacade.h>

class DictionaryWindow : public QObject {
    Q_OBJECT

public:
    DictionaryWindow(QObject *parent = 0);
    ~DictionaryWindow();

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

#endif // DICTIONARYWINDOW_H
