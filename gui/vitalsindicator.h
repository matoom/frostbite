#ifndef VITALSINDICATOR_H
#define VITALSINDICATOR_H

#include <QObject>
#include <QtGui>
#include <QProgressBar>
#include <QHBoxLayout>

#include <toolbardefines.h>

class VitalsIndicator : public QObject {
    Q_OBJECT
public:
    VitalsIndicator(QObject *parent = 0);
    ~VitalsIndicator();

    QWidget* create();

    QWidget* vitalsWidget;
    QProgressBar* healthBar;
    QProgressBar* concentrationBar;
    QProgressBar* fatigueBar;
    QProgressBar* spiritBar;

private:
    QProgressBar* vitalsProgress(const char*, const char*, qint8);
    QFrame* vitalsFrame(QProgressBar*);

signals:

public slots:

};

#endif // VITALSINDICATOR_H
