#ifndef WIELDINDICATOR_H
#define WIELDINDICATOR_H

#include <QObject>
#include <QtGui>
#include <QLabel>
#include <QHBoxLayout>

#define T_WIELD_W 150
#define T_WIELD_H 32

class WieldIndicator : public QObject {
    Q_OBJECT
public:
    WieldIndicator(QObject *parent = 0, const char* = "");
    ~WieldIndicator();

    QWidget *create();
    void setScale(float scale);

    QLabel *textLabel;
    QLabel *imageLabel;

private:
    QLabel *wieldTextLabel(const char*);
    QLabel *wieldImageLabel(const char*);

    const char* icon;

signals:

public slots:

};

#endif // WIELDINDICATOR_H
