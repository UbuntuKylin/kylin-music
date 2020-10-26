#ifndef PLAYSONGAREA_H
#define PLAYSONGAREA_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QLineEdit>
#include <QTableWidget>
#include <QScrollBar>
#include <QHeaderView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class PlaySongArea : public QWidget
{
    Q_OBJECT

public:
    explicit PlaySongArea(QWidget *parent = nullptr);
//    QWidget *m_controlFrame;
    QPushButton *lastBtn, *playBtn, *nextBtn, *volumeBtn, *playModeBtn, *lyricBtn, *listBtn;
    QPushButton *likeMusicBtn;
    QLabel *bottomLeftLabel;
//    QSlider *playTimeSlider, *volumeSlider;
    bool playstates = false;

    QLabel *coverPhotoLabel;
    QStringList sqlFilenameList;
    QMediaPlaylist *PlayList;
    QMediaPlayer *Music;
//    int playMode = 0;

    QString MusicName;
    QLabel *songNameofNowPlaying;

public slots:
//    void playSong(bool);
    void play_Song();   //播放和暂停

private:
    void initWidget();
    QHBoxLayout *h_mainLayout, *m_hControlLayout;


};

#endif // PLAYSONGAREA_H
