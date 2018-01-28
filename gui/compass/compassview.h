#ifndef COMPASSWINDOW_H
#define COMPASSWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QApplication>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <compass.h>
#include <mainwindow.h>
#include <gamewindow.h>

class GameWindow;

class CompassView : public QWidget {
    Q_OBJECT

public:
    CompassView(QWidget *parent = 0);
    ~CompassView();

    void paint(Compass*);
    void gameWindowResizeEvent(GameWindow*);

private:
    bool locked;
    bool visible;
    bool anchored;
    QPoint oldPos;
    QLabel* imageLabel;
    ClientSettings* settings;
    MainWindow* mainWindow;
    QPixmap compass;

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

public slots:
    void setCompassLocked(bool locked);
    void setCompassAnchored(bool anchored);
    void setCompassVisible(bool visible);
    void resetCompass();
};

#endif // COMPASSWINDOW_H
