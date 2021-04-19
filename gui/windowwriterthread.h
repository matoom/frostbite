#ifndef WINDOWWRITERTHREAD_H
#define WINDOWWRITERTHREAD_H

#include <QString>
#include <QPlainTextEdit>

#include "workqueuethread.h"

class Highlighter;
class Alter;
class WindowInterface;
class MainWindow;
class WindowInterface;

class WindowWriterThread : public WorkQueueThread<QString> {
    Q_OBJECT
    using Parent = WorkQueueThread<QString>;
public:
    WindowWriterThread(QObject *parent, WindowInterface* window);
    ~WindowWriterThread() = default;
protected:
    void onProcess(const QString& data) override;

private:
    QPlainTextEdit* textEdit;

    Highlighter* highlighter;
    Alter* alter;

    MainWindow* mainWindow;
    QRegExp rxRemoveTags;
    WindowInterface *window;

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
