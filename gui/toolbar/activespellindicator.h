#ifndef ACTIVESPELLINDICATOR_H
#define ACTIVESPELLINDICATOR_H

#include <QObject>

#include <QLabel>
#include <QHBoxLayout>

class ActiveSpellIndicator : public QObject {
    Q_OBJECT
public:
    explicit ActiveSpellIndicator(QObject *parent = 0);
    ~ActiveSpellIndicator();

    QWidget* create();
    void setToolTip(QString text);
    void setText(QString text);

    QLabel* spellLabel;

private:
    QLabel* createSpellLabel();

signals:
    
public slots:
    
};

#endif // ACTIVESPELLINDICATOR_H
