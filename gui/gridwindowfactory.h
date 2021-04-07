#ifndef GRIDWINDOWFACTORY_H
#define GRIDWINDOWFACTORY_H

#include <QObject>
#include <QTableWidget>
#include <QDockWidget>
#include <QHeaderView>

class MainWindow;
class GeneralSettings;

class GridWindowFactory : public QObject {
    Q_OBJECT

public:
    explicit GridWindowFactory(QObject *parent = 0);
    ~GridWindowFactory();

    QDockWidget* createWindow(const char*);

private:
    QPalette palette();
    QTableWidget* tableBox(QString);

    MainWindow* mainWindow;
    GeneralSettings* settings;

signals:

public slots:
};

#endif // GRIDWINDOWFACTORY_H
