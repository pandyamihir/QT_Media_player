#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QTextStream>

struct SongItem {
    QString filePath;
    QString fileName; // Wxtracted from filepath for display
};

class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
public:
    Q_INVOKABLE QString getSongAt(int index) const;
    enum Roles {
        FilePathRole = Qt::UserRole + 1,
        FileNameRole
    };

    explicit PlaylistModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Q_INVOKABLE void addSong(const QString &filePath);
    // Q_INVOKABLE void removeSong(int index);
    // Q_INVOKABLE void clearPlaylist();
    // Q_INVOKABLE QString getSongAt(int index) const;
    Q_INVOKABLE int songCount() const;
    Q_INVOKABLE void setCurrentIndex(int index);

    int getCurrentIndex() const;

    void loadPlaylist();
    void savePlaylist();

signals:
    void playlistUpdated();
    void currentIndexChanged(int index);

private:
    QList<SongItem> playlist;
    const QString playlistFile = "playlist.txt";
    int currentIndex;
};

#endif // PLAYLISTMODEL_H
