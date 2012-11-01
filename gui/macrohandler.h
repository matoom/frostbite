#ifndef MACROHANDLER_H
#define MACROHANDLER_H

#include <QObject>
#include <QKeyEvent>
#include <QDebug>
#include <QLineEdit>

class MacroHandler : public QObject
{
    Q_OBJECT
public:
    explicit MacroHandler(QObject *parent = 0);

    void processKeyPress(QKeyEvent *event);

signals:

public slots:

};

#endif // MACROHANDLER_H
