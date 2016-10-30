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
    static TextUtils* Instance();

    int expStateToNumeric(QString);
    int expBriefToNumeric(QString);
    QString expNumericToState(int);
    QString msToMMSS(int);
    QString addNumericStateToExp(QString exp);
    QString findLowestActiveValue(QStringList list);
    QString htmlToPlain(QString& data);
    void plainToHtml(QString& data);

    QString toHash(QString text);
    QString toBrief(QString direction);
    QString stripMapSpecial(QString text);

private:
    TextUtils(QObject *parent = 0);
    TextUtils(TextUtils const& copy);
    TextUtils& operator = (TextUtils const& copy);
    static TextUtils* m_pInstance;

    void populateExpStates();
    QStringList mindStates;

    QRegExp rxNumber;
    QRegExp rxRemoveTags;

signals:
    
public slots:
    
};

#endif // DATACONVERSIONSERVICE_H
