#ifndef MAPFACADE_H
#define MAPFACADE_H

#include <QObject>
#include <QtGui>

QT_BEGIN_NAMESPACE
class QDockWidget;
class QComboBox;
class QLabel;
QT_END_NAMESPACE

class MainWindow;
class MapReader;
class MapWindowFactory;
class MapDialog;
class MapData;
class MapWindow;
class MapZone;
class GeneralSettings;

class MapFacade : public QObject {
    Q_OBJECT

public:
    explicit MapFacade(MainWindow* parent);
    ~MapFacade();

    void init();

    MainWindow* getMainWindow();
    QDockWidget* getMapWindow();
    MapReader* getMapReader();

    MapData* getData();

private:
    MainWindow* mainWindow;
    MapWindowFactory* mapWindowFactory;
    MapReader* mapReader;

    QDockWidget* mapWindow;

    QComboBox* mapSelect;
    QComboBox* levelSelect;
    QLabel* mapIdLabel;
    QLabel* zoomLabel;
    MapWindow* mapView;

    MapData* mapData;

    MapDialog* mapDialog;

    GeneralSettings* settings;

    void moveSelected(QString zoneId, int nodeId, int level);
    void populateLevels(QString zoneId, int level = 0);
    void setSelected(QString zoneId, int level = 0);

    void setNotFoundMessage();

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
    void hideMapDialog();
    void setZoom(QString zoomLevel);
    void updateMapColors(QColor background);    
    void lockControls();
    void unlockControls();

};

#endif // MAPFACADE_H
