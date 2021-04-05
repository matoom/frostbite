#ifndef CONVERSATIONSWINDOW_H
#define CONVERSATIONSWINDOW_H

#include <QObject>
#include <QDockWidget>

class MainWindow;
class WindowFacade;
class GenericWindow;
class WindowWriterThread;
class ConversationsLogger;
class ClientSettings;

class ConversationsWindow : public QObject {
    Q_OBJECT

public:
    ConversationsWindow(QObject *parent = 0);
    ~ConversationsWindow();

    QDockWidget* getDockWidget();

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GenericWindow* window;
    QDockWidget* dock;
    WindowWriterThread* writer;
    ConversationsLogger* conversationsLogger;
    ClientSettings* clientSettings;

    bool visible;

    void log(QString text);

public slots:
    void setVisible(bool);
    void write(QString text);

};

#endif // CONVERSATIONSWINDOW_H
