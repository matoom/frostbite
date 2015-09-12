#ifndef SPELLINDICATOR_H
#define SPELLINDICATOR_H

#include <QObject>
#include <QtGui>
#include <QLabel>
#include <QHBoxLayout>

#include <toolbardefines.h>

class SpellIndicator : public QObject {
    Q_OBJECT

public:
    explicit SpellIndicator(QObject *parent = 0);

    QWidget* create();
    void setToolTip(QString text);

    QLabel* imageLabel;

private:
    QLabel* wieldImageLabel(const char*);

signals:

public slots:

};

#endif // SPELLINDICATOR_H
