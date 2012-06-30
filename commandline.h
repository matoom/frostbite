#ifndef COMMANDLINE_H
#define COMMANDLINE_H

#include <QLineEdit>

#include <mainwindow.h>
#include <roundtimedisplay.h>
#include <keyboardfilter.h>

#define MAX_HISTORY_SIZE 50

class RoundTimeDisplay;

class CommandLine : public QLineEdit {
    Q_OBJECT

public:
    CommandLine(QWidget *parent = 0);
    ~CommandLine();

    void focus();
    void insertRtIndicator(QPixmap segmentDisplay, QPixmap numericDisplay);
    RoundTimeDisplay* getRoundtimeDisplay();

    void historyBack();
    void historyForward();
    void writeCommand();
    int historyCounter;

private:
    MainWindow *mainWindow;
    RoundTimeDisplay *roundtimeDisplay;
    WindowManager *windowManager;
    KeyboardFilter keyboardFilter;

    void resizeEvent(QResizeEvent  *event);
    //void keyPressEvent(QKeyEvent *event);

    void addHistory();    
    QVector<QString> history;


public slots:
    virtual void sendCommand();

};

#endif // COMMANDLINE_H
