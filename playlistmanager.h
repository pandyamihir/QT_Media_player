#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include "playlistmodel.h"

class PlaybackController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool shuffleMode READ isShuffleMode WRITE setShuffleMode NOTIFY shuffleModeChanged)
    Q_PROPERTY(int loopMode READ getLoopMode WRITE setLoopMode NOTIFY loopModeChanged)

public:
    explicit PlaybackController(PlaylistModel *model = nullptr, QObject *parent = nullptr);

    void setMediaPlayer(QMediaPlayer *player);

    Q_INVOKABLE void nextSong();
    Q_INVOKABLE void previousSong();
    Q_INVOKABLE void toggleShuffle();
    Q_INVOKABLE void toggleLoopMode();
    Q_INVOKABLE void setShuffleMode(bool enabled);

    bool isShuffleMode() const;
    void setLoopMode(int mode);

    int getLoopMode() const;
    // void setLoopMode(int mode);

public slots:
    void setSongIndex(int index);

signals:
    void shuffleModeChanged();
    void loopModeChanged();
    void songChanged(QString songPath);
    void currentSongIndexChanged(int index); // Add this line

private:
    PlaylistModel *playlistModel;
    QMediaPlayer *player;

    bool shuffleMode;
    int loopMode; // 0 = No Loop, 1 = Loop One, 2 = Loop All
};

#endif // PLAYLISTMANAGER_H
