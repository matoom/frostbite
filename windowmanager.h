#ifndef WINDOWMANAGER_H
#define WINDOWMANAGER_H

#include <QDockWidget>

#include <mainwindow.h>
#include <genericwindow.h>
#include <gamewindow.h>
#include <navigationdisplay.h>

class MainWindow;
class GameWindow;
class GenericWindow;
class NavigationDisplay;

class WindowManager : public QObject {
    Q_OBJECT

public:
    WindowManager(QObject *parent = 0);
    ~WindowManager();

    void loadWindows();
    QTextEdit* getGameWindow();
    void paintNavigationDisplay();
    void writePromptGameWindow(QByteArray);
    void writeGameWindow(QByteArray);
    void updateNavigationDisplay(QList<QString>);

public slots:


private:
    GenericWindow *genericWindow;
    MainWindow *mainWindow;
    QTextEdit *gameWindow;
    NavigationDisplay *navigationDisplay;

    QDockWidget *room;
    QDockWidget *arrival;
    QDockWidget *thoughts;
};

#endif // WINDOWMANAGER_H
