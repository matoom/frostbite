#ifndef HIGHLIGHTERTHREAD_H
#define HIGHLIGHTERTHREAD_H

#include <QObject>
#include <QPlainTextEdit>
#include <QQueue>
#include <QMutex>

#include <highlighter.h>

class Highlighter;

class HighlighterThread : public QThread {
    Q_OBJECT

public:
    HighlighterThread(QObject *parent, QPlainTextEdit* textEdit, bool multiLine);
    ~HighlighterThread();

    virtual void run();

private:
    QQueue<QString> dataQueue;
    QPlainTextEdit* textEdit;
    Highlighter* highlighter;
    MainWindow* mainWindow;
    QMutex mMutex;
    bool multiLine;


    void process(QString data);

public slots:
    void addText(QString);
    void updateSettings();

signals:
    void writeText(const QString&);
    void clearText();

};

#endif // HIGHLIGHTERTHREAD_H
