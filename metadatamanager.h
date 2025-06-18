#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaMetaData>
#include <QImage>
#include <QBuffer>

class MetadataManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString songTitle READ songTitle NOTIFY metadataChanged)
    Q_PROPERTY(QString songArtist READ songArtist NOTIFY metadataChanged)
    Q_PROPERTY(QString songAlbum READ songAlbum NOTIFY metadataChanged)
    Q_PROPERTY(QString albumArt READ albumArt NOTIFY albumArtUpdated)

public:
    explicit MetadataManager(QObject *parent = nullptr);
    void setMediaPlayer(QMediaPlayer *player);

    QString songTitle() const;
    QString songArtist() const;
    QString songAlbum() const;
    QString albumArt() const;

public slots:
    void updateMetadata();

signals:
    // void songTitleChanged();
    // void songArtistChanged();
    // void songAlbumChanged();
    void metadataChanged();
    void albumArtUpdated();

private:
    QMediaPlayer *player;
    QString m_albumArt;
};

#endif // METADATAMANAGER_H
