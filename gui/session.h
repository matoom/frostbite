#ifndef SESSION_H
#define SESSION_H

#include <QObject>


class MainWindow;
class TcpClient;
class XmlParserThread;
class Lich;

class Session : public QObject {
    Q_OBJECT
public:
    Session(MainWindow* parent, bool debug);
    ~Session() = default;

    TcpClient* getTcpClient();

    void openConnection(QString host, QString port, QString key);
    void openLocalConnection(QString port);
private:
    void setupParserAndClient();
    void setupClientStatus();

    Lich* lich;
    TcpClient* tcpClient;
    XmlParserThread* xmlParser;

    MainWindow* mainWindow;
signals:
    
};

#endif
