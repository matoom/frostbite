#include "audioplayer.h"

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;

    clientSettings = ClientSettings::getInstance();

    this->loadAudio();

    volume = clientSettings->getParameter("Audio/volume", 80).toFloat() / float(100);
    muted = clientSettings->getParameter("Audio/muted", false).toBool();

    connect(mainWindow, SIGNAL(volumeChanged(int)), this, SLOT(setVolume(int)));
    connect(mainWindow, SIGNAL(volumeMuted(bool)), this, SLOT(setMuted(bool)));
}

void AudioPlayer::setVolume(int volume) {
    this->volume = float(volume) / float(100);
}

void AudioPlayer::setMuted(bool muted) {
    this->muted = muted;
}

void AudioPlayer::loadAudio() {
    QStringList filter;
    filter << "*.wav";

    QDir myDir(QApplication::applicationDirPath()  + "/sounds");
    fileList = myDir.entryList(filter, QDir::Files, QDir::Name);

    for(QString fileName : fileList) {
        QSoundEffect* effect = new QSoundEffect(this);
        effect->setSource(QUrl::fromLocalFile(QApplication::applicationDirPath() + "/sounds/" + fileName));
        sounds.insert(fileName, effect);
    }
}

QStringList AudioPlayer::getAudioList() {
    return fileList;
}

void AudioPlayer::play(const QString& fileName) {
    if(!muted && sounds.contains(fileName)) {
        QSoundEffect* effect = sounds.value(fileName);
        effect->setVolume(volume);
        effect->play();
    }
}

AudioPlayer::~AudioPlayer() {
}

