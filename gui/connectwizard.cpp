#include "connectwizard.h"
#include "ui_connectwizard.h"

ConnectWizard::ConnectWizard(QWidget *parent) : QWizard(parent), ui(new Ui::ConnectWizard) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);
    settings = ClientSettings::Instance();

    movie = new QMovie(":/window/images/loading.gif");

    this->registerFields();

    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(pageSelected(int)));

    connect(this, SIGNAL(loadCharacterList(QString, QString)),
            mainWindow->getConnectionManager(), SLOT(initEauthSession(QString, QString)));

    connect(mainWindow->getConnectionManager(), SIGNAL(characterFound(QString, QString)),
            this, SLOT(addCharacterList(QString, QString)));

    connect(this, SIGNAL(retrieveSessionKey(QString)),
            mainWindow->getConnectionManager(), SLOT(retrieveEauthSessionKey(QString)));

    connect(mainWindow->getConnectionManager(), SIGNAL(sessionKeyRetrieved(QString)),
            this, SLOT(setSession(QString)));

    connect(this, SIGNAL(connectToServer(QString)),
            mainWindow->getConnectionManager(), SLOT(connectToHost(QString)));

    connect(this, SIGNAL(resetConnection()),
            mainWindow->getConnectionManager(), SLOT(resetEauthSession()));

    connect(mainWindow->getConnectionManager(), SIGNAL(eAuthError(QString)),
            this, SLOT(showError(QString)));
}

void ConnectWizard::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    this->init();
}

void ConnectWizard::registerFields() {
    ui->wizardPage1->registerField("gameHost*", ui->gameHostEdit);
    ui->wizardPage1->registerField("gamePort*", ui->gamePortEdit);
    ui->wizardPage1->registerField("authHost*", ui->authHostEdit);
    ui->wizardPage1->registerField("authPort*", ui->authPortEdit);
    ui->wizardPage1->registerField("user*", ui->userEdit);
    ui->wizardPage1->registerField("password*", ui->passwordEdit);
}

void ConnectWizard::init() {
    ui->gameHostEdit->setText(settings->getParameter("Login/gameHost", "prime.dr.game.play.net").toString());
    ui->gameHostEdit->setModified(true);

    ui->gamePortEdit->setText(settings->getParameter("Login/gamePort", "4901").toString());
    ui->gamePortEdit->setModified(true);

    ui->authHostEdit->setText(settings->getParameter("Login/authHost", "eaccess.play.net").toString());
    ui->authHostEdit->setModified(true);

    ui->authPortEdit->setText(settings->getParameter("Login/authPort", "7900").toString());
    ui->authPortEdit->setModified(true);

    QString user = settings->getParameter("Login/user", "").toString();
    if(!user.isEmpty()) {
        ui->userEdit->setText(user);
        ui->userEdit->setModified(true);
    }

    if(!password.isEmpty()) {
        ui->passwordEdit->setText(password);
        ui->passwordEdit->setModified(true);
    }

    if(ui->userEdit->text().isEmpty()) {
        ui->userEdit->setFocus();
    } else {
        ui->passwordEdit->setFocus();
    }
}

void ConnectWizard::saveField(QString name, QString value) {
    if(!value.isEmpty()) {
        settings->setParameter(name, value);
    }
}

void ConnectWizard::saveSettings() {
    this->saveField("Login/gameHost", ui->gameHostEdit->text());
    this->saveField("Login/gamePort", ui->gamePortEdit->text());
    this->saveField("Login/authHost", ui->authHostEdit->text());
    this->saveField("Login/authPort", ui->authPortEdit->text());
    this->saveField("Login/user", ui->userEdit->text());
}

void ConnectWizard::setLoading(bool loading) {
    if(loading) {
        ui->loadingLabel->setMovie(movie);
        movie->start();
    } else {
        ui->loadingLabel->clear();
    }
}

void ConnectWizard::pageSelected(int id) {
    switch (id) {
    case Page::login:
        characterList.clear();
        ui->characterBox->clear();

        emit resetConnection();
        break;
    case Page::character:
        if(characterList.isEmpty()) {
            this->saveSettings();
            this->password = ui->passwordEdit->text();
            ui->errorLabel->setText("");

            this->setLoading(true);

            emit loadCharacterList(ui->userEdit->text(), ui->passwordEdit->text());
            this->button(QWizard::NextButton)->setEnabled(false);
        }
        break;
    case Page::connect:
        this->button(QWizard::FinishButton)->setEnabled(false);
        emit retrieveSessionKey(characterList.value(ui->characterBox->currentText()));
        ui->finishLabel->setText("Retrieving session key ...");
        break;
    }
}

void ConnectWizard::addCharacterList(QString id, QString name) {
    characterList.insert(name, id);

    int index = ui->characterBox->findText(name);
    if(index == -1) {
        ui->characterBox->addItem(name);
    }

    this->setLoading(false);
    this->button(QWizard::NextButton)->setEnabled(true);
}

void ConnectWizard::setSession(QString sessionKey) {
    this->sessionKey = sessionKey;
    this->button(QWizard::FinishButton)->setEnabled(true);
    this->button(QWizard::FinishButton)->setFocus();
    ui->finishLabel->setText("Press finish to connect with " + ui->characterBox->currentText() + ".");
}

void ConnectWizard::accept() {
    QDialog::accept();

    emit connectToServer(sessionKey);
    this->restart();
    this->init();
}

void ConnectWizard::reject() {
    QDialog::reject();

    ui->errorLabel->setText("");
    this->password = "";
    emit resetConnection();
}

void ConnectWizard::showError(QString errorMsg) {
    ui->errorLabel->setText(errorMsg);
    this->restart();
    this->init();
}

ConnectWizard::~ConnectWizard() {
    delete movie;
    delete ui;
}
