#include "connectwizard.h"
#include "ui_connectwizard.h"

ConnectWizard::ConnectWizard(QWidget *parent) : QWizard(parent), ui(new Ui::ConnectWizard) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);
    settings = ClientSettings::Instance();

    movie = new QMovie(":/window/images/loading.gif");

    this->registerFields();
    this->populateGameList();

    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(pageSelected(int)));


    connect(this, SIGNAL(gameSelected(QString)),
            mainWindow->getConnectionManager(), SLOT(gameSelected(QString)));

    connect(this, SIGNAL(initSession(QString, QString, QString, QString)),
            mainWindow->getConnectionManager(), SLOT(initEauthSession(QString, QString, QString, QString)));

    connect(mainWindow->getConnectionManager(), SIGNAL(characterFound(QString, QString)),
            this, SLOT(addCharacterList(QString, QString)));

    connect(this, SIGNAL(retrieveSession(QString)),
            mainWindow->getConnectionManager(), SLOT(retrieveEauthSession(QString)));

    connect(mainWindow->getConnectionManager(), SIGNAL(sessionRetrieved(QString, QString, QString)),
            this, SLOT(setSession(QString, QString, QString)));

    connect(this, SIGNAL(connectToServer(QString, QString, QString)),
            mainWindow->getConnectionManager(), SLOT(connectToHost(QString, QString, QString)));

    connect(this, SIGNAL(resetConnection()),
            mainWindow->getConnectionManager(), SLOT(resetEauthSession()));

    connect(mainWindow->getConnectionManager(), SIGNAL(eAuthError(QString)),
            this, SLOT(showError(QString)));

    connect(mainWindow->getConnectionManager(), SIGNAL(resetPassword()),
            this, SLOT(resetPassword()));

    connect(mainWindow->getConnectionManager(), SIGNAL(enableGameSelect()),
            this, SLOT(enableGameSelect()));
}

void ConnectWizard::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);

    this->init();
}

void ConnectWizard::populateGameList() {
    gameList.insert("Dragonrealms", "DR");
    gameList.insert("Dragonrealms The Fallen", "DRF");
    gameList.insert("Dragonrealms Prime Test", "DRT");
    gameList.insert("Dragonrealms Platinum", "DRX");

    new QListWidgetItem(tr("Dragonrealms"), ui->gameList);
    new QListWidgetItem(tr("Dragonrealms The Fallen"), ui->gameList);
    new QListWidgetItem(tr("Dragonrealms Prime Test"), ui->gameList);
    new QListWidgetItem(tr("Dragonrealms Platinum"), ui->gameList);

    QList<QListWidgetItem*> selectedItems =
            ui->gameList->findItems(settings->getParameter("Login/game", "").toString(), Qt::MatchExactly);

    if(selectedItems.count() > 0) {
        ui->gameList->setCurrentItem(selectedItems.first());
    } else {
        ui->gameList->setCurrentRow(0);
    }
}

void ConnectWizard::registerFields() {
    ui->wizardPage1->registerField("authHost*", ui->authHostEdit);
    ui->wizardPage1->registerField("authPort*", ui->authPortEdit);
    ui->wizardPage1->registerField("user*", ui->userEdit);
    ui->wizardPage1->registerField("password*", ui->passwordEdit);
}

void ConnectWizard::init() {            
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
    this->saveField("Login/authHost", ui->authHostEdit->text());
    this->saveField("Login/authPort", ui->authPortEdit->text());
    this->saveField("Login/user", ui->userEdit->text());
}

void ConnectWizard::saveHistory() {
    this->saveField("Login/character", selectedCharacter);
    this->saveField("Login/game", selectedGame);
}

void ConnectWizard::enableGameSelect() {
    this->setGameListLoading(false);
    this->button(QWizard::NextButton)->setEnabled(true);
    ui->gameList->setEnabled(true);
}

void ConnectWizard::setCharacterListLoading(bool loading) {
    if(loading) {
        ui->loadingLabel->setMovie(movie);
        movie->start();
    } else {
        ui->loadingLabel->clear();
    }
}

void ConnectWizard::setGameListLoading(bool loading) {
    if(loading) {
        ui->gameListLoading->setMovie(movie);
        movie->start();
    } else {
        ui->gameListLoading->clear();
    }
}

void ConnectWizard::pageSelected(int id) {
    switch (id) {
    case Page::login:
        gamesLoaded = false;
        ui->gameList->setEnabled(false);

        emit resetConnection();
        break;
    case Page::game:
        characterList.clear();
        ui->characterList->clear();
        if(!gamesLoaded) {
            this->saveSettings();
            this->setGameListLoading(true);
            emit initSession(ui->authHostEdit->text(), ui->authPortEdit->text(),
                             ui->userEdit->text(), ui->passwordEdit->text());
            this->button(QWizard::NextButton)->setEnabled(false);
            gamesLoaded = true;
        }
        break;
    case Page::character:
        selectedGame = ui->gameList->currentItem()->text();
        emit gameSelected(gameList.value(selectedGame));

        if(characterList.isEmpty()) {            
            this->password = ui->passwordEdit->text();
            ui->errorLabel->setText("");

            this->setCharacterListLoading(true);

            this->button(QWizard::NextButton)->setEnabled(false);
        }
        break;
    case Page::connect:        
        this->button(QWizard::BackButton)->setEnabled(false);

        selectedCharacter = ui->characterList->currentItem()->text();

        this->saveHistory();

        this->button(QWizard::FinishButton)->setEnabled(false);
        emit retrieveSession(characterList.value(selectedCharacter));
        ui->finishLabel->setText("Retrieving session key ...");
        break;
    }
}

void ConnectWizard::resetPassword() {
    ui->passwordEdit->clear();
}

void ConnectWizard::addCharacterList(QString id, QString name) {
    characterList.insert(name, id);

    new QListWidgetItem(name, ui->characterList);

    QList<QListWidgetItem*> selectedItems =
            ui->characterList->findItems(settings->getParameter("Login/character", "").toString(), Qt::MatchExactly);

    if(selectedItems.count() > 0) {
        ui->characterList->setCurrentItem(selectedItems.first());
    } else if(characterList.size() == 1) {
        ui->characterList->setCurrentRow(0);
    }

    this->setCharacterListLoading(false);
    this->button(QWizard::NextButton)->setEnabled(true);
}

void ConnectWizard::setSession(QString host, QString port, QString sessionKey) {
    this->sessionHost = host;
    this->sessionPort = port;
    this->sessionKey = sessionKey;

    this->button(QWizard::FinishButton)->setEnabled(true);
    this->button(QWizard::FinishButton)->setFocus();
    ui->finishLabel->setText("Press finish to connect with " + ui->characterList->currentItem()->text() + ".");
}

void ConnectWizard::accept() {
    QDialog::accept();

    emit connectToServer(sessionHost, sessionPort, sessionKey);
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
