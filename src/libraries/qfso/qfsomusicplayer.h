/*
 * This file was generated by qfsodbusxml2cpp version 0.7
 * Command line was: qfsodbusxml2cpp -p qfsomusicplayer -c QFsoMusicPlayer specs/xml/org.freesmartphone.MusicPlayer.xml
 *
 * qfsodbusxml2cpp is Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#ifndef QFSOMUSICPLAYER_H_1306329414
#define QFSOMUSICPLAYER_H_1306329414

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>

/*
 * Proxy class for interface org.freesmartphone.MusicPlayer
 */
class QFsoMusicPlayer: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    { return "org.freesmartphone.MusicPlayer"; }

public:
    QFsoMusicPlayer(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent = 0);

    ~QFsoMusicPlayer();

public Q_SLOTS: // METHODS
    inline QDBusPendingReply<> DeletePlaylist(const QDBusObjectPath &list)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(list);
        return asyncCallWithArgumentList(QLatin1String("DeletePlaylist"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> GetCurrentPlaylist()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetCurrentPlaylist"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetInfoForFile(const QString &file)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(file);
        return asyncCallWithArgumentList(QLatin1String("GetInfoForFile"), argumentList);
    }

    inline QDBusPendingReply<QString> GetPlaying()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetPlaying"), argumentList);
    }

    inline QDBusPendingReply<QVariantMap> GetPlayingInfo()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetPlayingInfo"), argumentList);
    }

    inline QDBusPendingReply<QStringList> GetPlaylists()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetPlaylists"), argumentList);
    }

    inline QDBusPendingReply<int> GetVolume()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("GetVolume"), argumentList);
    }

    inline QDBusPendingReply<> Jump(int position)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(position);
        return asyncCallWithArgumentList(QLatin1String("Jump"), argumentList);
    }

    inline QDBusPendingReply<QDBusObjectPath> NewPlaylist(const QString &name)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(name);
        return asyncCallWithArgumentList(QLatin1String("NewPlaylist"), argumentList);
    }

    inline QDBusPendingReply<> Next()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Next"), argumentList);
    }

    inline QDBusPendingReply<> Pause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Pause"), argumentList);
    }

    inline QDBusPendingReply<> Play()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Play"), argumentList);
    }

    inline QDBusPendingReply<> PopPause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("PopPause"), argumentList);
    }

    inline QDBusPendingReply<> Previous()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Previous"), argumentList);
    }

    inline QDBusPendingReply<> PushPause()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("PushPause"), argumentList);
    }

    inline QDBusPendingReply<> SeekBackward(int step)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(step);
        return asyncCallWithArgumentList(QLatin1String("SeekBackward"), argumentList);
    }

    inline QDBusPendingReply<> SeekForward(int step)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(step);
        return asyncCallWithArgumentList(QLatin1String("SeekForward"), argumentList);
    }

    inline QDBusPendingReply<> SetCurrentPlaylist(const QDBusObjectPath &list)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(list);
        return asyncCallWithArgumentList(QLatin1String("SetCurrentPlaylist"), argumentList);
    }

    inline QDBusPendingReply<> SetPlaying(const QString &file)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(file);
        return asyncCallWithArgumentList(QLatin1String("SetPlaying"), argumentList);
    }

    inline QDBusPendingReply<> SetVolume(int volume)
    {
        QList<QVariant> argumentList;
        argumentList << qVariantFromValue(volume);
        return asyncCallWithArgumentList(QLatin1String("SetVolume"), argumentList);
    }

    inline QDBusPendingReply<> Stop()
    {
        QList<QVariant> argumentList;
        return asyncCallWithArgumentList(QLatin1String("Stop"), argumentList);
    }

Q_SIGNALS: // SIGNALS
    void Error(const QString &the_error, const QString &info);
    void PlayingChanged(const QString &file);
    void PlaylistAdded(const QDBusObjectPath &path);
    void PlaylistRemoved(const QDBusObjectPath &path);
    void Progress(int progress);
    void State(const QString &state);
};

namespace org {
  namespace freesmartphone {
    typedef ::QFsoMusicPlayer MusicPlayer;
  }
}
#endif
