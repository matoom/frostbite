#ifndef VITALSINDICATOR_H
#define VITALSINDICATOR_H

#include <QObject>
#include <QtGui>
#include <QProgressBar>
#include <QHBoxLayout>

#define T_VITALS_W 18
#define T_VITALS_H 32

class VitalsIndicator : public QObject {
    Q_OBJECT
public:
    VitalsIndicator(QObject *parent = 0);
    ~VitalsIndicator();

    QWidget* create();
    void setScale(float scale);

    QWidget* vitalsWidget;
    QProgressBar* healthBar;
    QProgressBar* concentrationBar;
    QProgressBar* fatigueBar;
    QProgressBar* spiritBar;
    QProgressBar* manaBar;

    QFrame* healthIndicator;
    QFrame* manaIndicator;
    QFrame* concentrationIndicator;
    QFrame* fatigueIndicator;
    QFrame* spiritIndicator;

private:
    QProgressBar* vitalsProgress(const char*, const char*, qint8);
    QFrame* vitalsFrame(QProgressBar*);

signals:

public slots:

};

#endif // VITALSINDICATOR_H
