#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>

#include <mainwindow.h>
#include <roundtimedisplay.h>
#include <keyboardfilter.h>

#define MAX_HISTORY_SIZE 50
#define MAX_FILENAME_SIZE 30

class RoundTimeDisplay;

class CommandLine : public QLineEdit {
    Q_OBJECT

public:
    CommandLine(QWidget *parent = 0);
    ~CommandLine();

    void focus();
    void insertRtIndicator(QPixmap segmentDisplay, QPixmap numericDisplay);
    RoundTimeDisplay* getRoundtimeDisplay();

    void stopScript();

    void historyBack();
    void historyForward();
    void writeCommand(QString);
    int historyCounter;

private:
    MainWindow* mainWindow;
    RoundTimeDisplay* roundtimeDisplay;
    WindowManager* windowManager;
    KeyboardFilter keyboardFilter;

    void resizeEvent(QResizeEvent  *event);
    //void keyPressEvent(QKeyEvent *event);
    bool filterCommand(QString);

    void addHistory();    
    QVector<QString> history;

public slots:
    virtual void sendCommand();

};

#endif // COMMANDLINE_H
