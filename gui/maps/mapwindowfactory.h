#ifndef MAPWINDOWFACTORY_H
#define MAPWINDOWFACTORY_H

#include <QWidget>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

#include <mainwindow.h>
#include <maps/mapwindow.h>

class MapWindow;

class MapWindowFactory : public QObject {
    Q_OBJECT

public:
    explicit MapWindowFactory(MapFacade* parent);

    QDockWidget* createWindow(const char* name);

private:    
    QGraphicsView* getView(QString name);
    QComboBox* getLevelSelect(QWidget* parent, QString name);
    QComboBox* getMapSelect(QWidget* parent, QString name);
    QLabel* getMapIdLabel(QWidget* parent, QString name);
    QPushButton* getResetButton(QWidget* parent, QString name);
    QPushButton* getZoomInButton(QWidget* parent, QString name);
    QPushButton* getZoomOutButton(QWidget* parent, QString name);

    MapFacade* mapFacade;
    MapWindow* mapWindow;
};

#endif // MAPWINDOWFACTORY_H
