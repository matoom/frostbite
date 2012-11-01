#ifndef CONNECTWIZARD_H
#define CONNECTWIZARD_H

#include <QWizard>
#include <QDebug>

#include <clientsettings.h>
#include <mainwindow.h>

namespace Page {
    enum { login, character, connect };
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
    QMovie* movie;
    QString sessionKey;
    QString password;

    void init();
    void saveField(QString, QString);
    void saveSettings();
    void registerFields();
    void setLoading(bool);
    void showEvent(QShowEvent*);

    void accept();
    void reject();

private slots:
    void pageSelected(int);
    void addCharacterList(QString, QString);
    void setSession(QString);
    void showError(QString);

signals:
    void loadCharacterList(QString, QString);
    void retrieveSessionKey(QString);
    void connectToServer(QString);
    void resetConnection();
};

#endif // CONNECTWIZARD_H
