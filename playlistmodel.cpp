#include "playlistmodel.h"
#include <QDebug>
#include <QFileInfo>

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractListModel(parent) {
    loadPlaylist();
    currentIndex = 0;
}

int PlaylistModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return playlist.count();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const {
    if(!index.isValid() || index.row() >= playlist.size()) return QVariant();

    const SongItem &song = playlist.at(index.row());

    switch (role)
    {
        case FilePathRole: return song.filePath;
        case FileNameRole: return song.fileName;
    }
    return QVariant();
}

QHash<int, QByteArray> PlaylistModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[FilePathRole] = "filePath";
    roles[FileNameRole] = "fileName";
    return roles;
}

int PlaylistModel::songCount() const
{
    return playlist.size();
}

int PlaylistModel::getCurrentIndex() const
{
    return currentIndex;
}

void PlaylistModel::setCurrentIndex(int index)
{
    if(index >= 0 && index < playlist.size() && currentIndex != index)
    {
        currentIndex = index;
        emit currentIndexChanged(currentIndex);
        // emit dataChanged(createIndex(currentIndex, 0), createIndex(currentIndex, 0));
        emit dataChanged(createIndex(0, 0), createIndex(playlist.size()-1, 0));
    }
}

void PlaylistModel::loadPlaylist()
{
    QString songPath = "C:/Users/PANDYA MIHIR/Downloads/Music";
    QDir SongsDir(songPath);
    QStringList filters;
    filters << "*.mp3";
    QStringList songList = SongsDir.entryList(filters, QDir::Files);

    for(auto &song : songList)
    {
        playlist.append({songPath + "/" + song, song});
    }
}

QString PlaylistModel::getSongAt(int index) const
{
    if (index >= 0 && index < playlist.size())
    {
        return playlist.at(index).filePath;
    }
    return QString();
}
