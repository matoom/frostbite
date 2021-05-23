#ifndef DICTIONARYSERVICE_H
#define DICTIONARYSERVICE_H

#include <QObject>
#include <QProcess>

#include "gui/workqueuethread.h"

class DictionarySettings;
class MainWindow;

class DictionaryService : public WorkQueueThread<QString> {
    Q_OBJECT
    using Parent = WorkQueueThread<QString>;    
public:
  enum ErrorCode {
    UnableToExecuteDict,
    WordNotFound
  };

public:
    explicit DictionaryService(QObject *parent);
    ~DictionaryService();

    void translate(const QString& word);

protected:
    void onProcess(const QString& data) override;
    
signals:
    void translationFinished(QString word, QString translation);
    void translationFailed(QString reason);

private:
    MainWindow* mainWindow;

    void emitError(const QString& reason);
private:
    DictionarySettings *settings;
};

#endif // DICTIONARYSERVICE_H
