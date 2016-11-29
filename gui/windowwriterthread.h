#ifndef WINDOWWRITERTHREAD_H
#define WINDOWWRITERTHREAD_H

#include <QObject>
#include <QPlainTextEdit>
#include <QQueue>
#include <QMutex>

#include <text/highlight/highlighter.h>
#include <globaldefines.h>

class Highlighter;

class WindowWriterThread : public QThread {
    Q_OBJECT

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

#endif // WINDOWWRITERTHREAD_H
