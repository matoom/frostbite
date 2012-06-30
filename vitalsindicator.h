#ifndef VITALSINDICATOR_H
#define VITALSINDICATOR_H

#include <QObject>
#include <QtGui>

#include <toolbardefines.h>

class VitalsIndicator : public QObject {
    Q_OBJECT
public:
    VitalsIndicator(QObject *parent = 0);
    ~VitalsIndicator();

    QWidget* create();

    QProgressBar *healthBar;
    QProgressBar *concentrationBar;
    QProgressBar *fatigueBar;
    QProgressBar *spiritBar;

private:
    QProgressBar *vitalsProgress(const char*, const char*, qint8);

signals:

public slots:

};

#endif // VITALSINDICATOR_H
