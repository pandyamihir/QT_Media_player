#include "metadatamanager.h"

MetadataManager::MetadataManager(QObject *parent) : QObject(parent), player(nullptr) {}

void MetadataManager::setMediaPlayer(QMediaPlayer *player)
{
    this->player = player;
    connect(player, &QMediaPlayer::metaDataChanged, this, &MetadataManager::updateMetadata);
}

void MetadataManager::updateMetadata()
{
    if(!player) return;
    emit metadataChanged();
    emit albumArtUpdated();
}

QString MetadataManager::songTitle() const
{
    return player ? player->metaData().stringValue(QMediaMetaData::Title) : "Unknown Title";
}

QString MetadataManager::songArtist() const
{
    return player ? player->metaData().stringValue(QMediaMetaData::AlbumArtist) : "Unknown Artist";
}

QString MetadataManager::songAlbum() const
{
    return player ? player->metaData().stringValue(QMediaMetaData::AlbumTitle) : "Unknown Album";
}

QString MetadataManager::albumArt() const
{
    if(player && !player->metaData().value(QMediaMetaData::ThumbnailImage).isNull())
    {
        QImage image = player->metaData().value(QMediaMetaData::ThumbnailImage).value<QImage>();
        QByteArray byteArray;
        QBuffer buffer(&byteArray);
        image.save(&buffer, "PNG");
        qDebug() << "Valid cover image available";

        return "data:image/png;base64," + byteArray.toBase64();
    }
    qDebug() << "No Valid cover image available";
    return "qrc:/Icons/Default.jpeg";
}

