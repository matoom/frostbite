#ifndef XMLPARSERTHREAD_H
#define XMLPARSERTHREAD_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QDateTime>
#include <QQueue>
#include <QByteArray>
#include <QFile>
#include <QHash>
#include <QtXml/QDomNode>

class MainWindow;
class WindowFacade;
class Toolbar;
class CommandLine;
class GameDataContainer;
class Highlighter;
class VitalsBar;

typedef QList<QString> DirectionsList;

class XmlParserThread : public QThread {
    Q_OBJECT

public:
    explicit XmlParserThread(QObject *parent = 0);
    ~XmlParserThread();

    virtual void run();
    bool isCmgr();

    void process(QString);

private:
    QQueue<QByteArray> dataQueue;
    QMutex mMutex;

    void cache(QByteArray data);    

    bool filterPlainText(QDomElement, QDomNode);
    bool filterDataTags(QDomElement, QDomNode);

    MainWindow* mainWindow;
    WindowFacade* windowFacade;
    Toolbar* toolBar;
    VitalsBar* vitalsBar;
    CommandLine* commandLine;
    GameDataContainer* gameDataContainer;
    Highlighter* highlighter;
    QStringList inventory;

    QString gameText;
    QDateTime time;    
    QDateTime roundTime;
    QDateTime castTime;

    QString charName;

    QString activeSpells;
    QString group;

    QString stormfrontSettings;

    QHash<QString, QVariant> scheduled;

    bool exit;
    bool bold;
    bool initRoundtime;
    bool initCastTime;
    bool prompt;

    QString streamCache;

    bool mono;
    bool cmgr;

    bool pushStream;

    void processGameData(QString);

    QString processMonoOutput(QString line);
    QString processCommands(QString line);

    void processPushStream(QString);
    void processDynaStream(QString);

    QString fixUnclosedStreamTags(QString data);

    void warnUnknownEntity(QString ref, QString xml);
    void warnInvalidXml(QString ref, QString xml);

    void writeTextLines(QString text);

    QString parseTalk(QDomElement element);
    QString toString(QDomElement element);
    QString fixInputXml(QString);
    QString stripTags(QString);
    QString addTime(QString);        
    QString wrapRoot(QString data);
    QString wrapCdata(QString data);

    QString traverseXmlNode(QDomElement element, QString text);

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
    void updateSpellWindow(QString);
    void updateAtmosphericsWindow(QString);
    void updateGroupWindow(QString);
    void updateCombatWindow(QString);

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
    void writeDefaultSettings(QString);

    void registerStreamWindow(QString, QString);
    void writeStreamWindow(QString, QString);
    void clearStreamWindow(QString);    

public slots:
    void addData(QByteArray);
    void flushStream();
    void updateHighlighterSettings();
};

#endif // XMLPARSERTHREAD_H
