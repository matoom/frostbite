#ifndef HIGHLIGHTINSERTER_H
#define HIGHLIGHTINSERTER_H

#include <QObject>

class MainWindow;

class HighlightInserter : public QObject {
    Q_OBJECT

public:
    explicit HighlightInserter(QObject *parent = 0);

    void regex(QString pattern, QString color);
    void commit();

private:
    MainWindow *mainWindow;

signals:
    void reloadTextHighlights();

public slots:
};

#endif // HIGHLIGHTINSERTER_H
