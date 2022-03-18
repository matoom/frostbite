#include "connectwizard.h"
#include "ui_connectwizard.h"

#include "clientsettings.h"
#include "mainwindow.h"
#include "tcpclient.h"

ConnectWizard::ConnectWizard(QWidget *parent) : QWizard(parent), ui(new Ui::ConnectWizard) {
    ui->setupUi(this);

    mainWindow = (MainWindow*)qobject_cast<QObject *>(parent);
    settings = ClientSettings::getInstance();

    movie = new QMovie(":/window/images/loading.gif");

    currentTab = Tab::Login;
    ui->Connect->setCurrentIndex(0);

    this->registerFields();

    connect(this, SIGNAL(currentIdChanged(int)), this, SLOT(pageSelected(int)));

    connect(ui->lichBox, SIGNAL(stateChanged(int)), this, SLOT(lichBoxChanged(int)));
    connect(ui->gameLichBox, SIGNAL(stateChanged(int)), this, SLOT(lichBoxChanged(int)));

    connect(this, SIGNAL(gameSelected(QString)),
            mainWindow->getTcpClient(), SLOT(gameSelected(QString)));

    connect(this, SIGNAL(initSession(QString, QString, QString, QString)),
            mainWindow->getTcpClient(), SLOT(initEauthSession(QString, QString, QString, QString)));

    connect(this, SIGNAL(setProxy(bool, QString, QString)),
            mainWindow->getTcpClient(), SLOT(setProxy(bool, QString, QString)));

    connect(mainWindow->getTcpClient(), SIGNAL(characterFound(QString, QString)),
            this, SLOT(addCharacterList(QString, QString)));

    connect(this, SIGNAL(retrieveSession(QString)),
            mainWindow->getTcpClient(), SLOT(retrieveEauthSession(QString)));

    connect(mainWindow->getTcpClient(), SIGNAL(sessionRetrieved(QString, QString, QString)),
            this, SLOT(setSession(QString, QString, QString)));

    connect(this, SIGNAL(connectToServer(QString, QString, QString)),
            mainWindow->getTcpClient(), SLOT(connectToHost(QString, QString, QString)));

    connect(this, SIGNAL(connectToLich(QString, QString, QString)),
            mainWindow->getTcpClient(), SLOT(connectToLich(QString, QString, QString)));

    connect(this, SIGNAL(connectToHost(QString, QString)),
            mainWindow->getTcpClient(), SLOT(connectToHost(QString, QString)));

    connect(this, SIGNAL(resetConnection()),
            mainWindow->getTcpClient(), SLOT(resetEauthSession()));

    connect(mainWindow->getTcpClient(), SIGNAL(eAuthError(QString)),
            this, SLOT(showError(QString)));

    connect(mainWindow->getTcpClient(), SIGNAL(resetPassword()),
            this, SLOT(resetPassword()));

    connect(mainWindow->getTcpClient(), SIGNAL(enableGameSelect()),
            this, SLOT(enableGameSelect()));

    connect(mainWindow->getTcpClient(), SIGNAL(setGameList(QMap<QString, QString>)),
            this, SLOT(setGameList(QMap<QString, QString>)));

    connect(this->ui->Connect, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
}

void ConnectWizard::tabChanged(int id) {
    if(id == 0) {
        this->page(0)->setFinalPage(false);
        this->button(WizardButton::NextButton)->setEnabled(true);
        this->button(WizardButton::FinishButton)->setEnabled(true);
        currentTab = Tab::Login;
    } else if (id == 1) {
        this->page(0)->setFinalPage(true);
        this->button(WizardButton::NextButton)->setEnabled(false);
        this->button(WizardButton::FinishButton)->setEnabled(true);
        currentTab = Tab::DirectConnect;
    } else if (id == 2) {
        this->page(0)->setFinalPage(true);
        this->button(WizardButton::NextButton)->setEnabled(false);
        this->button(WizardButton::FinishButton)->setEnabled(true);
        currentTab = Tab::AnyConnect;
    } else if(id == 3) {
        this->button(WizardButton::NextButton)->setEnabled(false);
        this->button(WizardButton::FinishButton)->setEnabled(false);
        currentTab = Tab::Proxy;
    }
}

void ConnectWizard::lichBoxChanged(int state) {
    ui->lichBox->setChecked(state == Qt::CheckState::Checked);
    settings->setParameter("Login/lichEnabled", state == Qt::CheckState::Checked);
    ui->gameLichBox->setChecked(state == Qt::CheckState::Checked);
    settings->setParameter("DirectConnect/lichEnabled", state == Qt::CheckState::Checked);
}

void ConnectWizard::showEvent(QShowEvent* event) {
    QDialog::showEvent(event);
    this->init();
    this->initProxy();
    this->tabChanged(ui->Connect->currentIndex());
}

void ConnectWizard::populateGameList() {
    ui->gameList->clear();

    foreach(QString key, gameList.keys() ) {
        new QListWidgetItem(key, ui->gameList);
    }

    QString game = settings->getParameter("Login/game", "").toString();
    QList<QListWidgetItem*> selectedItems = ui->gameList->findItems(game, Qt::MatchExactly);

    if(selectedItems.count() > 0) {
        ui->gameList->setCurrentItem(selectedItems.first());
    } else {
        ui->gameList->setCurrentRow(0);
    }
}

void ConnectWizard::setGameList(QMap<QString, QString> gameList) {
    mutex.lock();
    this->gameList = gameList;
    mutex.unlock();
    this->populateGameList();
}

void ConnectWizard::registerFields() {
    ui->wizardPage1->registerField("authHost", ui->authHostEdit);
    ui->wizardPage1->registerField("authPort", ui->authPortEdit);
    ui->wizardPage1->registerField("user", ui->userEdit);
    ui->wizardPage1->registerField("password", ui->passwordEdit);

    ui->wizardPage1->registerField("gameHost", ui->gameHostEdit);
    ui->wizardPage1->registerField("gamePort", ui->gamePortEdit);
    ui->wizardPage1->registerField("sessionKey", ui->sessionKeyEdit);
}

void ConnectWizard::initProxy() {
    ui->proxyEnabled->setChecked(settings->getParameter("Proxy/enabled", false).toBool());
    ui->proxyHostEdit->setText(settings->getParameter("Proxy/host", "").toString());
    ui->proxyPortEdit->setText(settings->getParameter("Proxy/port", "").toString());
}

void ConnectWizard::init() {
    ui->authHostEdit->setText(settings->getParameter("Login/authHost", "eaccess.play.net").toString());
    ui->authHostEdit->setModified(true);
    QString port = settings->getParameter("Login/authPort", "7910").toString();
    //TODO: migrate old settings to ssl port; remove in later versions;
    if(port == "7900") {
        port = "7910";
    }
    ui->authPortEdit->setText(port);
    ui->authPortEdit->setModified(true);
    ui->lichBox->setChecked(settings->getParameter("Login/lichEnabled", false).toBool());

    ui->gameHostEdit->setText(settings->getParameter("DirectConnct/gameHost", "dr.simutronics.net").toString());
    ui->gameHostEdit->setModified(true);
    ui->gamePortEdit->setText(settings->getParameter("DirectConnct/gamePort", "11024").toString());
    ui->gamePortEdit->setModified(true);
    ui->gameLichBox->setChecked(settings->getParameter("Login/lichEnabled", false).toBool());

    ui->anyHostEdit->setText(settings->getParameter("AnyConnect/host", "localhost").toString());
    ui->anyHostEdit->setModified(true);
    ui->anyPortEdit->setText(settings->getParameter("AnyConnect/port", "8000").toString());
    ui->anyPortEdit->setModified(true);

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

    ui->sessionKeyEdit->setText("");

    isLichConfigured = !settings->getParameter("Script/lichLocation", "").toString().isEmpty();
    if(isLichConfigured) {
        ui->lichBox->setVisible(true);
        ui->gameLichBox->setVisible(true);
    } else {
        ui->lichBox->setVisible(false);
        ui->gameLichBox->setVisible(false);
    }
}

void ConnectWizard::saveField(QString name, QString value) {
    if(!value.isEmpty()) {
        settings->setParameter(name, value);
    }
}

void ConnectWizard::saveSettings(Tab::Type tab) {
    if(tab == Tab::Login) {
        this->saveField("Login/authHost", ui->authHostEdit->text());
        this->saveField("Login/authPort", ui->authPortEdit->text());
        this->saveField("Login/user", ui->userEdit->text());
    } else if(tab == Tab::DirectConnect) {
        this->saveField("DirectConnect/gameHost", ui->gameHostEdit->text());
        this->saveField("DirectConnect/gamePort", ui->gamePortEdit->text());
    } else if(tab == Tab::AnyConnect) {
        this->saveField("AnyConnect/host", ui->anyHostEdit->text());
        this->saveField("AnyConnect/port", ui->anyPortEdit->text());
    }
    settings->setParameter("Proxy/enabled", ui->proxyEnabled->isChecked());
    this->saveField("Proxy/host", ui->proxyHostEdit->text());
    this->saveField("Proxy/port", ui->proxyPortEdit->text());
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
    case Page::Login:
        gamesLoaded = false;
        ui->gameList->setEnabled(false);
        emit resetConnection();
        break;
    case Page::Game:
        characterList.clear();
        ui->characterList->clear();

        if(!gamesLoaded) {
            this->setGameListLoading(true);
            emit setProxy(ui->proxyEnabled->isChecked(), ui->proxyHostEdit->text(), ui->proxyPortEdit->text());
            emit initSession(ui->authHostEdit->text().trimmed(), ui->authPortEdit->text().trimmed(),
                             ui->userEdit->text().trimmed(), ui->passwordEdit->text());
            this->button(QWizard::NextButton)->setEnabled(false);
            gamesLoaded = true;
        }
        break;
    case Page::Character:
        selectedGame = ui->gameList->currentItem()->text();        
        emit gameSelected(gameList.value(selectedGame));
        if(characterList.isEmpty()) {
            this->password = ui->passwordEdit->text();
            ui->errorLabel->setText("");

            this->setCharacterListLoading(true);

            this->button(QWizard::NextButton)->setEnabled(false);
        }
        break;
    case Page::Connect:
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

    QString charName = settings->getParameter("Login/character", "").toString();
    QList<QListWidgetItem*> selectedItems = ui->characterList->findItems(charName, Qt::MatchExactly);

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
    if(this->validateCurrentPage()) {                
        if(currentTab == Tab::DirectConnect) {
            this->_connect(ui->gameHostEdit->text().trimmed(), ui->gamePortEdit->text().trimmed(),
                           ui->sessionKeyEdit->text().trimmed());
        } else if(currentTab == Tab::AnyConnect) {
            emit connectToHost(ui->anyHostEdit->text().trimmed(), ui->anyPortEdit->text().trimmed());
        } else if(currentTab == Tab::Login) {
            this->_connect(sessionHost, sessionPort, sessionKey);
        }
        this->saveSettings(currentTab);
        this->restart();
        this->init();
    }
}

void ConnectWizard::_connect(QString sessionHost, QString sessionPort, QString sessionKey) {
    if(ui->lichBox->isChecked() && isLichConfigured) {
        emit connectToLich(sessionHost, sessionPort, sessionKey);
    } else {
        emit connectToServer(sessionHost, sessionPort, sessionKey);
    }
}

void ConnectWizard::reject() {
    QDialog::reject();

    ui->errorLabel->setText("");
    ui->directErrorLabel->setText("");

    emit resetConnection();
}

void ConnectWizard::showError(QString errorMsg) {
    ui->errorLabel->setText(errorMsg);
    this->restart();
    this->init();
}

bool ConnectWizard::validateCurrentPage() {
    if(this->currentId() == 0) {
        if(currentTab == Tab::DirectConnect) {
            if(ui->gameHostEdit->text().isEmpty()) {
                ui->directErrorLabel->setText("Game host is required.");
                return false;
            } else if (ui->gamePortEdit->text().isEmpty()) {
                ui->directErrorLabel->setText("Game port is required.");
                return false;
            } else if (ui->sessionKeyEdit->text().isEmpty()) {
                ui->directErrorLabel->setText("Session key is required.");
                return false;
            } else {
                return true;
            }
        } else if(currentTab == Tab::AnyConnect) {
            if(ui->anyHostEdit->text().isEmpty()) {
                ui->anyErrorLabel->setText("Host is required.");
                return false;
            } else if (ui->anyPortEdit->text().isEmpty()) {
                ui->anyErrorLabel->setText("Port is required.");
                return false;
            } else {
                return true;
            }
        } else {
            if(ui->authHostEdit->text().isEmpty()) {
                ui->errorLabel->setText("Auth host is required.");
                return false;
            } else if (ui->authPortEdit->text().isEmpty()) {
                ui->errorLabel->setText("Auth port is required.");
                return false;
            } else if (ui->userEdit->text().isEmpty()) {
                ui->errorLabel->setText("Username is required.");
                return false;
            } else if (ui->passwordEdit->text().isEmpty()) {
                ui->errorLabel->setText("Password is required.");
                return false;
            } else {
                return true;
            }
        }
    } else {
        ui->directErrorLabel->setText("");
        ui->errorLabel->setText("");
        ui->anyErrorLabel->setText("");
        return true;
    }
}

ConnectWizard::~ConnectWizard() {
    delete movie;
    delete ui;
}
