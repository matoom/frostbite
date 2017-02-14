#ifndef ALTER_H
#define ALTER_H

#include <QObject>
#include <QRegularExpression>

#include <text/alter/substitutionsettings.h>
#include <text/alter/ignoresettings.h>

#include <textutils.h>

class Alter : public QObject {
    Q_OBJECT

public:
    explicit Alter(QObject *parent = 0);
    ~Alter();

    QString substitute(QString text, QString window);
    bool ignore(QString text, QString window);

    void reloadSettings();

private:
     IgnoreSettings* ignoreSettings;
     SubstitutionSettings* substituteSettings;

signals:

public slots:
};

#endif // ALTER_H
