#include "mapwindowfactory.h"

MapWindowFactory::MapWindowFactory(MapFacade *parent) : QObject(parent) {
    mapFacade = parent;
    mapWindow = new MapWindow(mapFacade);
}

QPushButton* MapWindowFactory::getResetButton(QWidget* parent, QString name) {
    QPushButton* button = new QPushButton(parent);
    button->setObjectName(name + "Reset");
    button->setMaximumWidth(15);
    button->setMaximumHeight(20);
    button->setText("R");
    button->setContentsMargins(0, 0, 0, 0);
    button->setFlat(false);
    button->setToolTip("Reset to current location");
    button->setCursor(Qt::PointingHandCursor);

    connect(button, SIGNAL(pressed()), mapWindow, SLOT(reset()));
    return button;
}


QPushButton* MapWindowFactory::getZoomInButton(QWidget* parent, QString name) {
    QPushButton* button = new QPushButton(parent);
    button->setObjectName(name + "ZoomIn");
    button->setMaximumWidth(15);
    button->setMaximumHeight(20);
    button->setText("+");
    button->setContentsMargins(0, 0, 0, 0);
    button->setFlat(false);
    button->setToolTip("Zoom in");
    button->setCursor(Qt::PointingHandCursor);

    connect(button, SIGNAL(pressed()), mapWindow, SLOT(zoomIn()));
    return button;
}

QPushButton* MapWindowFactory::getZoomOutButton(QWidget* parent, QString name) {
    QPushButton* button = new QPushButton(parent);
    button->setObjectName(name + "ZoomOut");
    button->setMaximumWidth(15);
    button->setMaximumHeight(20);
    button->setText("-");
    button->setContentsMargins(0, 0, 0, 0);
    button->setFlat(false);
    button->setToolTip("Zoom out");
    button->setCursor(Qt::PointingHandCursor);

    connect(button, SIGNAL(pressed()), mapWindow, SLOT(zoomOut()));
    return button;
}

QComboBox* MapWindowFactory::getLevelSelect(QWidget* parent, QString name) {
   QComboBox* combo = new QComboBox(parent);
   combo->setObjectName(name + "LevelSelect");
   combo->setDisabled(true);
   combo->setMinimumContentsLength(2);
   combo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
   combo->setToolTip("Map level");
   connect(combo, SIGNAL(activated(int)), mapFacade, SLOT(mapLevelSelected(int)));
   return combo;
}

QComboBox* MapWindowFactory::getMapSelect(QWidget* parent, QString name) {
   QComboBox* combo = new QComboBox(parent);
   combo->setObjectName(name + "Select");
   combo->setDisabled(true);
   combo->setMinimumWidth(150);
   connect(combo, SIGNAL(activated(int)), mapFacade, SLOT(mapSelected(int)));
   return combo;
}

QLabel* MapWindowFactory::getMapIdLabel(QWidget* parent, QString name) {
   QLabel* label = new QLabel(parent);
   label->setObjectName(name + "IdLabel");   
   label->setText("-");
   label->setToolTip("Room id");
   return label;
}

QGraphicsView* MapWindowFactory::getView(QString name) {
    mapWindow->setObjectName(name + "View");    
    connect(mapFacade, SIGNAL(nodeSelected(MapZone*, int)), mapWindow, SLOT(selectNode(MapZone*, int)));
    return (QGraphicsView*)mapWindow;
}

QDockWidget* MapWindowFactory::createWindow(const char* name) {
    QDockWidget* dock = new QDockWidget(QObject::tr(name), mapFacade->getMainWindow());
    dock->setObjectName(QObject::tr(name) + "Window");
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::TopDockWidgetArea);

    QWidget* controls = new QWidget(dock);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setMargin(0);
    hLayout->addWidget(getMapSelect(controls, name));
    hLayout->addWidget(getLevelSelect(controls, name));
    hLayout->addWidget(getMapIdLabel(controls, name));
    hLayout->addStretch();
    hLayout->addWidget(getResetButton(controls, name));
    hLayout->addWidget(getZoomOutButton(controls, name));
    hLayout->addWidget(getZoomInButton(controls, name));
    controls->setLayout(hLayout);

    QWidget* mapWindow = new QWidget(dock);
    QVBoxLayout* vLayout = new QVBoxLayout();
    vLayout->setMargin(0);
    vLayout->addWidget(controls);
    vLayout->addWidget(this->getView(name));
    mapWindow->setLayout(vLayout);

    dock->setWidget(mapWindow);

    return dock;
}
