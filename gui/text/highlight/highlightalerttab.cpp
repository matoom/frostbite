#include "highlightalerttab.h"

HighlightAlertTab::HighlightAlertTab(QObject *parent) : QObject(parent) {
    highlightDialog = (HighlightDialog*)parent;
    settings = new HighlightSettings();
    audioPlayer = AudioPlayer::Instance();

    applyButton = highlightDialog->getApplyButton();

    bleedingGroup = highlightDialog->getBleedingGroup();
    bleedingPlayButton = highlightDialog->getBleedingPlayButton();
    bleedingPlaySelect = highlightDialog->getBleedingPlaySelect();

    stunGroup = highlightDialog->getStunGroup();
    stunPlayButton = highlightDialog->getStunPlayButton();
    stunPlaySelect = highlightDialog->getStunSelect();

    healthGroup = highlightDialog->getHealthGroup();
    healthPlayButton = highlightDialog->getHealthPlayButton();
    healthPlaySelect = highlightDialog->getHealthSelect();
    healthSlider = highlightDialog->getHealthSlider();
    healthSliderText = highlightDialog->getHealthSliderText();

    deathGroup = highlightDialog->getDeathGroup();
    deathPlayButton = highlightDialog->getDeathPlayButton();
    deathPlaySelect = highlightDialog->getDeathSelect();

    webbedGroup = highlightDialog->getWebbedGroup();
    webbedPlayButton = highlightDialog->getWebbedPlayButton();
    webbedPlaySelect = highlightDialog->getWebbedSelect();

    this->initFileSelect();
    this->initSettings();

    connect(healthSlider, SIGNAL(sliderMoved(int)), this, SLOT(healthValueUpdated(int)));

    connect(bleedingPlayButton, SIGNAL(clicked()), this, SLOT(bleedingPlay()));
    connect(stunPlayButton, SIGNAL(clicked()), this, SLOT(stunPlay()));
    connect(healthPlayButton, SIGNAL(clicked()), this, SLOT(healthPlay()));
    connect(deathPlayButton, SIGNAL(clicked()), this, SLOT(deathPlay()));
    connect(webbedPlayButton, SIGNAL(clicked()), this, SLOT(webbedPlay()));

    connect(bleedingGroup, SIGNAL(clicked(bool)), this, SLOT(bleedingSelected(bool)));
    connect(bleedingPlaySelect, SIGNAL(activated(const QString&)), this, SLOT(bleedingFileSelected(const QString&)));

    connect(stunGroup, SIGNAL(clicked(bool)), this, SLOT(stunSelected(bool)));
    connect(stunPlaySelect, SIGNAL(activated(const QString&)), this, SLOT(stunFileSelected(const QString&)));

    connect(healthGroup, SIGNAL(clicked(bool)), this, SLOT(healthSelected(bool)));
    connect(healthPlaySelect, SIGNAL(activated(const QString&)), this, SLOT(healthFileSelected(const QString&)));

    connect(deathGroup, SIGNAL(clicked(bool)), this, SLOT(deathSelected(bool)));
    connect(deathPlaySelect, SIGNAL(activated(const QString&)), this, SLOT(deathFileSelected(const QString&)));

    connect(webbedGroup, SIGNAL(clicked(bool)), this, SLOT(webbedSelected(bool)));
    connect(webbedPlaySelect, SIGNAL(activated(const QString&)), this, SLOT(webbedFileSelected(const QString&)));
}

void HighlightAlertTab::updateSettings() {
    delete settings;
    settings = new HighlightSettings();
}

void HighlightAlertTab::initSettings() {
    this->initBleedingSettings();
    this->initStunSettings();
    this->initHealthSettings();
    this->initDeathSettings();
    this->initWebbedSettings();
}

void HighlightAlertTab::initBleedingSettings() {
    bleedingGroup->setChecked(settings->getSingleParameter("AlertHighlight/bleeding/enabled", false).toBool());
    int index = bleedingPlaySelect->findText(settings->getSingleParameter("AlertHighlight/bleeding/file", "").toString());
    if(index != -1) {
        bleedingPlaySelect->setCurrentIndex(index);
    } else {
        bleedingPlaySelect->setCurrentIndex(0);
    }
}

void HighlightAlertTab::initStunSettings() {
    stunGroup->setChecked(settings->getSingleParameter("AlertHighlight/stun/enabled", false).toBool());
    int index = stunPlaySelect->findText(settings->getSingleParameter("AlertHighlight/stun/file", "").toString());
    if(index != -1) {
        stunPlaySelect->setCurrentIndex(index);
    } else {
        stunPlaySelect->setCurrentIndex(0);
    }
}

