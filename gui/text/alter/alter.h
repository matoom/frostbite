#ifndef ALTER_H
#define ALTER_H

#include <QObject>
#include <QRegularExpression>

#include "altersettingsentry.h"

class IgnoreSettings;
class SubstitutionSettings;

class Alter : public QObject {
    Q_OBJECT

public:
    explicit Alter(QObject *parent = 0);
    ~Alter();

    QString substitute(QString text, QString window);
    bool ignore(QString text, QString window);

    void reloadSettings();

    QList<AlterSettingsEntry> subsList;
    QList<AlterSettingsEntry> ignoreList;

private:
     IgnoreSettings* ignoreSettings;
     SubstitutionSettings* substituteSettings;
     bool ignoreEnabled;

signals:

public slots:
};

#endif // ALTER_H
