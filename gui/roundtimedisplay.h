#ifndef ROUNDTIMEDISPLAY_H
#define ROUNDTIMEDISPLAY_H

#include <mainwindow.h>
#include <gamedatacontainer.h>

#include <globaldefines.h>

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
    static const int RT_INTERVAL_MS = 200;

    MainWindow* mainWindow;
    GameDataContainer* data;

    int roundTime;
    int castTime;

    QPixmap segmentDisplay(int rt, int ct);
    QPixmap numericDisplay(int seconds);

    QColor getRtColor();
    QColor getCtColor();

    GeneralSettings* settings;

    QColor rtColor;
    QColor ctColor;

    void initTimer();
    int toSeconds(int ms);

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
