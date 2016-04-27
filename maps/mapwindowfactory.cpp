#include "mapwindowfactory.h"

MapWindowFactory::MapWindowFactory(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;
    mapWindow = new MapWindow(mainWindow);
}

QPushButton* MapWindowFactory::getZoomInButton(QString name) {
    QPushButton* button = new QPushButton(mainWindow);
    button->setObjectName(name + "ZoomIn");
    button->setMaximumWidth(20);
    button->setText("+");
    button->setContentsMargins(0, 0, 0, 0);

    connect(button, SIGNAL(pressed()), mapWindow, SLOT(zoomIn()));
    return button;
}

QPushButton* MapWindowFactory::getZoomOutButton(QString name) {
    QPushButton* button = new QPushButton(mainWindow);
    button->setObjectName(name + "ZoomOut");
    button->setMaximumWidth(20);
    button->setText("-");
    button->setContentsMargins(0, 0, 0, 0);
    connect(button, SIGNAL(pressed()), mapWindow, SLOT(zoomOut()));
    return button;
}

QComboBox* MapWindowFactory::getLevelSelect(QString name) {
   QComboBox* combo = new QComboBox(mainWindow);
   combo->setObjectName(name + "LevelSelect");
   combo->setDisabled(true);
   combo->setMinimumContentsLength(1);
   combo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
   connect(combo, SIGNAL(activated(int)),
           mainWindow->getWindowFacade(), SLOT(mapLevelSelected(int)));
   return combo;
}

QComboBox* MapWindowFactory::getMapSelect(QString name) {
   QComboBox* combo = new QComboBox(mainWindow);
   combo->setObjectName(name + "Select");
   combo->setDisabled(true);
   connect(combo, SIGNAL(activated(int)),
           mainWindow->getWindowFacade(), SLOT(mapSelected(int)));
   return combo;
}

QLabel* MapWindowFactory::getMapIdLabel(QString name) {
   QLabel* label = new QLabel(mainWindow);
   label->setObjectName(name + "IdLabel");
   label->setText("Id: -");
   return label;
}

QGraphicsView* MapWindowFactory::getView(QString name) {
    mapWindow->setObjectName(name + "View");    
    connect(mainWindow->getWindowFacade(), SIGNAL(nodeSelected(MapZone*, int)),
            mapWindow, SLOT(selectNode(MapZone*, int)));
    return (QGraphicsView*)mapWindow;
}

QDockWidget* MapWindowFactory::createWindow(const char* name) {
    QDockWidget *dock = new QDockWidget(QObject::tr(name), mainWindow);
    dock->setObjectName(QObject::tr(name) + "Window");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);

    QWidget* controls = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setMargin(0);
    hLayout->addWidget(getMapSelect(name));
    hLayout->addWidget(getLevelSelect(name));
    hLayout->addWidget(getMapIdLabel(name));
    hLayout->addWidget(getZoomInButton(name));
    hLayout->addWidget(getZoomOutButton(name));
    controls->setLayout(hLayout);

    QWidget* mapWindow = new QWidget();
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setMargin(0);
    vLayout->addWidget(controls);
    vLayout->addWidget(this->getView(name));
    mapWindow->setLayout(vLayout);

    dock->setWidget(mapWindow);

    return dock;
}
