#include "connectdialog.h"
#include "ui_connectdialog.h"

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
}

void ConnectDialog::loadData() {
    this->ui->serverHost->setText(settings->getParameter("Connection/serverHost", "prime.dr.game.play.net").toString());
    this->ui->serverPort->setText(settings->getParameter("Connection/serverPort", "4901").toString());
    this->ui->key->setText(settings->getParameter("Connection/sessionKey", "").toString());

    this->ui->useProxy->setChecked(settings->getParameter("Connection/useProxy", false).toBool());

    proxyEnabled(settings->getParameter("Connection/useProxy", false).toBool());

    this->ui->proxyHost->setText(settings->getParameter("Connection/proxyHost", "").toString());
    this->ui->proxyPort->setText(settings->getParameter("Connection/proxyPort", "").toString());
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
    mainWindow->getConnectionManager()->connectToHost();

    this->accept();
}

void ConnectDialog::connectCancel() {
    this->reject();
}

void ConnectDialog::saveSettings() {
    settings->setParameter("Connection/serverHost", this->ui->serverHost->text());
    settings->setParameter("Connection/serverPort", this->ui->serverPort->text());
    settings->setParameter("Connection/sessionKey", this->ui->key->text());

    settings->setParameter("Connection/useProxy", this->ui->useProxy->isChecked());
    settings->setParameter("Connection/proxyHost", this->ui->proxyHost->text());
    settings->setParameter("Connection/proxyPort", this->ui->proxyPort->text());
}

void ConnectDialog::closeEvent(QCloseEvent *event) {
    /* save dialog window state */
    settings->setParameter("ConnectionDialog", this->saveGeometry());
}

ConnectDialog::~ConnectDialog() {
    delete ui;
}
