#ifndef SESSION_H
#define SESSION_H

#include <QObject>


class MainWindow;
class TcpClient;
class XmlParserThread;

class Session : public QObject {
    Q_OBJECT
public:
    Session(MainWindow* parent, TcpClient* client, XmlParserThread* parser);
    ~Session() = default;

private:
    TcpClient* tcpClient;
    XmlParserThread* xmlParser;
    MainWindow* mainWindow;    
signals:
    
};

#endif
