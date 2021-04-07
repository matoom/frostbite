#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QDebug>
#include <QWheelEvent>

class MapFacade;
class MapZone;
class ContextMenu;
class GeneralSettings;

class MapWindow : public QGraphicsView {
    Q_OBJECT

public:
    explicit MapWindow(MapFacade* parent);
    ~MapWindow();

private:
     void wheelEvent(QWheelEvent* zoomEvent);
    void contextMenuEvent(QContextMenuEvent* event);

    void scaleView(qreal);
    void buildContextMenu();

    void setLoadingMessage();

    MapFacade* mapFacade;
    QGraphicsEllipseItem* selected;

    QString zoneId;
    int level;

    ContextMenu* menu;

    GeneralSettings* settings;

signals:
    void updateMapWindow(QString);

public slots:
    void zoomIn();
    void zoomOut();
    void selectNode(MapZone* zone, int nodeId);
    void reset();
};

#endif // MAPWINDOW_H
