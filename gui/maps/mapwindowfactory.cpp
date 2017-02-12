#include "mapwindowfactory.h"

MapWindowFactory::MapWindowFactory(MapFacade *parent) : QObject(parent) {
    mapFacade = parent;
    mapWindow = new MapWindow(mapFacade);

    settings = new GeneralSettings();
}

QPalette MapWindowFactory::palette() {
    QPalette palette = QPalette();

    QColor textBackground = settings->dockWindowBackground();
    palette.setColor(QPalette::Base, textBackground);

    return palette;
}

QPushButton* MapWindowFactory::createResetButton(QWidget* parent, QString name) {
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

QPushButton* MapWindowFactory::createZoomInButton(QWidget* parent, QString name) {
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

QPushButton* MapWindowFactory::createZoomOutButton(QWidget* parent, QString name) {
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

QComboBox* MapWindowFactory::createLevelSelect(QWidget* parent, QString name) {
   QComboBox* combo = new QComboBox(parent);
   combo->setObjectName(name + "LevelSelect");
   combo->setDisabled(true);
   combo->setMinimumContentsLength(2);
   combo->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
   combo->setToolTip("Map level");
   connect(combo, SIGNAL(activated(int)), mapFacade, SLOT(mapLevelSelected(int)));
   return combo;
}

QComboBox* MapWindowFactory::createMapSelect(QWidget* parent, QString name) {
   QComboBox* combo = new QComboBox(parent);
   combo->setObjectName(name + "Select");
   combo->setDisabled(true);
   combo->setMinimumWidth(150);
   connect(combo, SIGNAL(activated(int)), mapFacade, SLOT(mapSelected(int)));
   return combo;
}

QLabel* MapWindowFactory::createMapIdLabel(QWidget* parent, QString name) {
   QLabel* label = new QLabel(parent);
   label->setObjectName(name + "IdLabel");   
   label->setText("-");
   label->setToolTip("Room id");
   return label;
}

QLabel* MapWindowFactory::createZoomLabel(QWidget* parent, QString name) {
    QLabel* label = new QLabel(parent);
    label->setObjectName(name + "ZoomLabel");
    label->setText("1x");
    label->setToolTip("Zoom level");
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

    dock->setPalette(this->palette());

    QWidget* controls = new QWidget(dock);
    QHBoxLayout *hLayout = new QHBoxLayout();
    hLayout->setMargin(0);
    hLayout->addWidget(createMapSelect(controls, name));
    hLayout->addWidget(createLevelSelect(controls, name));
    hLayout->addWidget(createMapIdLabel(controls, name));
    hLayout->addStretch();        
    hLayout->addWidget(createZoomLabel(controls, name));
    hLayout->addWidget(createResetButton(controls, name));
    hLayout->addWidget(createZoomOutButton(controls, name));
    hLayout->addWidget(createZoomInButton(controls, name));
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
