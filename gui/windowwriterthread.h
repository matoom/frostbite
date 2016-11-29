#ifndef WINDOWWRITERTHREAD_H
#define WINDOWWRITERTHREAD_H

#include <QObject>
#include <QPlainTextEdit>
#include <QQueue>
#include <QMutex>

#include <text/highlight/highlighter.h>
#include <globaldefines.h>

//#include <log4qt/logger.h>

class Highlighter;

class WindowWriterThread : public QThread {
    Q_OBJECT
    //LOG4QT_DECLARE_QCLASS_LOGGER

public:
    WindowWriterThread(QObject *parent, WindowInterface* window);
    ~WindowWriterThread();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QPlainTextEdit* textEdit;
    Highlighter* highlighter;
    MainWindow* mainWindow;
    QMutex mMutex;
    //QList<QString> logWindows;
    QRegExp rxRemoveTags;
    QString localData;
    WindowInterface *window;

    void process(QString);
    void setText(QString);

    bool exit;

public slots:
    void addText(QString);
    void clearWriter();
    void updateSettings();

signals:
    void writeText(const QString&);
    void clearText();

};

#endif // WINDOWWRITERTHREAD_H
