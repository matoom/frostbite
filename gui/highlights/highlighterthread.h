#ifndef HIGHLIGHTERTHREAD_H
#define HIGHLIGHTERTHREAD_H

#include <QObject>
#include <QPlainTextEdit>
#include <QQueue>
#include <QMutex>

#include <highlights/highlighter.h>
#include <globaldefines.h>

#include <log4qt/logger.h>

class Highlighter;

class HighlighterThread : public QThread {
    Q_OBJECT
    LOG4QT_DECLARE_QCLASS_LOGGER

public:
    HighlighterThread(QObject *parent, WindowInterface* window);
    ~HighlighterThread();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QPlainTextEdit* textEdit;
    Highlighter* highlighter;
    MainWindow* mainWindow;
    QMutex mMutex;
    QList<QString> logWindows;
    QRegExp rxRemoveTags;
    QString localData;
    WindowInterface *window;

    void process(QString);
    void setText(QString);

    bool exit;

public slots:
    void addText(QString);
    void updateSettings();

signals:
    void writeText(const QString&);
    void clearText();

};

#endif // HIGHLIGHTERTHREAD_H
