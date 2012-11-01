#ifndef WORDCOMPLETER_H
#define WORDCOMPLETER_H

#include <QObject>
#include <QDebug>
#include <QStringList>

class WordCompleter : public QObject {
    Q_OBJECT

public:
    explicit WordCompleter(QObject *parent = 0);

    QString complete(QString);

    bool match;

private:
    QStringList words;
    QStringList matchesFound;
    int order;
    int commandCount;

    void findMatches(QStringList);
    
signals:
    
public slots:
    
};

#endif // WORDCOMPLETER_H
