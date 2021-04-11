#ifndef DICTIONARYSERVICE_H
#define DICTIONARYSERVICE_H

#include <QObject>
#include <QProcess>

class DictionarySettings;
class MainWindow;

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

private:
    MainWindow* mainWindow;

    void emitError(const QString& reason);
        
private slots:
    void processErrorOccurred(QProcess::ProcessError error);
    void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
    
private:
    DictionarySettings *settings;
    QProcess* process;
};

#endif // DICTIONARYSERVICE_H
