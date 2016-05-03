#include "mapdialog.h"
#include "ui_mapdialog.h"

MapDialog::MapDialog(MapFacade* mapFacade, QWidget *parent) : QDialog(parent), ui(new Ui::MapDialog) {
    ui->setupUi(this);

    this->mapFacade = mapFacade;

    connect(ui->mapSelect, SIGNAL(activated(int)), this, SLOT(mapSelected(int)));
    connect(ui->levelSelect, SIGNAL(activated(int)), this, SLOT(mapLevelSelected(int)));
    connect(ui->zoomIn, SIGNAL(pressed()), this, SLOT(zoomIn()));
    connect(ui->zoomOut, SIGNAL(pressed()), this, SLOT(zoomOut()));

    ui->mapView->scale(1.0, 1.0);

    ui->mapView->setObjectName("DialogMapView");
}

void MapDialog::scaleView(qreal scaleFactor) {
    qreal factor = ui->mapView->transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.5 || factor > 25) return;
    ui->mapView->scale(scaleFactor, scaleFactor);
}

void MapDialog::zoomIn() {
    scaleView(pow((double)2, 1 / 2.4));
}

void MapDialog::zoomOut() {
    scaleView(pow((double)2, -1 / 2.4));
}

void MapDialog::populate() {
    ui->mapSelect->addItem("");

    QMap<QString, MapZone*> zones = mapFacade->getMapReader()->getZones();

    QMap<QString, MapZone*>::iterator i;
    for (i = zones.begin(); i != zones.end(); ++i) {
        ui->mapSelect->addItem(i.key() + ": " + i.value()->getName(), i.key());
    }
}

void MapDialog::mapSelected(int index) {
    QString zoneId = ui->mapSelect->itemData(index).toString();

    QList<int>& levels = mapFacade->getMapReader()->getZones().value(zoneId)->getLevels();

    ui->levelSelect->clear();
    foreach(int level, levels) {
        ui->levelSelect->addItem(QString::number(level), level);
    }
    this->showMap(zoneId);
}

void MapDialog::mapLevelSelected(int index) {
    this->showMap(ui->mapSelect->currentData().toString(), ui->levelSelect->itemData(index).toInt());
}

void MapDialog::showMap(QString zoneId, int level) {
    int index = ui->mapSelect->findData(zoneId);
    if (index != -1) ui->mapSelect->setCurrentIndex(index);

    ui->nodeInfo->clear();

    QGraphicsScene* scene = mapFacade->getMapReader()->getScenes().value(zoneId).value(level).scene;
    ui->mapView->setScene(scene);
}

void MapDialog::setInfo(MapNode* node) {
    QString info;
    info += "<b>[" + QString::number(node->getId()) + "]</b> " + node->getName();
    if(!node->getNotes().isEmpty()) {
        info += " (" + node->getNotes().join(", ") + ")";
    }
    info += "<br/>";

    for(int i = 0; i < node->getDesc().size(); i++) {
        info += QString::number(i) + ") " + node->getDesc().value(i) + "<br/>";
    }
    info += "<b>Exits:</b> ";
    QList<MapDestination*> values = node->getDestinations().values();
    for (int i = 0; i < values.size(); ++i) {
        info += "[" + values.at(i)->getMove() + " -> " + QString::number(values.at(i)->getDestId()) + "]; ";
    }
    ui->nodeInfo->setText(info);
}

MapDialog::~MapDialog() {
    delete ui;
}
