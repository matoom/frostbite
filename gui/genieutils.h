#ifndef GENIEUTILS_H
#define GENIEUTILS_H

#include <QObject>

class MainWindow;

class GenieUtils : public QObject {
    Q_OBJECT

public:
    explicit GenieUtils(QObject *parent = 0);

    void importHighlights(QString imports);

private:
    void addRegexHighlight(QString pattern, QString color);
    void commitSettings();

    MainWindow *mainWindow;

signals:
    void reloadSettings();
};

#endif // GENIEUTILS_H
