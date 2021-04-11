#ifndef ALTER_H
#define ALTER_H

#include <QObject>
#include <QRegularExpression>
#include <QUrl>

#include "altersettingsentry.h"

class IgnoreSettings;
class SubstitutionSettings;
class LinkSettings;
class HyperlinkService;

class Alter : public QObject {
    Q_OBJECT

public:
    explicit Alter(QObject *parent = 0);
    ~Alter();

    QString substitute(QString text, QString window);
    QString addLink(QString text, QString window);
    bool ignore(QString text, QString window);

    void reloadSettings();

    QList<AlterSettingsEntry> subsList;
    QList<AlterSettingsEntry> ignoreList;
    QList<AlterSettingsEntry> linkList;

private:
     IgnoreSettings* ignoreSettings;
     SubstitutionSettings* substituteSettings;
     LinkSettings* linkSettings;
     bool ignoreEnabled;

     int createLink(AlterSettingsEntry entry, QString &text, int indexStart, QString match);
     QString createCommand(QString text, QString command);

signals:

public slots:
};

#endif // ALTER_H
