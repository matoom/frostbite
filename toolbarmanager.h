#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QObject>

#include <mainwindow.h>
#include <vitalsindicator.h>
#include <statusindicator.h>
#include <wieldindicator.h>
#include <quickbuttondisplay.h>

class MainWindow;
class QuickButtonDisplay;

class ToolbarManager : public QObject {
    Q_OBJECT

public:
    ToolbarManager(QObject *parent = 0);
    ~ToolbarManager();

    void loadToolbar();
    void updateVitals(QString name, QString value);    
    void updateStatus(QString visible, QString icon);
    void updateWieldLeft(QString value);
    void updateWieldRight(QString value);

    int getHealthValue();
    int getConcentrationValue();
    int getFatigueValue();
    int getSpiritValue();

private:
    MainWindow *mainWindow;
    VitalsIndicator *vitalsIndicator;
    StatusIndicator *statusIndicator;
    QuickButtonDisplay *quickButtonDisplay;
    WieldIndicator *wieldLeft;
    WieldIndicator *wieldRight;

public slots:
    void quickButtonAction();

};

#endif // TOOLBARMANAGER_H
