#ifndef MAPDIALOG_H
#define MAPDIALOG_H

#include <QDialog>

class MapReader;
class GeneralSettings;
class MapFacade;
class MapNode;
class RoomNode;

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
    void populateLevels(QString zoneId, int level = 0);
    void setSelected(QString zoneId, int level = 0);

    void center(const RoomNode& roomNode);

private:
    Ui::MapDialog *ui;

    MapFacade* mapFacade;

    GeneralSettings* settings;

    void scaleView(qreal scaleFactor);

public slots:
    void setBackgroundBrush(QBrush);
    void mapSelected(int index);
    void mapLevelSelected(int index);    

    void zoomIn();
    void zoomOut();
    void reset();

    void lockControls();
    void unlockControls();
};

#endif // MAPDIALOG_H
