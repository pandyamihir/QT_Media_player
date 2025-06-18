#include "mediacontroller.h"
#include "metadatamanager.h"
#include "playlistmanager.h"
#include <QTimer>

MediaController::MediaController(MetadataManager *meta, PlaybackController *playlist, QObject *parent)
    : QObject{parent}, metaData(meta), playlistManager(playlist)  // Keep this line as is
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    m_currentPos = 0;
    m_totalDuration = 0;

    metaData->setMediaPlayer(player);
    playlistManager->setMediaPlayer(player);

    player->setAudioOutput(audioOutput);
    player->setSource(QUrl::fromLocalFile("C:/Users/PANDYA MIHIR/Downloads/Music/Aankhon Me Teri.mp3"));
    audioOutput->setVolume(50);
    audioOutput->setVolume(m_volume / 100.0f); // Convert to 0-1 range

    connect(player, &QMediaPlayer::positionChanged, this, &MediaController::onPositionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MediaController::onDurationChanged);
    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MediaController::onMediaStatusChanged);
}

void MediaController::play()
{
    player->play();
}

void MediaController::pause()
{
    player->pause();
}

void MediaController::stop()
{
    player->stop();
}

int MediaController::currentPos() const
{
    return m_currentPos;
}

int MediaController::totalDuration() const
{
    return m_totalDuration;
}

void MediaController::onPositionChanged(int pos)
{
    m_currentPos = pos;
    emit currentPosChanged();
}

void MediaController::onDurationChanged(int duration)
{
    m_totalDuration = duration;
    emit totalDurationChanged();
}

void MediaController::setCurrentPos(int newCurrentPos)
{
    if (m_currentPos == newCurrentPos)
        return;
    m_currentPos = newCurrentPos;
    player->setPosition(m_currentPos);
    emit currentPosChanged();
}

int MediaController::volume() const
{
    return m_volume;
}

bool MediaController::isMuted() const
{
    return m_muted;
}

void MediaController::setVolume(int volume)
{
    volume = qBound(0, volume, 100);
    if (m_volume == volume)
        return;

    m_volume = volume;
    audioOutput->setVolume(m_volume / 100.0f);
    emit volumeChanged();
}

void MediaController::setMuted(bool muted)
{
    if (m_muted == muted)
        return;

    m_muted = muted;
    audioOutput->setMuted(m_muted);
    emit mutedChanged();
}

void MediaController::volumeUp()
{
    setVolume(qMin(m_volume + 10, 100));
}

void MediaController::volumeDown()
{
    setVolume(qMax(m_volume - 10, 0));
}

void MediaController::toggleMute()
{
    setMuted(!m_muted);
}

void MediaController::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        QTimer::singleShot(1500, this, [this]() {  // 1.5 second delay
            playlistManager->nextSong();
        });
    }
}
