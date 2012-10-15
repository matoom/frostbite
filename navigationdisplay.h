#ifndef NAVIGATIONDISPLAY_H
#define NAVIGATIONDISPLAY_H

#include <windowmanager.h>

#include <navigationdefines.h>

class WindowManager;

class NavigationDisplay : public QObject {
    Q_OBJECT

public:
    NavigationDisplay(QObject *parent = 0);
    ~NavigationDisplay();

    QPixmap paint();
    void updateState(QList<QString>);
    void setAutoPilot(bool);

private:
    WindowManager* wm;

    QHash<bool, QList<QPixmap> > *images;

    QList<QPixmap> active;
    QList<QPixmap> inactive;

    QList<bool> state;
    QList<QString> dir;

    void loadImages();
};

#endif // NAVIGATIONDISPLAY_H
