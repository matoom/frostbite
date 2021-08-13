#ifndef SCRIPTSTREAMSERVER_H
#define SCRIPTSTREAMSERVER_H

#include "qobjectdefs.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>

#include "scriptwriterthread.h"

class ScriptStreamServer : public ScriptWriterThread {
    Q_OBJECT
public:
    using Parent = ScriptWriterThread;

public:
    explicit ScriptStreamServer(QObject* parent = 0);
    virtual ~ScriptStreamServer();

private:
    void restart();
    void close();

    QTcpServer server;
    QList<QTcpSocket*> sockets;

public slots:
    void reloadSettings();
    // Put the message to the queue for processing and sending
    void writeData(QString message);
private slots:
    // connection management
    void onNewConnection();
    void onSocketStateChanged(QAbstractSocket::SocketState socketState);
    // send data to open sockets
    void sendMessage(QByteArray message);
};

#endif // SCRIPTSTREAMSERVER_H
