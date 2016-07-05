#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H

#include <QTableWidget>
#include <QDockWidget>
#include <QLabel>

#include <mainwindow.h>

class GridWindow : public QTableWidget {
    Q_OBJECT

public:
    explicit GridWindow(QWidget *parent = 0);

    QColor getBgColor();
    QColor getTextColor();

    QLabel* ceateGridItem(QWidget* parent, QString key, QString style);

    void track(QString skillName, QWidget* widget);
    void clearTracked();

private:
    MainWindow* mainWindow;
    GeneralSettings* settings;
    WindowFacade* wm;

    QStringList tracked;

    void loadSettings();    

    //void mouseReleaseEvent(QMouseEvent* event);
    //bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:
    void addTrackedItem(int, int);
};

#endif // EXPWINDOW_H
