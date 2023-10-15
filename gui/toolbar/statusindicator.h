#ifndef STATUSINDICATOR_H
#define STATUSINDICATOR_H

#include <QObject>
#include <QtGui>
#include <QLabel>
#include <QHBoxLayout>

#define T_STATUS_W 32
#define T_STATUS_H 32

class GameDataContainer;

class StatusIndicator : public QObject {
    Q_OBJECT

public:
    StatusIndicator(QObject *parent = 0);
    ~StatusIndicator();

    QWidget *create();
    void setScale(float scale);

    void updateStatus(QString visible, QString icon);
    QHash<QString, bool> getFullStatus();

private:
    GameDataContainer* gameDataContainer;

    QLabel *playerStatusLabel(const char*, const char*, bool);    
    void updatePosture(const char*, QString);
    void updateCondition(const char*, QString);

    void setInvisible(bool);
    void setImmobile(bool);
    void setJoined(bool);
    void setHidden(bool);
    void setCondition(bool, QString);
    void setPosture(bool, QString);

    bool visibleToBool(QString visible);

    QHash<QString, bool> conditionState;
    QHash<QString, bool> fullStatus;

    QLabel *invisible;
    QLabel *immobile;
    QLabel *joined;
    QLabel *hidden;
    QLabel *condition;
    QLabel *posture;

signals:

public slots:

};

#endif // STATUSINDICATOR_H
