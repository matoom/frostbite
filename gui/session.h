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

public slots:
    void unstuck();

private slots:
    // Connect statuses from tcp client
    void connectAvailable(bool);
    void connectStarted();
    void connectSucceeded();
    void writeMessage(QString);

signals:
    void flushCache();

private:
    void bindParserAndClient();
    void bindVitalsBar();
    void bindToolBar();
    void bindCommandLine();
    void bindWindowFacade();
    void bindWindows();
    void bindScriptService();
    void bindMainWindow();

    Lich* lich;
    TcpClient* tcpClient;
    XmlParserThread* xmlParser;

    MainWindow* mainWindow;
};

#endif
