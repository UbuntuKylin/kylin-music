/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
//#include <QScrollBar>
#include <QHeaderView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QCheckBox>

#include "beforeplaylist.h"
#include "widgetstyle.h"
#include "allpupwindow.h"

class PlaySongArea : public QWidget
{
    Q_OBJECT

public:
    explicit PlaySongArea(QWidget *parent = nullptr);
//    QWidget *m_controlFrame;
    QPushButton *lastBtn, *playBtn, *nextBtn,*playModeBtn,*lyricBtn,*volumeBtn;
    QPushButton  *listBtn;
    //上一首 播放 下一首 音量 播放顺序 歌词  历史播放列表

    QPushButton *likeMusicBtn;
    QLabel *bottomLeftLabel;//歌曲时间
//    QSlider *playTimeSlider, *volumeSlider;
    bool playstates = false;

    QLabel *coverPhotoLabel;  //歌曲封面
    QStringList sqlFilenameList;
    QMediaPlaylist *PlayList;
    QMediaPlayer *Music;
//    int playMode = 0;

    QString MusicName;
    QLabel *songNameofNowPlaying;

    BeforePlayList *mybeforeList;
    void playcolor();
    void songText(QString songName);   // 正在播放

public slots:
//    void playSong(bool);
    void play_Song();   //播放和暂停


private:
    void initWidget();
    QHBoxLayout *h_mainLayout, *m_hControlLayout;


};

#endif // PLAYSONGAREA_H
