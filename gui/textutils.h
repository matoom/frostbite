#ifndef DATACONVERSIONSERVICE_H
#define DATACONVERSIONSERVICE_H

#include <QObject>
#include <QDebug>
#include <QHash>
#include <QStringList>
#include <QCryptographicHash>

#include <limits>

class TextUtils : public QObject {
    Q_OBJECT

public:
    explicit TextUtils(QObject *parent = 0);
    ~TextUtils();

    static int expStateToNumeric(QString);
    static int expBriefToNumeric(QString);
    static QString expNumericToState(int);
    static QString msToMMSS(int);    
    static QString findLowestActiveValue(QStringList list);
    static QString htmlToPlain(QString& data);
    static void plainToHtml(QString& data);

    static QString toHash(QString text);
    static QString toBrief(QString direction);
    static QString stripMapSpecial(QString text);

private:
    static void populateExpStates();
    static QStringList mindStates;

signals:
    
public slots:
    
};

#endif // DATACONVERSIONSERVICE_H
