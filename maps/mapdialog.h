#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDialog>

#include <maps/mapzone.h>
#include <maps/mapfacade.h>
#include <maps/mapreader.h>

class MapReader;

namespace Ui {
    class MapDialog;
}

class MapDialog : public QDialog {
    Q_OBJECT

public:
    explicit MapDialog(MapFacade* mapFacade, QWidget *parent = 0);
    ~MapDialog();

    void populate();
    void setInfo(MapNode* node);
    void showMap(QString zoneId, int level = 0);

private:
    Ui::MapDialog *ui;

    MapFacade* mapFacade;

    void scaleView(qreal scaleFactor);

public slots:
    void mapSelected(int index);
    void mapLevelSelected(int index);

    void zoomIn();
    void zoomOut();
};

#endif // MAPDIALOG_H
