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

#include "songitem.h"

SongItem::SongItem(QWidget *parent):QWidget(parent)
{
    setFixedHeight(40);


    setAttribute(Qt::WA_TranslucentBackground, true);

//    setStyleSheet("background:red;");


    initItem();
    itemcolor();


}

HistoryListItem::HistoryListItem(QWidget *parent):QWidget(parent)
{
    setFixedHeight(40);


    setAttribute(Qt::WA_TranslucentBackground, true);

//    setStyleSheet("background:red;");


    initItem();


}

void HistoryListItem::initItem()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    songNameWid = new QWidget(this);
    singerLabel = new QLabel(this);

    songTimeLabel = new QLabel(this);
    songNameLabel = new QLabel(this);

    int myheight = 35;


    QHBoxLayout *songWidLayout = new QHBoxLayout(songNameWid);

//    songNameLabel->setText("年少有为");
    songNameLabel->setStyleSheet("width: 182px;\
                                 height: 14px;\
                                 font-size: 14px;\
                                  \
                                 font-weight: 400;\
                                 color: #303133;\
                                 line-height: 14px;"
                                 );


//    songTimeLabel->setText("04:21");
    songTimeLabel->setFixedHeight(myheight);

    songTimeLabel->setStyleSheet("width: 35px;\
                                 height: 14px;\
                                 font-size: 14px;\
                                  \
                                 font-weight: 400;\
                                 color: #303133;\
                                 line-height: 14px;"
                                 );

//    singerLabel->setText("未知歌手");
    singerLabel->setFixedHeight(myheight);

    singerLabel->setStyleSheet("width: 158px;\
                               height: 14px;\
                               font-size: 14px;\
                                \
                               font-weight: 400;\
                               color: #303133;\
                               line-height: 14px;"
                               );

    songWidLayout->addWidget(songNameLabel,0,Qt::AlignLeft | Qt::AlignVCenter);
    songWidLayout->setSpacing(5);
    songWidLayout->setContentsMargins(0,0,20,0);


    songNameWid->setLayout(songWidLayout);
    songNameWid->setFixedHeight(myheight);

    mainLayout->addWidget(songNameWid,278);
    mainLayout->addWidget(singerLabel,174,Qt::AlignVCenter);
    mainLayout->addWidget(songTimeLabel,64,Qt::AlignVCenter);

}


void SongItem::initItem()
{
    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    songNameWid = new QWidget(this);
    singerLabel = new QLabel(this);
    albumLabel = new QLabel(this);
    songTimeLabel = new QLabel(this);
    songNameLabel = new QLabel(this);
    hoverLike = new QPushButton(this);
    hoverPlay = new QPushButton(this);

    int myheight = 35;


    QHBoxLayout *songWidLayout = new QHBoxLayout(songNameWid);


    hoverLike->setFixedSize(16,16);
    hoverLike->hide();


    hoverPlay->setFixedSize(16,16);
    hoverPlay->hide();

//    songNameLabel->setText("年少有为");



//    songTimeLabel->setText("04:21");
    songTimeLabel->setFixedHeight(myheight);



//    singerLabel->setText("未知歌手");
    singerLabel->setFixedHeight(myheight);


//    albumLabel->setText("未知专辑");
    albumLabel->setFixedHeight(myheight);




    songWidLayout->addWidget(songNameLabel,0,Qt::AlignLeft | Qt::AlignVCenter);
    songWidLayout->addWidget(hoverLike,0,Qt::AlignRight);
    songWidLayout->addWidget(hoverPlay,Qt::AlignRight);
    songWidLayout->setSpacing(5);
    songWidLayout->setContentsMargins(0,0,20,0);


    songNameWid->setLayout(songWidLayout);
    songNameWid->setFixedHeight(myheight);

    mainLayout->addWidget(songNameWid,278);
    mainLayout->addWidget(singerLabel,174,Qt::AlignVCenter);
    mainLayout->addWidget(albumLabel,174,Qt::AlignVCenter);
    mainLayout->addWidget(songTimeLabel,64,Qt::AlignVCenter);



}

