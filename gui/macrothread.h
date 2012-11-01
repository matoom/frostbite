#ifndef MACROTHREAD_H
#define MACROTHREAD_H

#include <QThread>
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>

class MacroThread : public QThread {
    Q_OBJECT

public:
    explicit MacroThread();

    virtual void run();

private:
    QHash<QString, QStringList> macro;
    int sequenceTime;
    bool abort;

signals:
    void setText(QString);
    void setCursor(int);
    void writeCommand(QString);

public slots:
    void stopThread();
    void init(QHash<QString, QStringList>, int);

};

#endif // MACROTHREAD_H
