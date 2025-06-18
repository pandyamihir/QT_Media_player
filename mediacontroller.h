#ifndef MEDIACONTROLLER_H
#define MEDIACONTROLLER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include "metadatamanager.h"
#include "playlistmanager.h"

class MediaController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentPos READ currentPos WRITE setCurrentPos NOTIFY currentPosChanged)
    Q_PROPERTY(int totalDuration READ totalDuration NOTIFY totalDurationChanged)
    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)
    Q_PROPERTY(bool muted READ isMuted WRITE setMuted NOTIFY mutedChanged)

public:
    explicit MediaController(MetadataManager * meta, PlaybackController *playlist, QObject *parent = nullptr);
    Q_INVOKABLE void play();
    Q_INVOKABLE void pause();
    Q_INVOKABLE void stop();
    Q_INVOKABLE void setCurrentPos(int newCurrentPos);
    Q_INVOKABLE void volumeUp();
    Q_INVOKABLE void volumeDown();
    Q_INVOKABLE void toggleMute();

    int currentPos() const;
    int totalDuration() const;
    int volume() const;
    bool isMuted() const;
    void setVolume(int volume);
    void setMuted(bool muted);

signals:
    void currentPosChanged();
    void totalDurationChanged();
    void volumeChanged();
    void mutedChanged();

private:
    QMediaPlayer * player;
    QAudioOutput * audioOutput;
    MetadataManager * metaData;
    PlaybackController *playlistManager;

    int m_currentPos;
    int m_totalDuration;

    int m_volume = 50;
    bool m_muted = false;

public slots:
    void onPositionChanged(int pos);
    void onDurationChanged(int duration);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
};

#endif // MEDIACONTROLLER_H
