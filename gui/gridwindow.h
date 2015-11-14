#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <QTableWidget>
#include <QDockWidget>

#include <mainwindow.h>

class GridWindow : public QTableWidget {
    Q_OBJECT

public:
    explicit GridWindow(QWidget *parent = 0);

    QColor getBgColor();
    QColor getTextColor();

private:
    MainWindow* mainWindow;
    GeneralSettings* settings;
    WindowFacade* wm;

    void loadSettings();

signals:    

public slots:
};

#endif // EXPWINDOW_H
