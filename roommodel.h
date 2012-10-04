#ifndef ROOMMODEL_H
#define ROOMMODEL_H

#include <QObject>

class RoomModel : public QObject {
    Q_OBJECT

public:
    explicit RoomModel(QObject *parent = 0);

    QString toString();

public:
    QString getName() { return name; }
    void setName(QString name) { this->name = name; }

    QString getDesc() { return desc; }
    void setDesc(QString desc) { this->desc = desc; }

    QString getObjs() { return objs; }
    void setObjs(QString objs) { this->objs = objs; }

    QString getPlayers() { return players; }
    void setPlayers(QString players) { this->players = players; }

    QString getExits() { return exits; }
    void setExits(QString exits) { this->exits = exits; }

    QString getExtra() { return extra; }
    void setExtra(QString extra) { this->extra = extra; }

private:
    QString name;
    QString desc;
    QString objs;
    QString players;
    QString exits;
    QString extra;

signals:
    
public slots:
    
};

#endif // ROOMMODEL_H
