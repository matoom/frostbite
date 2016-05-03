#ifndef MAPFACADE_H
#define MAPFACADE_H

#include <QObject>

#include <mainwindow.h>
#include <maps/mapwindowfactory.h>
#include <maps/mapwindow.h>
#include <maps/mapreader.h>
#include <maps/mapdialog.h>

class MainWindow;
class MapReader;
class MapWindowFactory;
class MapDialog;

class MapFacade : public QObject {
    Q_OBJECT

public:
    explicit MapFacade(MainWindow* parent);

    void init();

    MainWindow* getMainWindow();
    QDockWidget* getMapWindow();
    MapReader* getMapReader();

private:
    MainWindow* mainWindow;
    MapWindowFactory* mapWindowFactory;
    MapReader* mapReader;

    QDockWidget* mapWindow;

    QComboBox* mapSelect;
    QComboBox* levelSelect;
    QLabel* mapIdLabel;
    MapWindow* mapView;

    MapDialog* mapDialog;

    void moveSelected(QString zoneId, int nodeId, int level);

signals:    
    void nodeSelected(MapZone* zone, int nodeId);

public slots:
    void mapsReady();
    void mapSelected(int index);
    void mapLevelSelected(int index);
    void showMap(QWidget* widget, QString zoneId, int level = 0);
    void showMap(QString zoneId, int level = 0);
    void selectNode(QWidget* widget, QString zoneId, int level, int nodeId);
    void selectNode(QString zoneId, int level, int nodeId);
    void updateMapWindow(QString hash);
    void showMapDialog();

};

#endif // MAPFACADE_H
