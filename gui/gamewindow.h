#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QPlainTextEdit>
#include <QMouseEvent>
#include <QResizeEvent>

#include <mainwindow.h>
#include <windowmanager.h>
#include <clientsettings.h>
#include <defaultvalues.h>

class MainWindow;
class WindowManager;

class GameWindow : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

private:
    void contextMenuEvent(QContextMenuEvent* event);
    void resizeEvent(QResizeEvent* event);

    void loadSettings();
    void buildContextMenu();

    void showEvent(QShowEvent*);

    MainWindow* mainWindow;
    WindowManager* windowManager;
    ClientSettings* settings;

    QAction* copyAct;
    QAction* selectAct;
    QMenu* menu;

signals:

private slots:
    void adjustRowMargin();
    void copySelected();
    void enableCopy(bool);
};

#endif // GAMEWINDOW_H
