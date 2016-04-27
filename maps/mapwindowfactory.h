#ifndef MAPWINDOWFACTORY_H
#define MAPWINDOWFACTORY_H

#include <QWidget>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include <mainwindow.h>
#include <maps/mapwindow.h>

class MainWindow;
class MapWindow;

class MapWindowFactory : public QObject {
    Q_OBJECT

public:
    explicit MapWindowFactory(QObject *parent = 0);

    QDockWidget* createWindow(const char* name);

private:    
    QGraphicsView* getView(QString name);
    QComboBox* getLevelSelect(QString name);
    QComboBox* getMapSelect(QString name);
    QLabel* getMapIdLabel(QString name);        
    QPushButton* getZoomInButton(QString name);
    QPushButton* getZoomOutButton(QString name);

    MainWindow* mainWindow;
    MapWindow* mapWindow;
};

#endif // MAPWINDOWFACTORY_H
