#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "mainwindow.h"
#include "clientsettings.h"


ConnectDialog::ConnectDialog(QWidget *parent) : QDialog(parent), ui(new Ui::ConnectDialog) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);
    settings = ClientSettings::Instance();

    this->restoreGeometry(settings->getParameter("ConnectionDialog", NULL).toByteArray());

    /* load last/default settings */
    this->loadData();

    connect(this->ui->useProxy, SIGNAL(stateChanged(int)), this, SLOT(proxyState(int)));
    connect(this->ui->connectButton, SIGNAL(clicked()), this, SLOT(connectStart()));
    connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(connectCancel()));

    if(this->ui->user->text().isEmpty()) {
        this->ui->user->setFocus();
    } else {
        this->ui->key->setFocus();
    }
}

void ConnectDialog::loadData() {
    this->ui->serverHost->setText(settings->getParameter("Login/serverHost", "prime.dr.game.play.net").toString());
    this->ui->serverPort->setText(settings->getParameter("Login/serverPort", "4901").toString());
    this->ui->authHost->setText(settings->getParameter("Login/authHost", "eaccess.play.net").toString());
    this->ui->authPort->setText(settings->getParameter("Login/authPort", "7900").toString());
    this->ui->user->setText(settings->getParameter("Login/user", "").toString());

    proxyEnabled(settings->getParameter("Login/useProxy", false).toBool());
    this->ui->useProxy->setChecked(settings->getParameter("Login/useProxy", false).toBool());
    this->ui->proxyHost->setText(settings->getParameter("Login/proxyHost", "").toString());
    this->ui->proxyPort->setText(settings->getParameter("Login/proxyPort", "").toString());
}

void ConnectDialog::proxyState(int state) {
    switch (state) {
        case 0:
            proxyEnabled(false);
            break;
        case 2:
            proxyEnabled(true);
            break;
        default:
            proxyEnabled(false);
            break;
    }
}

void ConnectDialog::proxyEnabled(bool state) {
    this->ui->proxyHost->setEnabled(state);
    this->ui->proxyPort->setEnabled(state);
}

void ConnectDialog::connectStart() {
    this->saveSettings();
    // start connection code here
    mainWindow->getConnectionManager()->initLoginSession(this->ui->user->text(),
        this->ui->key->text());

    this->accept();
}

void ConnectDialog::connectCancel() {
    this->reject();
}

void ConnectDialog::saveSettings() {
    settings->setParameter("Login/serverHost", this->ui->serverHost->text());
    settings->setParameter("Login/serverPort", this->ui->serverPort->text());
    settings->setParameter("Login/authHost", this->ui->authHost->text());
    settings->setParameter("Login/authPort", this->ui->authPort->text());
    settings->setParameter("Login/user", this->ui->user->text());

    settings->setParameter("Login/useProxy", this->ui->useProxy->isChecked());
    settings->setParameter("Login/proxyHost", this->ui->proxyHost->text());
    settings->setParameter("Login/proxyPort", this->ui->proxyPort->text());
}

void ConnectDialog::closeEvent(QCloseEvent *event) {
    /* save dialog window state */
    settings->setParameter("ConnectionDialog", this->saveGeometry());
}

ConnectDialog::~ConnectDialog() {
    delete ui;
}
