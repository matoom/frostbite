#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QStringList>
#include <QDir>
#include <QSound>

class AudioPlayer : public QObject {
    Q_OBJECT

public:
    static AudioPlayer* Instance();

    QStringList getAudioList();
    void play(const QString&);

private:
    AudioPlayer(QObject *parent = 0);
    AudioPlayer(AudioPlayer const& copy);
    AudioPlayer& operator = (AudioPlayer const& copy);
    static AudioPlayer* m_pInstance;

    void loadAudioList();
    QStringList fileList;

signals:

public slots:

};

#endif // AUDIOPLAYER_H
