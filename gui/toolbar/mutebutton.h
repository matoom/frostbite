#ifndef MUTEBUTTON_H
#define MUTEBUTTON_H

#include <QObject>
#include <QToolButton>

class Toolbar;
class ClientSettings;
class MainWindow;

class MuteButton : QToolButton {
    Q_OBJECT

public:
    explicit MuteButton(QWidget *parent = 0);

private:
    Toolbar* toolBar;
    ClientSettings* clientSettings;
    MainWindow* mainWindow;

    bool muted;

signals:
    void volumeMuted(bool muted);

public slots:
    void clicked();
    void setMuted(bool muted);
};

#endif // MUTEBUTTON_H
