#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QSoundEffect>

class MainWindow;
class ClientSettings;

class AudioPlayer : public QObject {
    Q_OBJECT

public:
    AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

private:    
    void loadAudio();

    QStringList fileList;
    QMap<QString, QSoundEffect*> sounds;

    ClientSettings* clientSettings;
    MainWindow* mainWindow;

    float volume;
    bool muted;

signals:

public slots:
    void setVolume(int volume);
    void setMuted(bool muted);
    QStringList getAudioList();
    void play(const QString& fileName);

};

#endif // AUDIOPLAYER_H
