#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QMediaFormat>
#include <QAudioDevice>
#include <QMediaDevices>
#include <QProcess>
#include "mediacontroller.h"
#include "metadatamanager.h"
#include "playlistmodel.h"
#include "playlistmanager.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    PlaylistModel playlistModel;
    PlaybackController playlistManager(&playlistModel);
    MetadataManager metadataManager;
    MediaController mediaController(&metadataManager, &playlistManager);

    engine.rootContext()->setContextProperty("mediaController", &mediaController);
    engine.rootContext()->setContextProperty("metadataManager", &metadataManager);
    engine.rootContext()->setContextProperty("playlistmodel", &playlistModel);
    engine.rootContext()->setContextProperty("playlistmanager", &playlistManager);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("MediaPlayer", "Main");

    return app.exec();
}
