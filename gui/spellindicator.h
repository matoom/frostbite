#ifndef SPELLINDICATOR_H
#define SPELLINDICATOR_H

#include <QObject>
#include <QtGui>

#include <toolbardefines.h>

class SpellIndicator : public QObject {
    Q_OBJECT

public:
    explicit SpellIndicator(QObject *parent = 0);

    QWidget* create();

    QLabel* imageLabel;

private:
    QLabel* wieldImageLabel(const char*);

signals:

public slots:

};

#endif // SPELLINDICATOR_H
