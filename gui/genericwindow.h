#ifndef GENERICWINDOW_H
#define GENERICWINDOW_H

#include <QObject>
#include <QPlainTextEdit>
#include <QMouseEvent>
#include <clientsettings.h>

#include <mainwindow.h>

class MainWindow;
class ClientSettings;

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
    ClientSettings* settings;

    QAction* copyAct;
    QAction* selectAct;
    QMenu* menu;

signals:

private slots:
    void copySelected();
    void enableCopy(bool);
    
public slots:
    
};

#endif // GENERICWINDOW_H
