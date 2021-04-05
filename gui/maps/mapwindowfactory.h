#ifndef MAPWINDOWFACTORY_H
#define MAPWINDOWFACTORY_H

#include <QWidget>
#include <QDockWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QGraphicsView>

class MapFacade;
class MapWindow;
class GeneralSettings;

class MapWindowFactory : public QObject {
    Q_OBJECT

public:
    explicit MapWindowFactory(MapFacade* parent);
    ~MapWindowFactory();

    QDockWidget* createWindow(const char* name);

private:    
    QGraphicsView* getView(QString name);
    QComboBox* createLevelSelect(QWidget* parent, QString name);
    QComboBox* createMapSelect(QWidget* parent, QString name);
    QLabel* createMapIdLabel(QWidget* parent, QString name);
    QLabel* createZoomLabel(QWidget* parent, QString name);
    QPushButton* createResetButton(QWidget* parent, QString name);
    QPushButton* createZoomInButton(QWidget* parent, QString name);
    QPushButton* createZoomOutButton(QWidget* parent, QString name);

    QPalette palette();

    MapFacade* mapFacade;
    MapWindow* mapWindow;

    GeneralSettings* settings;
};

#endif // MAPWINDOWFACTORY_H
