#ifndef WINDOWINTERFACE_H
#define WINDOWINTERFACE_H

#include <QColor>
#include <QPlainTextEdit>

class WindowInterface {

public:
    virtual ~WindowInterface() {}

    virtual QColor getBgColor() = 0;
    virtual QColor getTextColor() = 0;
    virtual QPlainTextEdit* getMainWindow() = 0;
    virtual QTextDocument* getDocument() = 0;
    virtual QString getObjectName() = 0;

    virtual void setAppend(bool) = 0;
    virtual bool append() = 0;
    virtual void setStream(bool) = 0;
    virtual bool stream() = 0;

public slots:

};

#endif // WINDOWINTERFACE_H
