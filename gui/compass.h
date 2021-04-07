#ifndef COMPASS_H
#define COMPASS_H

#include <QObject>
#include <QtGui>

class WindowFacade;

class Compass : public QObject {
    Q_OBJECT

public:
    Compass(QObject *parent = 0);
    ~Compass();

    QPixmap paint();
    void paint(QWidget* widget, QRect);
    void updateState(QList<QString>);
    void setAutoPilot(bool);

private:
    WindowFacade* wm;

    QHash<bool, QList<QPixmap> > *images;

    QList<QPixmap> active;
    QList<QPixmap> inactive;

    QList<bool> state;
    QList<QString> dir;

    void loadImages();
};

#endif // COMPASS_H