void SongItem::itemcolor()
{
    if(WidgetStyle::themeColor == 1)
    {

        songNameLabel->setStyleSheet("width: 182px;\
                                     height: 14px;\
                                     font-size: 14px;\
                                      \
                                     font-weight: 400;\
                                     color: #F9F9F9;\
                                     line-height: 14px;");

        songTimeLabel->setStyleSheet("width: 35px;\
                                     height: 14px;\
                                     font-size: 14px;\
                                      \
                                     font-weight: 400;\
                                     color: #F9F9F9;\
                                     line-height: 14px;");

        singerLabel->setStyleSheet("width: 158px;\
                                   height: 14px;\
                                   font-size: 14px;\
                                    \
                                   font-weight: 400;\
                                   color: #F9F9F9;\
                                   line-height: 14px;");


        albumLabel->setStyleSheet("width: 158px;\
                                  height: 14px;\
                                  font-size: 14px;\
                                   \
                                  font-weight: 400;\
                                  color: #F9F9F9;\
                                  line-height: 14px;");

        hoverPlay->setStyleSheet("QPushButton{border-image:url(:/img/default/play1.png);width:16px;height:16px;}"
                                 "QPushButton::hover{border-image:url(:/img/clicked/play1.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/play1.png);}");

        hoverLike->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);width:16px;height:16px;}"
                                 "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/love2.png);}");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        songNameLabel->setStyleSheet("width: 182px;\
                                     height: 14px;\
                                     font-size: 14px;\
                                      \
                                     font-weight: 400;\
                                     color: #303133;\
                                     line-height: 14px;");

        songTimeLabel->setStyleSheet("width: 35px;\
                                     height: 14px;\
                                     font-size: 14px;\
                                      \
                                     font-weight: 400;\
                                     color: #303133;\
                                     line-height: 14px;");

        singerLabel->setStyleSheet("width: 158px;\
                                   height: 14px;\
                                   font-size: 14px;\
                                    \
                                   font-weight: 400;\
                                   color: #303133;\
                                   line-height: 14px;");


        albumLabel->setStyleSheet("width: 158px;\
                                  height: 14px;\
                                  font-size: 14px;\
                                   \
                                  font-weight: 400;\
                                  color: #303133;\
                                  line-height: 14px;");

        hoverPlay->setStyleSheet("QPushButton{border-image:url(:/img/default/play1.png);width:16px;height:16px;}"
                                 "QPushButton::hover{border-image:url(:/img/clicked/play1.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/play1.png);}");

        hoverLike->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);width:16px;height:16px;}"
                                 "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                 "QPushButton::pressed{border-image:url(:/img/clicked/love2.png);}");
    }

}

//历史记录label
void HistoryListItem::song_singerText(QString songName,QString singer)
{
    QString show_songName = "";
    QString show_singer   = "";
    if(songName.length() > 9)
    {
        show_songName = songName.mid(0,8);
        show_songName.append("...");
        songNameLabel->setText(show_songName);
        songNameLabel->setToolTip(songName);
    }
    else
    {
        songNameLabel->setText(songName);
        songNameLabel->setToolTip(songName);
    }
    if(singer.length() > 4)
    {
        show_singer = singer.mid(0,3);
        show_singer.append("...");
        singerLabel->setText(show_singer);
        singerLabel->setToolTip(singer);
    }
    else
    {
        singerLabel->setText(singer);
        singerLabel->setToolTip(singer);
    }
}

//播放列表label
void SongItem::song_singer_albumText(QString songName,QString singer,QString album)
{
    QString show_songName = "";
    QString show_singer   = "";
    QString show_album    = "";
    if(songName.length() > 17)
    {
        show_songName = songName.mid(0,16);
        show_songName.append("...");
        songNameLabel->setText(show_songName);
        songNameLabel->setToolTip(songName);
    }
    else
    {
        songNameLabel->setText(songName);
        songNameLabel->setToolTip(songName);
    }
    if(singer.length() > 9)
    {
        show_singer = singer.mid(0,8);
        show_singer.append("...");
        singerLabel->setText(show_singer);
        singerLabel->setToolTip(singer);
    }
    else
    {
        singerLabel->setText(singer);
        singerLabel->setToolTip(singer);
    }
    if(album.length() > 10)
    {
        show_album = album.mid(0,9);
        show_album.append("...");
        albumLabel->setText(show_album);
        albumLabel->setToolTip(album);
    }
    else
    {
        albumLabel->setText(album);
        albumLabel->setToolTip(album);
    }
}
