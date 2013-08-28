#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QPlainTextEdit>
#include <QMouseEvent>
#include <QResizeEvent>

#include <mainwindow.h>
#include <windowmanager.h>
#include <generalsettings.h>
#include <defaultvalues.h>
#include <windowinterface.h>
#include <snapshot.h>

class MainWindow;
class WindowManager;
class Snapshot;

class GameWindow : public QPlainTextEdit, public WindowInterface {
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0);
    ~GameWindow();

    QColor getBgColor();
    QColor getTextColor();
    QTextDocument* getDocument();
    QString getObjectName();
    QPlainTextEdit* getMainWindow();

    void setAppend(bool append);
    bool append();

private:
    void contextMenuEvent(QContextMenuEvent* event);
    void resizeEvent(QResizeEvent* event);

    void loadSettings();
    void buildContextMenu();

    void showEvent(QShowEvent*);

    MainWindow* mainWindow;
    WindowManager* windowManager;
    GeneralSettings* settings;

    Snapshot* snapshot;

    QAction* copyAct;
    QAction* selectAct;
    QAction* clearAct;
    QAction* saveAct;
    QMenu* menu;

    bool _append;

signals:    

private slots:
    void copySelected();
    void enableCopy(bool);
    void saveAsHtml();

public slots:

};

#endif // GAMEWINDOW_H
