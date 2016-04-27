#ifndef XmlParserThread_H
#define XmlParserThread_H

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <QFile>
#include <QtXml/QDomNode>

#include <mainwindow.h>
#include <gamedatacontainer.h>
#include <highlighter.h>

class MainWindow;
class WindowFacade;
class Toolbar;
class CommandLine;
class GameDataContainer;
class Highlighter;

typedef QList<QString> DirectionsList;

class XmlParserThread : public QThread {
    Q_OBJECT

public:
    explicit XmlParserThread(QObject *parent = 0);
    ~XmlParserThread();
    
    virtual void run();

    void process(QByteArray);

private:
    QQueue<QByteArray> dataQueue;
    QMutex mMutex;

    bool filterPlainText(QDomElement, QDomNode);
    void filterDataTags(QDomElement, QDomNode);

    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    Toolbar* toolBar;
    CommandLine* commandLine;
    GameDataContainer* gameDataContainer;
    Highlighter* highlighter;
    QStringList inventory;

    QString gameText;
    QString scriptText;
    QDateTime time;
    QDateTime roundTime;

    bool exit;
    bool pushStream;
    bool inv;
    bool familiar;
    bool perc;
    bool mono;
    bool bold;
    bool initRoundtime;
    bool prompt;

    int familiarElementCount;

    void processGameData(QByteArray);
    void processPushStream(QByteArray);
    void writeGameText(QByteArray);
    //void writeScript(QByteArray);
    void fixMonoTags(QString&);
    QString stripTags(QString);
    QString stripPushTags(QString);

    QRegExp rxAmp;
    QRegExp rxDmg;

    QByteArray localData;

signals:
    void updateConversationsWindow(QString);

    void updateNavigationDisplay(DirectionsList);
    void updateRoomWindowTitle(QString);
    void updateExpWindow(QString, QString);
    void updateRoomWindow();
    void updateMapWindow(QString);
    void updateDeathsWindow(QString);
    void updateThoughtsWindow(QString);
    void updateArrivalsWindow(QString);
    void updateFamiliarWindow(QString);

    void updateVitals(QString, QString);
    void updateStatus(QString, QString);
    void updateWieldLeft(QString);
    void updateWieldRight(QString);
    void updateSpell(QString);
    void updateActiveSpells();
    void clearActiveSpells();

    void setTimer(int);
    void writeScriptMessage(QByteArray);
    void setMainTitle(QString);
    void writeText(QByteArray, bool);
    void writeSettings();
    void writeModeSettings();

public slots:
    void addData(QByteArray);
    void updateHighlighterSettings();
};

#endif // XmlParserThread_H
