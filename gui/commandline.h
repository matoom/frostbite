#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>

class RoundTimeDisplay;
class MacroService;
class WordCompleter;
class GenieUtils;
class KeyboardFilter;
class MainWindow;
class WindowFacade;
class GeneralSettings;

class CommandLine : public QLineEdit {
    Q_OBJECT

public:
    CommandLine(QWidget *parent = 0);
    ~CommandLine();

    void focus();
    void insertRt(QPixmap segmentDisplay, QPixmap numericDisplay);
    RoundTimeDisplay* getRoundtimeDisplay();

    void clearRt();
    void abortScript();
    void abortSequence();

    void historyBack();
    void historyForward();
    void writeCommand(QString, QString style = "body");
    void write(QString, QString style = "body");
    void moveCursor(int);
    void completeCommand();
    bool runMacro(QString);
    void clearCommand();    
    void doCopy();    
    void windowControl(int key);


    void loadSettings();

    void setCmdFont(QFont font);
    void setCmdFontColor(QColor color);
    void setCmdBgColor(QColor color);

    int historyCounter;

private:
    MainWindow* mainWindow;
    RoundTimeDisplay* roundtimeDisplay;
    WindowFacade* windowFacade;
    MacroService* macroService;
    WordCompleter* wordCompleter;
    KeyboardFilter* keyboardFilter;        
    GeneralSettings* settings;
    GenieUtils* genieUtils;

    void resizeEvent(QResizeEvent  *event);
    //void keyPressEvent(QKeyEvent *event);
    bool filterCommand(QString);

    void addHistory();    
    QVector<QString> history;

    QColor backgroundColor;

signals:

public slots:
    virtual void sendCommand();

private slots:
    void resetCompleter(const QString&);
    void reloadSettings();
};

#endif // COMMANDLINE_H
