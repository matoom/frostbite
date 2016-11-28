#ifndef ROUNDTIMEDISPLAY_H
#define ROUNDTIMEDISPLAY_H

#include <mainwindow.h>
#include <gamedatacontainer.h>

class MainWindow;
class GameDataContainer;

class RoundTimeDisplay : public QObject {
    Q_OBJECT

public:
    RoundTimeDisplay(QObject *parent);
    ~RoundTimeDisplay();

    void repaint();
    QTimer *timer;

private:
    static const int RT_SEGMENT_WIDTH = 30;

    MainWindow* mainWindow;
    GameDataContainer* gameDataContainer;

    int roundTime;
    int castTime;

    QPixmap segmentDisplay(int rt, int ct);
    QPixmap numericDisplay(int seconds);

    QColor getRtColor();
    QColor getCtColor();

    GeneralSettings* settings;

    QColor rtColor;
    QColor ctColor;

signals:
    void callPaint(int, int);

public slots:
    void setTimer(int seconds);
    void setCastTimer(int seconds);
    void paint(int, int);

private slots:
    void intervalEvent();
    void reloadSettings();
};

#endif // ROUNDTIMEDISPLAY_H
