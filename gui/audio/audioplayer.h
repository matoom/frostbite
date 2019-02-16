#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QApplication>
#include <QMediaPlayer>
#include <QBuffer>
#include <clientsettings.h>
#include <mainwindow.h>

class MainWindow;

class AudioPlayer : public QObject {
    Q_OBJECT

public:
    AudioPlayer(QObject *parent = 0);
    ~AudioPlayer();

private:    
    void loadAudio();
    QByteArray* readFile(QString fileName);

    QStringList fileList;
    QMap<QString, QBuffer*> sounds;
    QMediaPlayer* player;

    ClientSettings* clientSettings;
    MainWindow* mainWindow;

signals:

public slots:
    void setVolume(int volume);
    void setMuted(bool muted);
    QStringList getAudioList();
    void play(const QString& fileName);

};

#endif // AUDIOPLAYER_H
