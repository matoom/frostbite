#ifndef MAPWINDOW_H
#define MAPWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QDebug>
#include <QWheelEvent>

#include <maps/mapzone.h>
#include <mainwindow.h>

class MainWindow;

class MapWindow : public QGraphicsView {
    Q_OBJECT

public:
    explicit MapWindow(QWidget *parent = 0);

    void setScene(QString zoneId, int level, QGraphicsScene *scene);

private:
    void wheelEvent(QWheelEvent* zoomEvent);
    void scaleView(qreal scaleFactor);

    MainWindow* mainWindow;
    QGraphicsEllipseItem* selected;

    QString zoneId;
    int level;

signals:

public slots:
    void zoomIn();
    void zoomOut();
    void selectNode(MapZone* zone, int nodeId);
};

#endif // MAPWINDOW_H
