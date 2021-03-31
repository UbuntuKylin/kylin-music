#include <QDebug>
#include "player.h"

bool playController::play(QString playlist, int index)
{
    if (playlist.compare(m_curList)==0) {
        if (index == m_curIndex) {
            if (m_player->state() == QMediaPlayer::State::PlayingState)
            {
                play();
            }else {
                pause();
            }
        }
        else {
            stop();
            setSongIndex(index);
            play();
        }
        return true;
    }
    return false;
}

bool playController::play()
{
    if (m_player == nullptr) {
        return false;
    }
    if (m_player->isAvailable() == false) { //存疑
        return false;
    }
    if (m_player->state() == QMediaPlayer::State::PlayingState) {
        m_player->pause();
    } else {
        m_player->play();
        emit curIndexChanged(m_playlist->currentIndex());
    }

    return true;
}
bool playController::pause()
{
    return true;
}
bool playController::stop()
{
    if (m_player == nullptr) {
        return false;
    }
    m_player->stop();

    return true;
}
void playController::setSongIndex(int index)
{
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return;
    }

    if (index > m_playlist->mediaCount()) {
        return;
    }
    m_playlist->setCurrentIndex(index);
    emit curIndexChanged(index);
}
int playController::songIndex()
{
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return -1;
    }

    return m_playlist->currentIndex();
}

int playController::playmode()
{
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return -1;
    }

    return static_cast<QMediaPlaylist::PlaybackMode>(m_playlist->playbackMode());
}
void playController::setPlaymode(int mode)
{
    if (m_playlist == nullptr || m_player == nullptr) {
        return;
    }
    if (m_playlist == nullptr) {
        qDebug() << "m_playlist is null";
        return;
    }

    if (mode < 0 || mode > 5) {
        m_playlist->setPlaybackMode(QMediaPlaylist::PlaybackMode::CurrentItemOnce);
    } else
        m_playlist->setPlaybackMode(static_cast<QMediaPlaylist::PlaybackMode>(mode));
}

void playController::curPlaylist()
{
    // Print Playlist information
    qDebug() << "Current playlist name:" << m_curList;
    if (m_playlist == nullptr) {
        return;
    }

    for (auto i = 0; i < m_playlist->mediaCount(); i++) {
        QMediaContent content = m_playlist->media(i);
        qDebug() << "   "
                 << "media[" << i << "] is:" << content.canonicalUrl();
    }
}
void playController::setCurPlaylist(QString name, QStringList songPaths)
{
    if (m_curList.compare(name)==0)
    {
        return ;
    }

    if (m_playlist == nullptr || m_player == nullptr) {
        return;
    }

    m_curList = name;
    m_playlist->clear();

    for (auto path : songPaths) {
        m_playlist->addMedia(QUrl::fromLocalFile(path));
    }

    if (m_player != nullptr) {
        m_player->setPlaylist(m_playlist);
    }
    m_player->stop();
    isInitialed = true;
}
void playController::addSongToCurList(QString name, QString songPath)
{
    if (name.compare(m_curList) != 0) {
        qDebug() << __FUNCTION__ << " the playlist to add is not Current playlist.";
        return;
    }
    if (m_playlist != nullptr) {
        m_playlist->addMedia(QUrl::fromLocalFile(songPath));
    }
}
void playController::removeSongFromCurList(QString name, int index)
{
    if (name.compare(m_curList) != 0) {
        qDebug() << __FUNCTION__ << " the playlist to add is not Current playlist.";
        return;
    }
    if (m_playlist != nullptr) {
        m_playlist->removeMedia(index);
    }
}
playController::playController()
    : m_curList(""),m_curIndex(-1)
{
    qDebug() << "musicPlayer Create..";
    m_player = new QMediaPlayer(this);
    if (m_player == nullptr) {
        qDebug() << "failed to create player ";
        return;
    }
    m_playlist = new QMediaPlaylist(m_player);
    if (m_playlist == nullptr) {
        qDebug() << "failed to create laylist";
        return;
    }
    m_player->setPlaylist(m_playlist);
}
void playController::onCurrentIndexChanged()
{
    qDebug() << "onCurrentIndexChanged";
}
void playController::onPositionChanged(double value)
{
    qDebug() << "onPositionChanged";
    if (m_player == nullptr) {
        return;
    }
    m_player->setPosition(m_player->duration() * value);
}
void playController::onNextSong()
{
    if (m_playlist == nullptr || m_player == nullptr) {
        qDebug() << "m_playlist or m_player is nullptr";
        return;
    }
    m_playlist->next();
    auto index = m_playlist->currentIndex();
    emit curIndexChanged(index);
}
void playController::onPreviousSong()
{
    if (m_playlist == nullptr || m_player == nullptr) {
        qDebug() << "m_playlist or m_player is nullptr";
        return;
    }
    m_playlist->previous();
    auto index = m_playlist->currentIndex();
    emit curIndexChanged(index);
}
void playController::onError()
{
    qDebug() << "onError";
}
void playController::onMediaStatusChanged()
{
    qDebug() << "onMediaStatusChanged";
}
playController::~playController(/* args */)
{
    qDebug() << "musicPlayer destroy..";
    if (m_playlist != nullptr) {
        m_playlist->deleteLater();
    }
    if (m_player != nullptr) {
        m_player->deleteLater();
    }
}
