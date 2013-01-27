#ifndef GENERICWINDOW_H
#define GENERICWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QMouseEvent>
#include <generalsettings.h>

#include <mainwindow.h>

class MainWindow;
class GeneralSettings;

class GenericWindow : public QPlainTextEdit {
    Q_OBJECT

public:
    explicit GenericWindow(QWidget *parent = 0);
    ~GenericWindow();

private:
    void contextMenuEvent(QContextMenuEvent* event);
    void buildContextMenu();
    void loadSettings();

    MainWindow* mainWindow;
    GeneralSettings* settings;

    QAction* copyAct;
    QAction* selectAct;
    QAction* clearAct;
    QMenu* menu;

signals:

private slots:
    void copySelected();
    void enableCopy(bool);
    
public slots:
    
};

#endif // GENERICWINDOW_H
