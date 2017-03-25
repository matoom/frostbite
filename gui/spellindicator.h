#ifndef SPELLINDICATOR_H
#define SPELLINDICATOR_H

#include <QObject>
#include <QtGui>
#include <QLabel>
#include <QHBoxLayout>

#include <toolbardefines.h>
#include <defaultvalues.h>

class SpellIndicator : public QObject {
    Q_OBJECT

public:
    explicit SpellIndicator(QObject *parent = 0);
    ~SpellIndicator();

    QWidget* create();
    void setToolTip(QString text);
    void setText(QString text);

    QLabel* imageLabel;
    QLabel* textLabel;

private:
    QLabel* spellImageLabel(const char*);
    QLabel* spellTextLabel();

signals:

public slots:    

};

#endif // SPELLINDICATOR_H
