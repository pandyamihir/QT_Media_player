#include "playlistmanager.h"
#include <QDebug>

PlaybackController::PlaybackController(PlaylistModel *model, QObject *parent)
    : QObject(parent), playlistModel(model), player(nullptr), shuffleMode(false), loopMode(0) {

    connect(playlistModel, &PlaylistModel::currentIndexChanged, this, &PlaybackController::setSongIndex);
}

void PlaybackController::setMediaPlayer(QMediaPlayer *player)
{
    this->player = player;
    player->setSource(playlistModel->getSongAt(0));
}

void PlaybackController::nextSong() {
    if(!playlistModel || !player) return;

    int songCount = playlistModel->songCount();
    if(songCount == 0) return;

    int currentIndex = playlistModel->getCurrentIndex();
    int nextIndex;

    if(shuffleMode) {
        do {
            nextIndex = QRandomGenerator::global()->bounded(songCount);
        } while (nextIndex == currentIndex && songCount > 1); // Avoid same song if possible
    }
    else if(loopMode == 1) { // Loop One
        nextIndex = currentIndex;
    }
    else { // Normal or Loop All
        nextIndex = (currentIndex + 1) % songCount;
    }

    qDebug() << "Cur = " << currentIndex << "Next = " << nextIndex;
    playlistModel->setCurrentIndex(nextIndex);
    // QString nextSongpath = playlistModel->getSongAt(nextIndex);
    // emit songChanged(nextSongpath);
    setSongIndex(nextIndex);
}

void PlaybackController::previousSong() {
    if (!playlistModel || !player) return;

    int songCount = playlistModel->songCount();
    if(songCount == 0) return;

    int currentIndex = playlistModel->getCurrentIndex();
    int prevIndex = (currentIndex - 1 + songCount) % songCount;

    qDebug() << "Cur = " << currentIndex << "Prev = " << prevIndex;
    playlistModel->setCurrentIndex(prevIndex);

    QString prevSongPath = playlistModel->getSongAt(prevIndex);
    player->setSource(prevSongPath);
    player->play();
    emit songChanged(prevSongPath);
}

void PlaybackController::setSongIndex(int index)
{
    if (!playlistModel || !player) return;

    if (index >= 0 && index < playlistModel->songCount())
    {
        playlistModel->setCurrentIndex(index);
        QString songPath = playlistModel->getSongAt(index);
        player->setSource(QUrl::fromLocalFile(songPath));
        player->play();
        emit songChanged(songPath);
        emit currentSongIndexChanged(index); // Add this line
    }
}

void PlaybackController::toggleLoopMode()
{
    setLoopMode((loopMode + 1) % 3); // Cycles through 0, 1, 2
}

void PlaybackController::toggleShuffle()
{
    setShuffleMode(!shuffleMode);
}

bool PlaybackController::isShuffleMode() const {
    return shuffleMode;
}

void PlaybackController::setShuffleMode(bool enabled) {
    if(shuffleMode != enabled)
    {
        shuffleMode = enabled;
        emit shuffleModeChanged();
    }
}

int PlaybackController::getLoopMode() const {
    return loopMode;
}

void PlaybackController::setLoopMode(int mode) {
    if(mode >= 0 && mode <= 2 && loopMode != mode) {
        loopMode = mode;
        emit loopModeChanged();
    }
}
