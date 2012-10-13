#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QTextEdit>
#include <QMouseEvent>
#include <QResizeEvent>

#include <mainwindow.h>
#include <windowmanager.h>
#include <highlightsettings.h>
#include <defaultvalues.h>

class MainWindow;
class WindowManager;

class GameWindow : public QTextEdit {
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);

private:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent  *event);
    void wheelEvent(QWheelEvent  *event);

    void copySelected();

    MainWindow* mainWindow;
    WindowManager* windowManager;
    HighlightSettings* settings;

signals:

private slots:
    void adjustRowMargin();

};

#endif // GAMEWINDOW_H
