#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>

#include <mainwindow.h>
#include <roundtimedisplay.h>
#include <keyboardfilter.h>
#include <macroservice.h>

#include <wordcompleter.h>

#define MAX_HISTORY_SIZE 50
#define MAX_FILENAME_SIZE 30

class RoundTimeDisplay;
class MacroService;
class WordCompleter;

class CommandLine : public QLineEdit {
    Q_OBJECT

public:
    CommandLine(QWidget *parent = 0);
    ~CommandLine();

    void focus();
    void insertRtIndicator(QPixmap segmentDisplay, QPixmap numericDisplay);
    RoundTimeDisplay* getRoundtimeDisplay();

    void abortScript();
    void abortSequence();

    void historyBack();
    void historyForward();
    void writeCommand(QString, QString style = "body");
    void moveCursor(int);
    void completeCommand();
    bool runMacro(QString);
    void clearCommand();    
    void doCopy();
    void updateMacroSettings();

    int historyCounter;

private:
    MainWindow* mainWindow;
    RoundTimeDisplay* roundtimeDisplay;
    WindowFacade* windowFacade;
    MacroService* macroService;
    WordCompleter* wordCompleter;
    KeyboardFilter* keyboardFilter;

    void resizeEvent(QResizeEvent  *event);
    //void keyPressEvent(QKeyEvent *event);
    bool filterCommand(QString);

    void addHistory();    
    QVector<QString> history;

signals:

public slots:
    virtual void sendCommand();

private slots:
    void resetCompleter(const QString&);

};

#endif // COMMANDLINE_H
