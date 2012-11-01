#ifndef PING_H
#define PING_H

#include <QObject>

class Ping : public QObject {
    Q_OBJECT

public:
    Ping(QObject *parent = 0);

    QString getLatency();

signals:

public slots:

};

#endif // PING_H
