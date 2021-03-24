#ifndef DICTIONARYSERVICE_H
#define DICTIONARYSERVICE_H

#include <QObject>
#include <QMutex>
#include <QQueue>
#include <QProcess>

class DictionarySettings;

class DictionaryService : public QObject {
    Q_OBJECT
public:
  enum ErrorCode {
    UnableToExecuteDict,
    WordNotFound
  };

public:
    explicit DictionaryService(QObject *parent);
    ~DictionaryService();

    void translate(const QString& word);
    
signals:
    void translationFinished(QString translation);
    void translationFailed(QString reason);

private slots:
    void processErrorOccurred(QProcess::ProcessError error);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    
private:
    DictionarySettings *settings;
    QProcess* process;
    QMutex mutex;
    QQueue<QString> wordQueue;
};

#endif // DICTIONARYSERVICE_H
