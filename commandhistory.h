#ifndef COMMANDHISTORY_H
#define COMMANDHISTORY_H

#include <QLineEdit>
#include <QDebug>
#include <QKeyEvent>

class CommandHistory : public virtual QLineEdit {
    Q_OBJECT

public:
    CommandHistory(QWidget *parent = 0);

    QVector<QString> history;
    int historyCounter;

private:
    void historyBack();
    void historyForward();

protected:
    virtual void keyPressEvent(QKeyEvent *event) = 0;

};

#endif // COMMANDHISTORY_H