void HighlightAlertTab::initHealthSettings() {
    int value = settings->getSingleParameter("AlertHighlight/health/value", 99).toInt();
    healthSlider->setValue(value);
    healthSliderText->setText(QString::number(value) + "%");

    healthGroup->setChecked(settings->getSingleParameter("AlertHighlight/health/enabled", false).toBool());
    int index = healthPlaySelect->findText(settings->getSingleParameter("AlertHighlight/health/file", "").toString());
    if(index != -1) {
        healthPlaySelect->setCurrentIndex(index);
    } else {
        healthPlaySelect->setCurrentIndex(0);
    }
}

void HighlightAlertTab::initDeathSettings() {
    deathGroup->setChecked(settings->getSingleParameter("AlertHighlight/death/enabled", false).toBool());
    int index = deathPlaySelect->findText(settings->getSingleParameter("AlertHighlight/death/file", "").toString());
    if(index != -1) {
        deathPlaySelect->setCurrentIndex(index);
    } else {
        deathPlaySelect->setCurrentIndex(0);
    }
}

void HighlightAlertTab::initWebbedSettings() {
    webbedGroup->setChecked(settings->getSingleParameter("AlertHighlight/webbed/enabled", false).toBool());
    int index = webbedPlaySelect->findText(settings->getSingleParameter("AlertHighlight/webbed/file", "").toString());
    if(index != -1) {
        webbedPlaySelect->setCurrentIndex(index);
    } else {
        webbedPlaySelect->setCurrentIndex(0);
    }
}

void HighlightAlertTab::initFileSelect() {
    QStringList fileList = audioPlayer->getAudioList();

    bleedingPlaySelect->addItems(fileList);
    stunPlaySelect->addItems(fileList);
    healthPlaySelect->addItems(fileList);
    deathPlaySelect->addItems(fileList);
    webbedPlaySelect->addItems(fileList);
}

void HighlightAlertTab::registerChange(QString name, QVariant value) {
    if(!applyButton->isEnabled()) {
        applyButton->setEnabled(true);
    }
    changeList.insert(name, value);
}

void HighlightAlertTab::healthValueUpdated(int value) {
    healthSliderText->setText(QString::number(value) + "%");
    registerChange("health/value", value);
}

void HighlightAlertTab::bleedingSelected(bool value) {
    registerChange("bleeding/enabled", value);
}

void HighlightAlertTab::bleedingFileSelected(const QString& fileName) {
    registerChange("bleeding/file", fileName);
}

void HighlightAlertTab::stunSelected(bool value) {
    registerChange("stun/enabled", value);
}

void HighlightAlertTab::stunFileSelected(const QString& fileName) {
    registerChange("stun/file", fileName);
}

void HighlightAlertTab::healthSelected(bool value) {
    registerChange("health/enabled", value);
}

void HighlightAlertTab::healthFileSelected(const QString& fileName) {
    registerChange("health/file", fileName);
}

void HighlightAlertTab::deathSelected(bool value) {
    registerChange("death/enabled", value);
}

void HighlightAlertTab::deathFileSelected(const QString& fileName) {
    registerChange("death/file", fileName);
}

void HighlightAlertTab::webbedSelected(bool value) {
    registerChange("webbed/enabled", value);
}

void HighlightAlertTab::webbedFileSelected(const QString& fileName) {
    registerChange("webbed/file", fileName);
}

void HighlightAlertTab::bleedingPlay() {
    audioPlayer->play(bleedingPlaySelect->currentText());
}

void HighlightAlertTab::stunPlay() {
    audioPlayer->play(stunPlaySelect->currentText());
}

void HighlightAlertTab::healthPlay() {
    audioPlayer->play(healthPlaySelect->currentText());
}

void HighlightAlertTab::deathPlay() {
    audioPlayer->play(deathPlaySelect->currentText());
}

void HighlightAlertTab::webbedPlay() {
    audioPlayer->play(webbedPlaySelect->currentText());
}

void HighlightAlertTab::saveChanges() {
    QHashIterator<QString, QVariant> i(changeList);
    while (i.hasNext()) {
        i.next();
        settings->setSingleParameter("AlertHighlight/" + i.key(), i.value());
    }
}

void HighlightAlertTab::cancelChanges() {
    this->initSettings();
}

HighlightAlertTab::~HighlightAlertTab() {
    delete settings;
}
