#ifndef KEYBOARDFILTER_H
#define KEYBOARDFILTER_H

#include <QObject>
#include <QKeyEvent>
#include <QDebug>

class CommandLine;

class KeyboardFilter : public QObject {
    Q_OBJECT

public:
    explicit KeyboardFilter(QObject *parent = 0);

protected:
    bool eventFilter(QObject *Object, QEvent *Event);

private:
    CommandLine *commandLine;

};

#endif // KEYBOARDFILTER_H
