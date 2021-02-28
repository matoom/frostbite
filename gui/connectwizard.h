#ifndef CONNECTWIZARD_H
#define CONNECTWIZARD_H

#include <QWizard>
#include <QDebug>

#include <clientsettings.h>
#include <mainwindow.h>

struct Tab {
    typedef enum {
        Login,
        DirectConnect,
        AnyConnect,
        Proxy
    } Type;
};

/*namespace Tab {
    enum { login, directConnect, anyConnect, proxy };
}*/

namespace Page {
    enum { Login, Game, Character, Connect };
}

class ClientSettings;

namespace Ui {
    class ConnectWizard;
}

class ConnectWizard : public QWizard {
    Q_OBJECT
    
public:
    explicit ConnectWizard(QWidget *parent = 0);
    ~ConnectWizard();
    
private:
    Ui::ConnectWizard *ui;

    ClientSettings* settings;
    MainWindow* mainWindow;
    QHash<QString, QString> characterList;
    QMap<QString, QString> gameList;
    QMovie* movie;

    QString sessionKey;
    QString sessionPort;
    QString sessionHost;
    QString password;

    QString selectedCharacter;
    QString selectedGame;

    bool gamesLoaded;    
    bool isLichConfigured;

    Tab::Type currentTab;

    void init();
    void initProxy();
    void saveField(QString, QString);
    void saveSettings(Tab::Type tab);
    void saveHistory();
    void registerFields();
    void populateGameList();
    void setCharacterListLoading(bool);
    void setGameListLoading(bool);
    void showEvent(QShowEvent*);
    void _connect(QString, QString, QString);

    void accept();
    void reject();

    bool validateCurrentPage();

    QMutex mutex;

private slots:
    void pageSelected(int);
    void addCharacterList(QString, QString);
    void setSession(QString, QString, QString);
    void showError(QString);
    void resetPassword();
    void enableGameSelect();
    void setGameList(QMap<QString, QString>);
    void lichBoxChanged(int);
    void tabChanged(int);

signals:
    void initSession(QString, QString, QString, QString);
    void setProxy(bool, QString, QString);
    void gameSelected(QString);
    void retrieveSession(QString);
    void connectToLich(QString, QString, QString);
    void connectToServer(QString, QString, QString);
    void connectToHost(QString, QString);
    void resetConnection();
};

#endif // CONNECTWIZARD_H
