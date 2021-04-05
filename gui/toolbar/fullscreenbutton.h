#ifndef FULLSCREENBUTTON_H
#define FULLSCREENBUTTON_H

#include <QObject>
#include <QToolButton>

class Toolbar;
class MainWindow;

class FullscreenButton : QToolButton {
    Q_OBJECT

public:
    explicit FullscreenButton(QWidget *parent = 0);
    
private:
    MainWindow* mainWindow;

    void init();
    void setButtonMode(bool);
    void buttonClicked();

signals:
    
public slots:

private slots:
    void clicked();
    
};

#endif // FULLSCREENBUTTON_H
