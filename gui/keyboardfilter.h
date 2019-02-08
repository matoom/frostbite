#ifndef KEYBOARDFILTER_H
#define KEYBOARDFILTER_H

#include <QObject>
#include <QKeyEvent>
#include <QDebug>

#include <macrosettings.h>

class CommandLine;
class MacroSettings;

class KeyboardFilter : public QObject {
    Q_OBJECT

public:
    explicit KeyboardFilter(QObject *parent = 0);
    ~KeyboardFilter();

protected:
    bool eventFilter(QObject *Object, QEvent *Event);

private:
    CommandLine* commandLine;
    MacroSettings* macroSettings;
public slots:
    void reloadSettings();

};

#endif // KEYBOARDFILTER_H
