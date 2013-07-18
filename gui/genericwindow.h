#ifndef GENERICWINDOW_H
#define GENERICWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QMouseEvent>

#include <generalsettings.h>
#include <mainwindow.h>
#include <windowinterface.h>
#include <snapshot.h>

class MainWindow;
class WindowManager;
class GeneralSettings;
class Snapshot;

class GenericWindow : public QPlainTextEdit, WindowInterface {
    Q_OBJECT

public:
    explicit GenericWindow(QWidget *parent = 0);
    ~GenericWindow();

    QColor getBgColor();
    QColor getTextColor();
    QTextDocument* getDocument();
    QString getObjectName();
    QPlainTextEdit* getMainWindow();

private:
    void contextMenuEvent(QContextMenuEvent* event);
    void buildContextMenu();
    void loadSettings();

    MainWindow* mainWindow;
    GeneralSettings* settings;
    WindowManager* wm;

    Snapshot* snapshot;

    QAction* copyAct;
    QAction* selectAct;
    QAction* saveAct;
    QAction* clearAct;
    QMenu* menu;

signals:

private slots:
    void copySelected();
    void enableCopy(bool);
    void saveAsHtml();
    
public slots:
    
};

#endif // GENERICWINDOW_H
