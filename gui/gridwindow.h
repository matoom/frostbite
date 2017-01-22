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

    QLabel* gridValueLabel(QWidget* parent, GeneralSettings* settings, QString key);

    void track(QString skillName, QWidget* widget);  

private:
    MainWindow* mainWindow;
    GeneralSettings* settings;
    WindowFacade* wm;

    QStringList tracked;

    void loadSettings();    
    void invertColors(QWidget* widget);

signals:

public slots:
    void addRemoveTracked(int, int);
    void updateSettings();
    void track(QString skillName);
    void clearTracked();
    void resize(int, int);

};

#endif // EXPWINDOW_H
