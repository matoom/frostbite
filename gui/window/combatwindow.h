#ifndef COMBATWINDOW_H
#define COMBATWINDOW_H

#include <QObject>
#include <QDockWidget>

class MainWindow;
class WindowFacade;
class GenericWindow;
class WindowWriterThread;

class CombatWindow : public QObject {
    Q_OBJECT
public:
    explicit CombatWindow(QObject *parent = 0);
    ~CombatWindow();

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

#endif // COMBATWINDOW_H
