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

#include "playsongarea.h"
#include "xatom-helper.h"
#include <QObject>
#include <QWidget>

PlaySongArea::PlaySongArea(QWidget *parent) : QWidget(parent)
{

    setFixedHeight(68);


    mybeforeList = new BeforePlayList(parent);
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(mybeforeList->winId(), hints);
    mybeforeList->hide();

    initWidget();
    playcolor();
}

void PlaySongArea::playcolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        setStyleSheet("background-color:#252526;");

//        lastBtn->setIcon(QIcon(":/img/dark/lastsong.png"));
        lastBtn->setIconSize(QSize(18,18));
        lastBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/lastsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");

//        playBtn->setIcon(QIcon(":/img/default/play2.png"));
        playBtn->setIconSize(QSize(36,36));
        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

//        nextBtn->setIcon(QIcon(":/img/dark/nextsong.png"));
        nextBtn->setIconSize(QSize(18,18));
        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/dark/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

//        volumeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/dark/icon_volume_large_w@2x.png)}");
//        volumeBtn->setIcon(QIcon(":/img/dark/icon_volume_large_w@2x.png"));

//        likeMusicBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/dark/icon_love2_b@2x.png)}");
        likeMusicBtn->setIcon(QIcon(":/img/dark/icon_love2_b@2x.png"));
        playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
        playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                   "QPushButton::hover{border-image:url(:/img/hover/sequence.png);}"
                                   "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");
//        playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/dark/icon_sequence_w@2x.png);}"
//                                   "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}");
//        playModeBtn->setIcon(QIcon(":/img/dark/icon_sequence_w@2x.png"));


        listBtn->setIcon(QIcon(":/img/dark/icon_songlist_h@2x.png"));
//        listBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/dark/icon_songlist_h@2x.png)}"
//                               "QPushButton::hover{border-image:url(:/img/clicked/songlist.png);}"
//                               "QPushButton::checked{border-image:url(:/img/clicked/songlist.png);}");

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        songNameofNowPlaying->setStyleSheet("width:56px;height:14px;font-size:14px;color:#F9F9F9;line-height:14px;");
    }
    else if (WidgetStyle::themeColor == 0)
    {
        setStyleSheet("QWidget{Background-color:#FFFFFF;}");

//        lastBtn->setIcon(QIcon(":/img/default/lastsong.png"));
        lastBtn->setIconSize(QSize(18,18));
        lastBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/lastsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/lastsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/lastsong.png);}");

//        playBtn->setIcon(QIcon(":/img/default/play2.png"));
        playBtn->setIconSize(QSize(36,36));
        playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

//        closeBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
//                                border-image:url(:/img/default/close.png);}\
//                                QPushButton::hover{border-image:url(:/img/hover/close.png);\
//                                                   background-color:#F86457;}\
//                                QPushButton:pressed{border-image:url(:/img/clicked/close.png);background-color:#E44C50;}");

//        nextBtn->setIcon(QIcon(":/img/default/nextsong.png"));
        nextBtn->setIconSize(QSize(18,18));
        nextBtn->setStyleSheet("QPushButton{background:transparent;border-radius:15px;border-image:url(:/img/default/nextsong.png);}"
                               "QPushButton::hover{border-image:url(:/img/hover/nextsong.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/nextsong.png);}");

        volumeBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;}");

        likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                    "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                    "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");

        playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
        playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                   "QPushButton::hover{border-image:url(:/img/hover/sequence.png);}"
                                   "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");

        listBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/songlist.png)}"
                               "QPushButton::hover{border-image:url(:/img/clicked/songlist.png);}"
                               "QPushButton::checked{border-image:url(:/img/clicked/songlist.png);}");

        coverPhotoLabel->setStyleSheet("background:transparent;border-image:url(:/img/fengmian.png);");

        songNameofNowPlaying->setStyleSheet("width:56px;height:14px;font-size:14px;color:#303133;line-height:14px;");

    }
}


