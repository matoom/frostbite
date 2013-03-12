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
    QHash<QString, QString> gameList;
    QMovie* movie;

    QString sessionKey;
    QString sessionPort;
    QString sessionHost;
    QString password;

    QString selectedCharacter;
    QString selectedGame;

    bool gamesLoaded;

    void init();
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

private slots:
    void pageSelected(int);
    void addCharacterList(QString, QString);
    void setSession(QString, QString, QString);
    void showError(QString);
    void resetPassword();
    void enableGameSelect();

signals:
    void initSession(QString, QString, QString, QString);
    void gameSelected(QString);
    void retrieveSession(QString);
    void connectToServer(QString, QString, QString);
    void resetConnection();
};

#endif // CONNECTWIZARD_H
