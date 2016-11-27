#ifndef XMLPARSERTHREAD_H
#define XMLPARSERTHREAD_H

#include <QObject>
#include <QQueue>
#include <QByteArray>
#include <QFile>
#include <QtXml/QDomNode>

#include <mainwindow.h>
#include <gamedatacontainer.h>
#include <text/highlight/highlighter.h>

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
    bool filterDataTags(QDomElement, QDomNode);

    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    Toolbar* toolBar;
    CommandLine* commandLine;
    GameDataContainer* gameDataContainer;
    Highlighter* highlighter;
    QStringList inventory;

    QString gameText;
    QDateTime time;    
    QDateTime roundTime;
    QDateTime castTime;

    QString activeSpells;

    QHash<QString, QVariant> scheduled;

    bool exit;
    bool bold;
    bool initRoundtime;
    bool initCastTime;
    bool prompt;

    void processGameData(QByteArray);
    void processPushStream(QString);
    void processOutput(QString);
    void warnUnknownEntity(QString ref, QString xml);

    QString parseTalk(QDomElement element);
    QString toString(QDomElement element);
    QString fixInputXml(QString);
    QString stripTags(QString);
    QString addTime(QString);

    void runScheduledEvents();
    void runEvent(QString event, QVariant data);

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
    void updateActiveSpells(QStringList);
    void clearActiveSpells();

    void setTimer(int);
    void setCastTimer(int);

    void writeScriptMessage(QByteArray);
    void setMainTitle(QString);
    void writeText(QByteArray, bool);
    void writeSettings();
    void writeModeSettings();

public slots:
    void addData(QByteArray);
    void updateHighlighterSettings();
};

#endif // XMLPARSERTHREAD_H