void PlaySongArea::initWidget()
{
    h_mainLayout = new QHBoxLayout(this);


    lastBtn = new QPushButton;
    lastBtn->setFixedSize(18,18);
    lastBtn->setCursor(Qt::PointingHandCursor);
//    lastBtn->setToolTip(" 上一首");
    lastBtn->setToolTip(tr(" On a"));


    playBtn = new QPushButton(this);
    playBtn->setFixedSize(36,36);
    playBtn->setCursor(Qt::PointingHandCursor);
//    playBtn->setToolTip("播放");
//    playBtn->setToolTip(tr("play"));


    nextBtn = new QPushButton;
    nextBtn->setFixedSize(18,18);
    nextBtn->setCursor(Qt::PointingHandCursor);
//    nextBtn->setToolTip("下一首");
    nextBtn->setToolTip(tr("The following piece"));


    volumeBtn = new QPushButton;
    volumeBtn->setFixedSize(16,16);
    volumeBtn->setCursor(Qt::PointingHandCursor);
    volumeBtn->setIcon(QIcon::fromTheme("audio-volume-high-symbolic"));
    volumeBtn->hide();
//    volumeBtn->setToolTip(" 静音 ");
//    volumeBtn->setToolTip(" mute ");
//    volumeBtn->setToolTip(" 功能未实现 ");
//    volumeBtn->setToolTip(tr(" Function not implemented "));

//    volumeBtn->hide();

//    volumeSlider = new QSlider;
//    volumeSlider->setOrientation(Qt::Horizontal);
//    volumeSlider->setFixedSize(100,12);
//    volumeSlider->setToolTip(" 音量调节");
//    volumeSlider->setMinimum(0);
//    volumeSlider->setMaximum(100);
//    volumeSlider->setSingleStep(1);
//    volumeSlider->setStyleSheet("QSlider::groove:horizontal{height: 4px;background: rgb(102,183,255);}\
//                                 QSlider::add-page:horizontal{background:#c2c2c4;}\
//                                 QSlider::handle:horizontal{width: 12px;background: url(:/img/default/vol_large.png);margin: -4 0 -4 0}");
//    volumeSlider->installEventFilter(this);

    likeMusicBtn = new QPushButton;
    likeMusicBtn->setFixedSize(16,16);
    likeMusicBtn->setCursor(Qt::PointingHandCursor);
//    likeMusicBtn->setToolTip("我喜欢");
//    likeMusicBtn->hide();
    likeMusicBtn->setToolTip(tr("I like"));
//    likeMusicBtn->setToolTip(" 功能未实现");
//    likeMusicBtn->setToolTip(tr(" Function not implemented"));


    playModeBtn = new QPushButton;
    playModeBtn->setFixedSize(16,16);
    playModeBtn->setCursor(Qt::PointingHandCursor);
//    playModeBtn->setToolTip(tr(" Order of play "));   //顺序播放
//    playModeBtn->setToolTip(tr(" 顺序播放 "));   //顺序播放
    playModeBtn->setToolTip(tr(" Order of play "));   //顺序播放
    playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
    playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                               "QPushButton::hover{border-image:url(:/img/hover/sequence.png);}"
                               "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");

    listBtn = new QPushButton;   //历史播放列表
    listBtn->setFixedSize(16,16);
    listBtn->setCheckable(true);
    listBtn->setChecked(false);
    listBtn->setCursor(Qt::PointingHandCursor);

//    listBtn->setToolTip(" 播放列表 ");
    listBtn->setToolTip(tr(" The playlist "));


    QWidget *bottomLeftWid = new QWidget(this);
    QWidget *playCenterWid = new QWidget(this);
    QWidget *bottmRightWid = new QWidget(this);

    bottomLeftLabel = new QLabel(this);
    bottomLeftLabel->setFixedHeight(18);
    bottomLeftLabel->setStyleSheet("font-size:13px;color:#8F9399;");
    bottomLeftLabel->setAttribute(Qt::WA_TranslucentBackground);

    coverPhotoLabel = new QLabel(this);
    coverPhotoLabel->setFixedSize(40,40);
//    coverPhotoLabel->setAttribute(Qt::WA_TranslucentBackground);


    QHBoxLayout *bottomLeftLayout = new QHBoxLayout(this);
    QHBoxLayout *playCenterLayout = new QHBoxLayout(this);
    QHBoxLayout *bottmRightLayout = new QHBoxLayout(this);
    QVBoxLayout *timeAndNameLayout = new QVBoxLayout(this);

    songNameofNowPlaying = new QLabel(this);
    QWidget *timeAndNameWidget = new QWidget(this);
    timeAndNameWidget->setFixedHeight(40);

    songNameofNowPlaying->setFixedHeight(20);

