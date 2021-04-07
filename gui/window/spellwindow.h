#ifndef SPELLWINDOW_H
#define SPELLWINDOW_H

#include <QObject>
#include <QDockWidget>

class MainWindow;
class WindowFacade;
class GenericWindow;
class WindowWriterThread;

class SpellWindow : public QObject {
    Q_OBJECT

public:
    SpellWindow(QObject *parent = 0);
    ~SpellWindow();

    QDockWidget* getDockWidget();

private:
    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    GenericWindow* window;
    WindowWriterThread* writer;

    QDockWidget* dock;

public slots:
   void write(QString text);

};

#endif // SPELLWINDOW_H
