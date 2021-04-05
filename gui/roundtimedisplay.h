#ifndef ROUNDTIMEDISPLAY_H
#define ROUNDTIMEDISPLAY_H

#include <QObject>
#include <QtGui>

class MainWindow;
class GameDataContainer;
class GeneralSettings;

class RoundTimeDisplay : public QObject {
    Q_OBJECT

public:
    RoundTimeDisplay(QObject *parent);
    ~RoundTimeDisplay();

    void setRtColor(QColor color);
    void setCtColor(QColor color);

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

    GeneralSettings* settings;

    QColor rtColor;
    QColor ctColor;

    void initTimer();
    int toSeconds(int ms);

    void loadSettings();

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