//    songNameofNowPlaying->hide();
    timeAndNameLayout->addWidget(songNameofNowPlaying,0,Qt::AlignTop);
    timeAndNameLayout->addWidget(bottomLeftLabel,Qt::AlignTop);

    timeAndNameLayout->setMargin(0);
    timeAndNameLayout->setSpacing(5);

    timeAndNameWidget->setLayout(timeAndNameLayout);

    bottomLeftLayout->addWidget(coverPhotoLabel);
    bottomLeftLayout->addWidget(timeAndNameWidget);
    bottomLeftLayout->setContentsMargins(30,0,0,0);

    playCenterLayout->addWidget(lastBtn,0,Qt::AlignRight);
    playCenterLayout->addSpacing(10);
    playCenterLayout->addWidget(playBtn,Qt::AlignRight);
    playCenterLayout->addSpacing(10);
    playCenterLayout->addWidget(nextBtn,Qt::AlignRight);

    bottmRightLayout->addWidget(volumeBtn,0,Qt::AlignRight);
    bottmRightLayout->addSpacing(6);
    bottmRightLayout->addWidget(likeMusicBtn,Qt::AlignRight);
    bottmRightLayout->addSpacing(6);
    bottmRightLayout->addWidget(playModeBtn,Qt::AlignRight);
    bottmRightLayout->addSpacing(6);
    //暂时隐藏历史播放列表
    bottmRightLayout->addWidget(listBtn,Qt::AlignRight);

    bottomLeftWid->setLayout(bottomLeftLayout);
    playCenterWid->setLayout(playCenterLayout);
    bottmRightWid->setLayout(bottmRightLayout);

    h_mainLayout->addWidget(bottomLeftWid,0,Qt::AlignLeft);
//    h_mainLayout->addSpacing(85);
    h_mainLayout->addWidget(playCenterWid,0,Qt::AlignCenter);
//    h_mainLayout->addWidget(lastBtn,Qt::AlignCenter);
//    h_mainLayout->addSpacing(10);
//    h_mainLayout->addWidget(playBtn,Qt::AlignCenter);
//    h_mainLayout->addSpacing(10);
//    h_mainLayout->addWidget(nextBtn,Qt::AlignCenter);

    h_mainLayout->addWidget(bottmRightWid,0,Qt::AlignRight);
//    h_mainLayout->addWidget(volumeBtn,0,Qt::AlignRight);
////    h_mainLayout->addWidget(volumeSlider,Qt::AlignVCenter);
//    h_mainLayout->addSpacing(6);
//    h_mainLayout->addWidget(likeMusicBtn,Qt::AlignRight);
//    h_mainLayout->addSpacing(6);
//    h_mainLayout->addWidget(playModeBtn,Qt::AlignRight);
//    h_mainLayout->addSpacing(6);
//    h_mainLayout->addWidget(listBtn,Qt::AlignRight);
    h_mainLayout->setMargin(10);
    h_mainLayout->setSpacing(10);
    h_mainLayout->setContentsMargins(0,0,30,0);
//    connect(playBtn,SIGNAL(clicked(bool)),this,SLOT(playSong(bool)));   //播放歌曲

}
//void PlaySongArea::playSong(bool)
//{
//    if(playstates)
//    {
//        playBtn->setFixedSize(36,36);
//        playBtn->setCursor(Qt::PointingHandCursor);
//        playBtn->setToolTip(" 播放");
//        playBtn->setIcon(QIcon(":/img/default/play2.png"));
//        playBtn->setIconSize(QSize(36,36));
//        playBtn->setStyleSheet("QPushButton{border-radius:17px}");
//        this->PauseSong();
//        playstates = false;

//    }
//    else
//    {
//        playBtn->setFixedSize(36,36);
//        playBtn->setCursor(Qt::PointingHandCursor);
//        playBtn->setToolTip(" 暂停");
//        playBtn->setIcon(QIcon(":/img/default/pause2.png"));
//        playBtn->setIconSize(QSize(36,36));
//        playBtn->setStyleSheet("QPushButton{border-radius:17px}");
////        this->playSong(bool);
//        playstates = true;
//    }
//}

void PlaySongArea::play_Song()   //播放和暂停
{
    if(Music->state() == QMediaPlayer::PlayingState)
    {
        qDebug()<<"暂停 暂停";
        Music->pause();
    }
    else
    {
        qDebug()<<"播放 播放";
        Music->play();
    }
}

void PlaySongArea::songText(QString songName)
{
    QString show_songName = "";
    if(songName.length() > 10)
    {
        show_songName = songName.mid(0,9);
        show_songName.append("...");
        songNameofNowPlaying->setText(show_songName);
        songNameofNowPlaying->setToolTip(songName);
    }
    else
    {
        songNameofNowPlaying->setText(songName);
        songNameofNowPlaying->setToolTip(songName);
    }
}
