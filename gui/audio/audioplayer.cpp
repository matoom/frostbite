#include "audioplayer.h"

AudioPlayer::AudioPlayer(QObject *parent) : QObject(parent) {
    mainWindow = (MainWindow*)parent;

    clientSettings = ClientSettings::getInstance();

    player = new QMediaPlayer(this);
    player->setVolume(clientSettings->getParameter("Audio/volume", 80).toInt());
    player->setMuted(clientSettings->getParameter("Audio/muted", false).toBool());

    this->loadAudio();

    connect(mainWindow, SIGNAL(volumeChanged(int)), this, SLOT(setVolume(int)));
    connect(mainWindow, SIGNAL(volumeMuted(bool)), this, SLOT(setMuted(bool)));
}

void AudioPlayer::setVolume(int volume) {
   player->setVolume(volume);
}

void AudioPlayer::setMuted(bool muted) {
   player->setMuted(muted);
}

void AudioPlayer::loadAudio() {
    QStringList filter;
    filter << "*.wav";

    QDir myDir(QApplication::applicationDirPath()  + "/sounds");
    fileList = myDir.entryList(filter, QDir::Files, QDir::Name);

    for(QString fileName : fileList) {
        QBuffer* buffer = new QBuffer(readFile(fileName));
        buffer->open(QIODevice::ReadOnly);
        sounds.insert(fileName, buffer);
    }
}

QStringList AudioPlayer::getAudioList() {
    return fileList;
}

QByteArray* AudioPlayer::readFile(QString fileName) {
    QFile file(QApplication::applicationDirPath() + "/sounds/" + fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray* fileBytes = new QByteArray(file.readAll());
    file.close();
    return fileBytes;
}

void AudioPlayer::play(const QString& fileName) {
    if(!fileName.isEmpty() && sounds.contains(fileName)) {
        QBuffer* buffer = sounds.value(fileName);
        player->setMedia(QMediaContent(), buffer);
        player->play();
    }
}

AudioPlayer::~AudioPlayer() {
    player->stop();
    delete player;

    for(QBuffer* buffer : sounds) {
        buffer->close();
        delete buffer;
    }
}

