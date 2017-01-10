#ifndef WINDOWWRITERTHREAD_H
#define WINDOWWRITERTHREAD_H

#include <QObject>
#include <QPlainTextEdit>
#include <QQueue>
#include <QMutex>

#include <text/highlight/highlighter.h>
#include <text/alter/alter.h>
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
    Alter* alter;

    MainWindow* mainWindow;
    QMutex mMutex;
    QRegExp rxRemoveTags;
    QString localData;
    WindowInterface *window;

    void write(QString);
    QString process(QString text, QString win);

    void setText(QString);

    bool exit;

public slots:
    void addText(QString);
    void updateSettings();

signals:
    void writeStream(const QString&);
    void writeText(const QString&);
    void clearText();

};

#endif // WINDOWWRITERTHREAD_H
