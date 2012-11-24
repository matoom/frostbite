#ifndef DATAPROCESSTHREAD_H
#define DATAPROCESSTHREAD_H

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <QFile>
#include <QtXml/QDomNode>

#include <mainwindow.h>
#include <gamedatacontainer.h>
#include <highlighter.h>

class MainWindow;
class WindowManager;
class ToolbarManager;
class CommandLine;
class GameDataContainer;
class Highlighter;

typedef QList<QString> DirectionsList;

class DataProcessThread : public QThread {
    Q_OBJECT

public:
    explicit DataProcessThread(QObject *parent = 0);
    
    virtual void run();

    void process(QByteArray);

private:
    QQueue<QByteArray> dataQueue;

    bool filterPlainText(QDomElement, QDomNode);
    void filterDataTags(QDomElement, QDomNode);

    MainWindow* mainWindow;
    WindowManager* windowManager;
    ToolbarManager* toolbarManager;
    CommandLine* commandLine;
    GameDataContainer* gameDataContainer;
    Highlighter* highlighter;
    QStringList inventory;

    QString gameText;
    QString scriptText;
    QDateTime time;
    QDateTime roundTime;

    bool pushStream;
    bool inv;
    bool mono;
    bool bold;
    bool initRoundtime;

    void processGameData(QByteArray);
    void processPushStream(QByteArray);
    void writeGameText(QByteArray);
    void writeScript(QByteArray);
    void fixMonoTags(QString&);
    QString stripTags(QString);

signals:
    void updateConversationsWindow(QString);
    void writeGameWindow(QByteArray);
    void writePromptGameWindow(QByteArray);

    void updateNavigationDisplay(DirectionsList);
    void updateRoomWindowTitle(QString);
    void updateExpWindow();
    void updateRoomWindow();
    void updateDeathsWindow(QString);
    void updateThoughtsWindow(QString);
    void updateArrivalsWindow(QString);

    void updateVitals(QString, QString);
    void updateStatus(QString, QString);
    void updateWieldLeft(QString);
    void updateWieldRight(QString);
    void updateSpell(QString);

    void setTimer(int);
    void writeScriptMessage(QByteArray);
    void setMainTitle(QString);

public slots:
    void addData(QByteArray);
    
};

#endif // DATAPROCESSTHREAD_H
