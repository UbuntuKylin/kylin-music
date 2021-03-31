#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>

enum PlayState {
    STOP_STATE = 0,
    PLAY_STATE = 1,
    PAUSED_STATE = 2
};

enum PlayMode {
    CurrentItemOnce = 0,
    CurrentItemInLoop = 1,
    Sequential = 2,
    Loop = 3,
    Random = 4
};
class playController : public QObject {
    Q_OBJECT
public:
    static playController& getInstance()
    {
        static playController instance;
        return instance;
    }

private:
    playController(/* args */);
    playController(playController const&);
    playController& operator=(playController const&);
    ~playController();

public:
    bool play(QString playlist, int index);

    bool play();
    bool pause();
    bool stop();

    void setVolume(int vol);
    int volume();

    void setPlayPosition(int pos);
    int playPosition();

    void setSongIndex(int index);
    int songIndex();

    int playmode();
    void setPlaymode(int mode);

    void curPlaylist();
    void setCurPlaylist(QString name, QStringList songPaths);
    void addSongToCurList(QString name, QString songPath);
    void removeSongFromCurList(QString name, int index);

    QMediaPlayer* getPlayer()
    {
        return m_player;
    }
    QMediaPlaylist* getPlaylist()
    {
        return m_playlist;
    }
signals:
    void curPositionChanged(qint64);
    void curDurationChanged(qint64);
    void curIndexChanged(int index);
    void playerError(int error, QString errMsg);
    void playerStateChange(int state);

public slots:
    void onCurrentIndexChanged();
    void onPositionChanged(double value);
    void onNextSong();
    void onPreviousSong();

private slots:
    void onError();
    void onMediaStatusChanged();

private:
    QString m_curList;
    int m_curIndex;
    QMediaPlayer* m_player;
    QMediaPlaylist* m_playlist;
    bool isInitialed = false;
};

#endif // PLAYER_H
