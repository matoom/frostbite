#ifndef CONNECTWIZARD_H
#define CONNECTWIZARD_H

#include <QWizard>
#include <QDebug>

#include <clientsettings.h>
#include <mainwindow.h>

namespace Page {
    enum { login, game, character, connect };
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

    void init();
    void initProxy();
    void saveField(QString, QString);
    void saveSettings();
    void saveHistory();
    void registerFields();
    void populateGameList();
    void setCharacterListLoading(bool);
    void setGameListLoading(bool);
    void showEvent(QShowEvent*);

    void accept();
    void reject();

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

signals:
    void initSession(QString, QString, QString, QString);
    void setProxy(bool, QString, QString);
    void gameSelected(QString);
    void retrieveSession(QString);
    void connectToLich(QString, QString, QString);
    void connectToServer(QString, QString, QString);
    void resetConnection();
};

#endif // CONNECTWIZARD_H
