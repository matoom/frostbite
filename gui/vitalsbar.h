#ifndef VITALSBAR_H
#define VITALSBAR_H

#include <QObject>
#include <QtGui>
#include <QProgressBar>

class MainWindow;
class ClientSettings;

class VitalsBar : public QObject {
    Q_OBJECT

public:
    explicit VitalsBar(QObject *parent = 0);
    ~VitalsBar();

    void add();
    void addToMenu();
    void load();

    bool isVisible() const;
private:
    MainWindow* mainWindow;

    ClientSettings* clientSettings;

    QAction* action;

    QProgressBar* health;
    QProgressBar* mana;
    QProgressBar* concentration;
    QProgressBar* spirit;
    QProgressBar* fatigue;

    QWidget* vitalsBar;

    QFrame* addFrame(QProgressBar* bar);
    void createMenuEntry(bool checked);
    QProgressBar* toolBar(const char* obName, QString bgColor);

signals:

public slots:
    void toggle(bool checked, bool saveSetting = true);
    void updateVitals(QString name, QString value);

};

#endif // VITALSBAR_H
