#include "songitem.h"

SongItem::SongItem(QWidget *parent):QWidget(parent)
{
    setFixedHeight(40);


    setAttribute(Qt::WA_TranslucentBackground, true);

//    setStyleSheet("background:red;");


    initItem();


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

    hoverLike->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);width:16px;height:16px;}"
                             "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                             "QPushButton::pressed{border-image:url(:/img/clicked/love2.png);}");
    hoverLike->setFixedSize(16,16);
    hoverLike->hide();

    hoverPlay->setStyleSheet("QPushButton{border-image:url(:/img/default/play1.png);width:16px;height:16px;}"
                             "QPushButton::hover{border-image:url(:/img/clicked/play1.png);}"
                             "QPushButton::pressed{border-image:url(:/img/clicked/play1.png);}");
    hoverPlay->setFixedSize(16,16);
    hoverPlay->hide();

//    songNameLabel->setText("年少有为");
    songNameLabel->setStyleSheet("width: 182px;\
                                 height: 14px;\
                                 font-size: 14px;\
                                 font-family: PingFangSC-Regular, PingFang SC;\
                                 font-weight: 400;\
                                 color: #303133;\
                                 line-height: 14px;");


//    songTimeLabel->setText("04:21");
    songTimeLabel->setFixedHeight(myheight);

    songTimeLabel->setStyleSheet("width: 35px;\
                                 height: 14px;\
                                 font-size: 14px;\
                                 font-family: PingFangSC-Regular, PingFang SC;\
                                 font-weight: 400;\
                                 color: #303133;\
                                 line-height: 14px;");

//    singerLabel->setText("未知歌手");
    singerLabel->setFixedHeight(myheight);

    singerLabel->setStyleSheet("width: 158px;\
                               height: 14px;\
                               font-size: 14px;\
                               font-family: PingFangSC-Regular, PingFang SC;\
                               font-weight: 400;\
                               color: #303133;\
                               line-height: 14px;");

//    albumLabel->setText("未知专辑");
    albumLabel->setFixedHeight(myheight);

    albumLabel->setStyleSheet("width: 158px;\
                              height: 14px;\
                              font-size: 14px;\
                              font-family: PingFangSC-Regular, PingFang SC;\
                              font-weight: 400;\
                              color: #303133;\
                              line-height: 14px;");



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
