#ifndef COMMANDPARSER_H
#define COMMANDPARSER_H

#include <QFile>
#include <QDomNode>

#include <mainwindow.h>
#include <gamedatacontainer.h>
#include <expmodel.h>

class MainWindow;
class WindowManager;
class ToolbarManager;
class CommandLine;
class GameDataContainer;

class CommandParser : public QObject {
    Q_OBJECT

public:
    explicit CommandParser(QObject *parent = 0);

    void process(QByteArray);
    void processMock();

private:
    bool filterPlainText(QDomElement, QDomNode);
    void filterDataTags(QDomElement, QDomNode);

    MainWindow *mainWindow;
    WindowManager *windowManager;
    ToolbarManager *toolbarManager;
    CommandLine *commandLine;
    GameDataContainer *gameDataContainer;

    QString gameText;
    QDateTime time;
    QDateTime roundTime;

    bool pushStream;
    bool mono;
    bool bold;
    bool initRoundtime;

    void processGameData(QByteArray);
    void writeGameText(QByteArray);

signals:

public slots:

};

#endif // COMMANDPARSER_H
