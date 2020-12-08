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

#include "sidebar.h"
#include <QDebug>
#include <QMessageBox>

SideBar::SideBar(QWidget *parent) : QFrame(parent)
{
    setFixedWidth(210);

    this->setObjectName("SideBar");



    initTopWidget();

    sidecolor();

}

void SideBar::sidecolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        PlayListBtn->setIcon(QIcon(":/img/default/songlist_w .png"));

        setStyleSheet("#SideBar{width: 210px;\
                      background: #1F2022;\
                      border-top-left-radius: 12px;border-bottom-left-radius:12px;}");

        logoLabel->setStyleSheet("border-image:url(:/img/kylin-music.png);margin-left:0px;");

        logoNameLabel->setStyleSheet("width: 56px;\
                                     height: 20px;\
                                     font-size: 14px;\
                                     font-family: PingFangSC-Medium, PingFang SC;\
                                     font-weight: 500;\
                                     color: #F9F9F9;\
                                     line-height: 20px;");
        recommendLabel->setStyleSheet("font-size: 14px;margin-left:26px;\
                                       \
                                      font-weight: 400;\
                                      border:none;\
                                      color: #8F9399;\
                                      line-height: 14px;"
                                        );
        PlayListBtn->setStyleSheet("QToolButton{padding-left:15px;margin-left:14px;\
                                   font-size: 14px;\
                                    \
                                   background:#FF7171;\
                                   color:#FFFFFF;border-radius:16px;}"
                                    "QToolButton::hover{background:#FF4848;}"
                                    "QToolButton::pressed{background:#DD3C3C;}");

        MySongListLabel->setStyleSheet("font-size: 14px;margin-left:26px;\
                                \
                               color: #8F9399;\
                               line-height: 14px;");
        songListWidget->setStyleSheet("QListWidget{background-color:#1F2022;border:0px;}"
                                      "QListWidget::item{background-color:#1F2022;}"
                                      "QListWidget::item:hover{background-color:#1F2022;}"
                                      "QListWidget::item:pressed{background-color:#1F2022;}");
        songListWidget->setContentsMargins(20,20,20,20);
    }
    else if(WidgetStyle::themeColor == 0)
    {
        PlayListBtn->setIcon(QIcon(":/img/default/songlist_w .png"));

        setStyleSheet("#SideBar{width: 210px;\
                      background: #FAFAFA;\
                      border-top-left-radius: 12px;border-bottom-left-radius:12px;}");
        logoLabel->setStyleSheet("border-image:url(:/img/kylin-music.png);margin-left:5px;"
                                 "width:24px;height:24;");
        logoNameLabel->setStyleSheet("width: 56px;\
                                     height: 20px;\
                                     font-size: 14px;\
                                     font-family: PingFangSC-Medium, PingFang SC;\
                                     font-weight: 500;\
                                     color: #303133;\
                                     line-height: 20px;");

        recommendLabel->setStyleSheet("font-size: 14px;margin-left:26px;\
                                       \
                                      font-weight: 400;\
                                      border:none;\
                                      color: #8F9399;\
                                      line-height: 14px;"
                                        );
        PlayListBtn->setStyleSheet("QToolButton{padding-left:15px;margin-left:14px;\
                                   font-size: 14px;\
                                    \
                                   background:#FF7171;\
                                   color:#FFFFFF;border-radius:16px;}"
                                    "QToolButton::hover{background:#FF4848;}"
                                    "QToolButton::pressed{background:#DD3C3C;}");
        MySongListLabel->setStyleSheet("font-size: 14px;margin-left:26px;\
                                \
                               color: #8F9399;\
                               line-height: 14px;");

        songListWidget->setStyleSheet("QListWidget{background-color:#FAFAFA;border:0px;}"
                                      "QListWidget::item:selected{background-color:#FAFAFA;}"
                                      "QListWidget::item:hover{background-color:#FAFAFA;}"
                                      "QListWidget::item:pressed{background-color:#FAFAFA;}");

    }
}



void SideBar::initTopWidget()
{

    QVBoxLayout *sideBarLayout = new QVBoxLayout(this);

    QHBoxLayout *logoLayout = new QHBoxLayout(this);
    logoLabel = new QLabel(this);
    logoNameLabel = new QLabel(this);
    logoNameLabel->setFixedSize(59,20);
//    logoNameLabel->setText("麒麟音乐");
    logoNameLabel->setText(tr("Kirin music"));
    //    logoLabel->setGeometry(40,20,100,30);
    logoLabel->setFixedSize(28,24);
    logoLayout->addWidget(logoLabel,Qt::AlignLeft);
    logoLayout->addWidget(logoNameLabel,0,Qt::AlignLeft);


    recommendLabel = new QLabel(this);
//    recommendLabel->setText("音乐库");
    recommendLabel->setText(tr("Music library"));
    recommendLabel->setGeometry(38,73,45,20);

    PlayListBtn = new QToolButton(this);
//    PlayListBtn->setGeometry(16,99,180,32);
    PlayListBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PlayListBtn->setIconSize(QSize(16,16));
//    PlayListBtn->setText("  歌曲列表");
    PlayListBtn->setText(tr("  The song list"));
    PlayListBtn->setFixedSize(180,32);

    QHBoxLayout *addSongListLayout = new QHBoxLayout(this);
    MySongListLabel = new QLabel(this);

    addSongListBtn = new QPushButton(this);
    MySongListLabel->setGeometry(38,161,56,16);
//    MySongListLabel->setText("我的歌单");
    MySongListLabel->setText(tr("My song list"));


//    addSongListBtn->setGeometry(100,161,16,16);
    addSongListBtn->setFixedSize(16,16);
//    addSongListBtn->setIcon(QIcon(":/img/default/add.png"));
    addSongListBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/add.png);}\
                                  QPushButton:pressed{border-image:url(:/img/clicked/add.png);}");
    addSongListLayout->addWidget(MySongListLabel);
    addSongListLayout->addWidget(addSongListBtn);
    addSongListLayout->addSpacing(20);

//    addSongListBtn->hide();   //添加歌单按钮隐藏



    songListWidget = new QListWidget(this);
//    songListWidget->setFrameShape(QListWidget::NoFrame);
//    songListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    songListWidget->setFocusPolicy(Qt::NoFocus);

    QWidget *logoWid = new QWidget(this);
    logoWid->setFixedSize(100, 40);
    logoWid->setLayout(logoLayout);
//    logoWid->setGeometry(8,8,82,24);

    sideBarLayout->addWidget(logoWid);
    sideBarLayout->addSpacing(15);
    sideBarLayout->addWidget(recommendLabel);
    sideBarLayout->addSpacing(5);
    sideBarLayout->addWidget(PlayListBtn);
    sideBarLayout->addSpacing(15);
    sideBarLayout->addLayout(addSongListLayout);
    sideBarLayout->addWidget(songListWidget);

    sideBarLayout->setMargin(0);

    this->setLayout(sideBarLayout);



    newSonglistPup = new AllPupWindow(this);
//    newSonglistPup->titleLab->setText("新建歌单");
    newSonglistPup->titleLab->setText(tr("The new song list"));
    newSonglistPup->pupDialog->hide();

    connect(addSongListBtn, SIGNAL(clicked(bool)), this, SLOT(addSongList()));

    connect(newSonglistPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(createSongList()));
    connect(newSonglistPup->enterLineEdit,SIGNAL(returnPressed()),this,SLOT(createSongList()));



    renameSongListPup = new AllPupWindow(this);
//    renameSongListPup->titleLab->setText("重命名");
    renameSongListPup->titleLab->setText(tr("rename"));
    renameSongListPup->pupDialog->hide();


    rightChangeWid = new QStackedWidget(this);
    myMusicListWid = new MusicListWid(this);


    rightChangeWid->addWidget(myMusicListWid);

    myMusicListWid->get_localmusic_information();   //获取歌曲列表里的所有信息


    sidemodel = new QSqlTableModel;
    sidemodel->setTable("NewPlayList");
    sidemodel->select();

    for(int i = 0; i < sidemodel->rowCount(); i++)
    {

        qDebug() << "数据库" << "加载歌单按钮";

        newSongList[i] = new QListWidgetItem(songListWidget);
        newSongListBtn[i] = new QToolButton(this);
        newSongListBtn[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        newSongListBtn[i]->setIcon(QIcon(":/img/default/songlist.png"));
        newSongListBtn[i]->setIconSize(QSize(16,16));

        QString listName = sidemodel->data(sidemodel->index(i,0)).toString();

        allListName.append(listName);  //初始化时把歌单名放进字符串表中

        qDebug() << "listName" << listName;

        newSongListBtn[i]->setText(listName);

        newSongListBtn[i]->setFixedSize(180,32);

        //设置侧边栏歌单列表条目的大小间距
        newSongList[i]->setSizeHint(QSize(180,38));

        //    favoriteMusicBtn->setStyleSheet("color:#303133;background:#FAFAFA;");

        if(WidgetStyle::themeColor == 1)
        {
            newSongListBtn[i]->setIcon(QIcon(":/img/default/songlist_w .png"));

            newSongListBtn[i]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                             font-size: 14px;\
                                             \
                                            background-color:#1F2022;\
                                            color:#F9F9F9;border-radius:16px;}"
                                            "QToolButton::hover{background-color:#303032;border-radius:16px;}"
                                            "QToolButton::pressed{background-color:#303032;border-radius:16px;}");
        }

        else if(WidgetStyle::themeColor == 0)
        {
            newSongListBtn[i]->setIcon(QIcon(":/img/default/songlist.png"));

            newSongListBtn[i]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                             font-size: 14px;\
                                             \
                                            background-color:#FAFAFA;\
                                            color:#303133;border-radius:16px;}"
                                            "QToolButton::hover{background-color:#EEEEEE;border-radius:16px;}"
                                            "QToolButton::pressed{background-color:#EEEEEE;border-radius:16px;}");
        }

        qDebug()<<"数据库"<<"关联歌单界面";

        songListWidget->setItemWidget(newSongList[i],newSongListBtn[i]);
        musicListChangeWid[i] = new MusicListWid(this);
        musicListChangeWid[i]->songListLabel->setText(listName);
        musicListChangeWid[i]->top_addSongBtn->hide();
        rightChangeWid->addWidget(musicListChangeWid[i]);

        connect(newSongListBtn[i],SIGNAL(clicked(bool)),this,SLOT(AlterPage()));

        readmodel = new QSqlTableModel;
        readmodel->setTable("LocalMusic");

        qDebug() << "数据库" << "获取歌单歌曲";
        get_listmusic_information(i);

    }



    connect(PlayListBtn,SIGNAL(clicked(bool)),this,SLOT(ChangePage()));
//    connect(musicListChangeWid[btnIndex]->Music,&QMediaPlayer::stateChanged,this,&SideBar::Music_playlist_stateChang);
    for(int i = 0;i < sidemodel->rowCount();i++)
    {
        musicListChangeWid[i]->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(musicListChangeWid[i],SIGNAL(customContextMenuRequested(const QPoint&)),
            this,SLOT(on_musicListChangeWid_customContextMenuRequested(const QPoint&)));
//        connect(musicListChangeWid[i]->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
//            this,SLOT(on_musicListChangeWid_doubleClicked(QListWidgetItem*)));
//        connect(musicListChangeWid[i]->PlayList,&QMediaPlaylist::currentIndexChanged,
//            this,&SideBar::updataplaylistwidget);
    }


}

void SideBar::on_musicListChangeWid_customContextMenuRequested(const QPoint &pos)
{
    if(musicListChangeWid[currentSelectList]->musicInfoWidget->count() <= 0)
    {
        QListWidgetItem *curItem1 = musicListChangeWid[currentPlayList]->musicInfoWidget->itemAt(pos);

        if(curItem1 == NULL)
        {
            return;
        }
    }
    menu = new QMenu(musicListChangeWid[currentSelectList]->musicInfoWidget);
    listPlayAct = new QAction(this);
    listNextAct = new QAction(this);
    listDeleAct = new QAction(this);
    listSongAct = new QAction(this);

//    listPlayAct->setText("播放");
    listPlayAct->setText(tr("play"));
//    listNextAct->setText("下一首");
    listNextAct->setText(tr("The following piece"));
//    listDeleAct->setText("从歌单中删除");
    listDeleAct->setText(tr("Delete the next song from the playlist"));
//    listSongAct->setText("歌曲信息");
    listSongAct->setText(tr("Song information"));
//    menu->addAction(listPlayAct);
//    menu->addAction(listNextAct);
    menu->addAction(listDeleAct);
    menu->addAction(listSongAct);
//    connect(listPlayAct,&QAction::triggered,this,&SideBar::listPlayAct_slot);   //jialin
//    connect(listNextAct,&QAction::triggered,this,&SideBar::listNextAct_slot);
    connect(listDeleAct,&QAction::triggered,this,&SideBar::deleteMusicFromSongList);
    connect(listSongAct,&QAction::triggered,this,&SideBar::listSongAct_slot);
    menu->exec(QCursor::pos());
    delete menu;
    delete listPlayAct;
    delete listNextAct;
    delete listDeleAct;
    delete listSongAct;

}

void SideBar::listPlayAct_slot()
{
    //    for(int i = 0;i < sidemodel->rowCount();i++)
    //    {

    qDebug()<<"listPlayAct_slot    currentSelectList"<<currentSelectList;
//    if (myMusicListWid->Music->state() == QMediaPlayer::PlayingState)
//    {
//        myMusicListWid->Music->stop();
//    }
    qDebug()<<"listPlayAct_slot   currentSelectList"<<currentSelectList;

    if(currentPlayList != -1 && currentPlayList != -2)
    {
        if (musicListChangeWid[currentSelectList]->Music->playlist() !=
            musicListChangeWid[currentSelectList]->PlayList)
        {
            qDebug() << "listPlayAct_slot  musicListChangeWid[currentSelectList]->PlayList";
            musicListChangeWid[currentSelectList]->Music->setPlaylist(musicListChangeWid[currentSelectList]->PlayList);
        }
    }

//    if(musicListChangeWid[currentSelectList]->Music->playlist() == musicListChangeWid[currentSelectList]->PlayList)
//    {
    qDebug() << "listPlayAct_slot  ";

    int row = musicListChangeWid[currentSelectList]->musicInfoWidget->currentIndex().row();

    // 当前选中歌单和当前播放歌单是同一歌单
    if (currentSelectList == currentPlayList)
    {
        // 当前选择的歌曲是当前播放的歌曲
        if (row == musicListChangeWid[currentSelectList]->PlayList->currentIndex())
        {
            // 歌曲正在播放
            if(musicListChangeWid[currentSelectList]->Music->state() == QMediaPlayer::PlayingState)
            {
                qDebug()<<"暂停";
                //                play_pause="播放";
                listPlayAct->setText(tr("pause"));
                musicListChangeWid[currentSelectList]->Music->pause();
            }
            // 歌曲没有播放
            else
            {
                qDebug()<<"播放";
                //                play_pause="暂停";
                listPlayAct->setText(tr("play"));
                musicListChangeWid[currentSelectList]->Music->play();
            }

        }
        // 当前选择的歌曲不是当前播放的歌曲
        else
        {
            listPlayAct->setText(tr("pause"));
            musicListChangeWid[currentSelectList]->Music->stop();
            musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(row);
            musicListChangeWid[currentSelectList]->Music->play();
        }
    }
    // 当前选中歌单和当前播放歌单不是同一歌单
    else
    {
        qDebug()<<"播放";
        //                play_pause="暂停";
        listPlayAct->setText("play");
        // 将其他歌单停止播放
        if (currentPlayList != -1 && currentPlayList != -2)
        {
            musicListChangeWid[currentPlayList]->Music->stop();
        }
        else
        {
            myMusicListWid->Music->stop();
        }

        currentPlayList = currentSelectList;
        musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(row);
        musicListChangeWid[currentSelectList]->Music->play();


    }


//        if(musicListChangeWid[currentSelectList]->musicInfoWidget->item(musicListChangeWid[currentSelectList]->PlayList->currentIndex())->isSelected())
//        {
//            if(musicListChangeWid[currentSelectList]->Music->state() == QMediaPlayer::PlayingState)
//            {
//                qDebug()<<"暂停";
//                //                play_pause="播放";
//                listPlayAct->setText("暂停");
//                musicListChangeWid[currentSelectList]->Music->pause();
//            }
//            else
//            {
//                qDebug()<<"播放";
//                //                play_pause="暂停";
//                listPlayAct->setText("播放");
//                if (currentPlayList != -1 && currentPlayList != -2) {
//                    musicListChangeWid[currentPlayList]->Music->stop();
//                }

//                currentPlayList = currentSelectList;

//                if (musicListChangeWid[currentSelectList]->Music->playlist() !=
//                        musicListChangeWid[currentSelectList]->PlayList) {
//                    musicListChangeWid[currentSelectList]->Music->setPlaylist(
//                                musicListChangeWid[currentSelectList]->PlayList);
//                }
//                musicListChangeWid[currentSelectList]->Music->play();
//            }
//        }
//        else
//        {
//            musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(musicListChangeWid[currentSelectList]->musicInfoWidget->currentIndex().row());
//            qDebug()<<"666666666666";
//            //            play_pause="播放";
//            listPlayAct->setText("播放");
//            if (currentPlayList != -1 && currentPlayList != -2) {
//                musicListChangeWid[currentPlayList]->Music->stop();
//            }

//            currentPlayList = currentSelectList;

//            if (musicListChangeWid[currentSelectList]->Music->playlist() !=
//                    musicListChangeWid[currentSelectList]->PlayList) {
//                musicListChangeWid[currentSelectList]->Music->setPlaylist(
//                            musicListChangeWid[currentSelectList]->PlayList);
//            }
//            musicListChangeWid[currentSelectList]->Music->play();

//        }
//    }
//    else
//    {
//        musicListChangeWid[currentSelectList]->Music->setPlaylist(musicListChangeWid[currentSelectList]->PlayList);
//        musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(musicListChangeWid[currentSelectList]->musicInfoWidget->currentIndex().row());
//        qDebug()<<"777777777777";
//        //        play_pause="播放";
//        listPlayAct->setText("播放");
//        if (currentPlayList != -1 && currentPlayList != -2) {
//            musicListChangeWid[currentPlayList]->Music->stop();
//        }

//        currentPlayList = currentSelectList;

//        if (musicListChangeWid[currentSelectList]->Music->playlist() !=
//                musicListChangeWid[currentSelectList]->PlayList) {
//            musicListChangeWid[currentSelectList]->Music->setPlaylist(
//                        musicListChangeWid[currentSelectList]->PlayList);
//        }
//        musicListChangeWid[currentSelectList]->Music->play();
//    }

}


void SideBar::listNextAct_slot()
{
//    for (int i = 0;i < sidemodel->rowCount();i++)
//    {
        myMusicListWid->Music->stop();

        if(musicListChangeWid[currentSelectList]->Music->playlist() == musicListChangeWid[currentSelectList]->PlayList)
        {
            int currentIndex = musicListChangeWid[currentSelectList]->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(++currentIndex == musicListChangeWid[currentSelectList]->PlayList->mediaCount())
            {
                currentIndex=0;
            }
            musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(currentIndex);
        }
        musicListChangeWid[currentSelectList]->Music->play();
//    }
}

void SideBar::deleteMusicFromSongList()
{
    // 删除歌曲时跳过高亮变换
    skipPlaylistHighlight = true;
    int tempmusicRow = musicListChangeWid[currentSelectList]->PlayList->currentIndex();                 //正在播放
    int tempmusicrow = musicListChangeWid[currentSelectList]->musicInfoWidget->currentIndex().row();    //选中

    int templistrow = currentSelectList;
    QString tempAllId = sidemodel->data(sidemodel->index(templistrow,1)).toString();
    int tempid = tempAllId.split(",")[tempmusicrow].toInt();

    QString idstr = sidemodel->data(sidemodel->index(templistrow, 1)).toString();
    idstr = idstr.replace(QString::number(tempid) + ",", "");

    sidemodel->setData(sidemodel->index(templistrow,1),idstr);


    musicListChangeWid[currentSelectList]->localAllMusicid.removeAt(tempmusicrow);
    musicListChangeWid[currentSelectList]->musicInfoWidget->removeItemWidget(musicListChangeWid[currentSelectList]->musicInfoWidget->item(tempmusicrow));
    delete musicListChangeWid[currentSelectList]->musicInfoWidget->item(tempmusicrow);


    if (tempmusicRow > tempmusicrow)
    {

//        musicListChangeWid[templistrow]->PlayList->setCurrentIndex(0);
        musicListChangeWid[currentSelectList]->PlayList->removeMedia(tempmusicrow, tempmusicrow);
        musicListChangeWid[currentSelectList]->Music->setPlaylist(musicListChangeWid[currentSelectList]->PlayList);
        musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(tempmusicRow - 1);

//        musicListChangeWid[templistrow]->Music->setPosition(position);
        musicListChangeWid[currentSelectList]->Music->play();
    }
    else if (tempmusicRow == tempmusicrow)
    {
        qDebug()<<"currentSelectList currentSelectList : "<<currentSelectList;
        if(musicListChangeWid[currentSelectList]->musicInfoWidget->count() == 1)
        {
            qDebug()<<"currentSelectList currentSelectList : ";
            musicListChangeWid[currentSelectList]->Music->stop();
//            playSongArea->songText("");
//            playSongArea->bottomLeftLabel->setText("");
        }
        else if(musicListChangeWid[currentSelectList]->musicInfoWidget->count() > 1)
        {
            if (tempmusicRow == musicListChangeWid[currentSelectList]->musicInfoWidget->count() - 1)
            {
                qDebug()<<"currentSelectList currentSelectList : "<<currentSelectList;
                musicListChangeWid[currentSelectList]->Music->stop();
                musicListChangeWid[currentSelectList]->PlayList->removeMedia(tempmusicRow);
                musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(0);
                musicListChangeWid[currentSelectList]->Music->setPlaylist(musicListChangeWid[currentSelectList]->PlayList);
            }
            else
            {
                qDebug()<<"currentSelectList : "<<currentSelectList;
                musicListChangeWid[currentSelectList]->Music->stop();
                qDebug()<<"currentSelectList : "<<currentSelectList;
                musicListChangeWid[currentSelectList]->PlayList->removeMedia(tempmusicRow);
                qDebug()<<"currentSelectList : "<<currentSelectList;
                musicListChangeWid[currentSelectList]->PlayList->setCurrentIndex(tempmusicRow);
                qDebug()<<"currentSelectList : "<<currentSelectList;
                musicListChangeWid[currentSelectList]->Music->setPlaylist(musicListChangeWid[templistrow]->PlayList);
            }
        }
    }
    else if (tempmusicRow < tempmusicrow)
    {
        musicListChangeWid[currentSelectList]->PlayList->removeMedia(tempmusicrow, tempmusicrow);
    }

    sidemodel->submitAll();
    sidemodel->select();

//        musicListChangeWid[currentSelectList]->songNumberLabel->setText("共"+QString::number(musicListChangeWid[currentSelectList]->musicInfoWidget->count())+"首");
    musicListChangeWid[currentSelectList]->songNumberLabel->setText(
                tr("A total of")+QString::number(musicListChangeWid[currentSelectList]->musicInfoWidget->count())+tr("The first"));
    // 删除歌曲结束时开启高亮变换
    skipPlaylistHighlight = false;
}

void SideBar::listSongAct_slot()
{

    int thisrow = musicListChangeWid[currentSelectList]->musicInfoWidget->currentIndex().row();
    sidemodel->setTable("NewPlayList");
    sidemodel->select();

    QString allmusicid = sidemodel->data(sidemodel->index(currentSelectList,1)).toString();

    int thisid = allmusicid.split(",")[thisrow].toInt();

    QStringList thismusicinfo = myMusicListWid->get_info_from_db(thisid);

    qDebug() << "listSongAct" << "thisrow = " << thisrow << "thisid = " << thisid ;

    QString titleStr  = thismusicinfo[0];
    QString pathStr   = thismusicinfo[1];
    QString artistStr = thismusicinfo[2];
    QString albumStr  = thismusicinfo[3];
    QString type      = thismusicinfo[4];
    QString mp3Size   = thismusicinfo[5];
    QString timeStr   = thismusicinfo[6];

//    QMessageBox::about(this,tr("歌曲信息"),tr("歌曲名称: %1 \n"
//                                          "歌手: %2 \n"
//                                          "专辑: %3 \n"
//                                          "文件类型: %4 \n"
//                                          "文件大小: %5 \n"
//                                          "文件时长: %6 \n"
//                                          "文件位置: %7 \n").arg(titleStr)
//                       .arg(artistStr)
//                       .arg(albumStr)
//                       .arg(type)
//                       .arg(mp3Size)
//                       .arg(timeStr)
//                       .arg(pathStr));
    QMessageBox::about(this,tr("Song information"),QString(tr("The song name") + ": %1 \n" +
                                                   tr("singer")        + ": %2 \n" +
                                                   tr("The album")     + ": %3 \n" +
                                                   tr("The file type") + ": %4 \n" +
                                                   tr("The file size") + ": %5 \n" +
                                                   tr("File length")   + ": %6 \n" +
                                                   tr("File location") + ": %7 \n")
                                                                    .arg(titleStr)
                                                                    .arg(artistStr)
                                                                    .arg(albumStr)
                                                                    .arg(type)
                                                                    .arg(mp3Size)
                                                                    .arg(timeStr)
                                                                    .arg(pathStr));

}


void SideBar::addSongList()
{
    //每次打开新建歌单按钮时清除输入文字框中上次输入的文字
    newSonglistPup->enterLineEdit->clear();
    newSonglistPup->pupDialog->show();
    newSonglistPup->enterLineEdit->setFocus();


}


void SideBar::createSongList()
{
    int num = songListWidget->count();   //num为songListWidget页面中当前item索引值

    QString listName = newSonglistPup->enterLineEdit->text();
    newSonglistPup->enterLineEdit->clear();
    qDebug() << listName;

    if( num >= 19 )
    {
        newSonglistPup->pupDialog->hide();

//        QMessageBox::about(this,"提示信息","歌单数量已达上限！！！");
        QMessageBox::about(this,tr("Prompt information"),tr("The number of playlists has reached the limit!!"));
        return;

    }

    sidemodel->setTable("NewPlayList");
    sidemodel->select();

    if(allListName.indexOf(listName) != -1)
    {
        newSonglistPup->pupDialog->hide();

//        QMessageBox::about(this,tr("提示信息"),tr("歌单名已存在！！！"));
        QMessageBox::about(this,tr("Prompt information"),tr("Single song name already exists!!"));

    }
    else
    {
        allListName.append(listName);
        newSongList[num] = new QListWidgetItem(songListWidget);  //我喜欢 这一item占有一个索引，所以num作为控制新建歌单数组的下标变量时应该减去1
        newSongListBtn[num] = new QToolButton(this);
        newSongListBtn[num]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        newSongListBtn[num]->setIconSize(QSize(16,16));
//        newSongListBtn[num]->setText("新建歌单" + QString::number(num+1));
        newSongListBtn[num]->setText(tr("The new song list") + QString::number(num+1));
        newSongListBtn[num]->setFixedSize(180,32);
        newSongList[num]->setSizeHint(QSize(180,32));

        if (listName != "") {
            newSongListBtn[num]->setText(listName);
        }
        else {
            listName = "新建歌单" + QString::number(num+1);
        }

        //    favoriteMusicBtn->setStyleSheet("color:#303133;background:#FAFAFA;");

        if(WidgetStyle::themeColor == 1)
        {
            newSongListBtn[num]->setIcon(QIcon(":/img/default/songlist_w .png"));

            newSongListBtn[num]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                               font-size: 14px;\
                                               \
                                              background-color:#1F2022;\
                                              color:#F9F9F9;border-radius:16px;}"
                                              "QToolButton::hover{background-color:#303032;border-radius:16px;}"
                                              "QToolButton::pressed{background-color:#303032;border-radius:16px;}");
        }
        else if(WidgetStyle::themeColor == 0)
        {
            newSongListBtn[num]->setIcon(QIcon(":/img/default/songlist.png"));

            newSongListBtn[num]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                             font-size: 14px;\
                                             \
                                            background-color:#FAFAFA;\
                                            color:#303133;border-radius:16px;}"
                                            "QToolButton::hover{background-color:#EEEEEE;border-radius:16px;}"
                                            "QToolButton::pressed{background-color:#EEEEEE;border-radius:16px;}");
        }
        songListWidget->setItemWidget(newSongList[num],newSongListBtn[num]);

        musicListChangeWid[num] = new MusicListWid(this);    //歌曲列表界面占据一个 所以此处应该是num
        musicListChangeWid[num]->top_addSongBtn->hide();
        musicListChangeWid[num]->songListLabel->setText(listName);
        rightChangeWid->addWidget(musicListChangeWid[num]);

        qDebug() << songListWidget->count() << "121111" << rightChangeWid->count();
        connect(newSongListBtn[num],SIGNAL(clicked(bool)),this,SLOT(AlterPage()));
        musicListChangeWid[num]->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(musicListChangeWid[num],SIGNAL(customContextMenuRequested(const QPoint&)),
            this,SLOT(on_musicListChangeWid_customContextMenuRequested(const QPoint&)));

        // 添加到数据库
        QSqlQuery query;
        QString insertList = QString("insert into NewPlayList (listname, musicid) values ('%1', '%2')").arg(listName).arg("");
        query.exec(insertList);

        newSonglistPup->pupDialog->hide();
    }

}

void SideBar::ChangePage()
{
    qDebug() << "void SideBar::AlterPage()void SideBar::AlterPage()void SideBar::AlterPage()";
    rightChangeWid->setCurrentIndex(0);
    currentSelectList = -1;

//    if (currentPlayList != 0)
//    {
//        // 歌曲列表失去焦点
//        for (int i = 0; i < myMusicListWid->musicInfoWidget->count(); i++)
//        {
//            myMusicListWid->musicInfoWidget->item(i)->setSelected(false);
//        }
//    }

}

void SideBar::AlterPage()
{

    qDebug() << "void SideBar::AlterPage()void SideBar::AlterPage()void SideBar::AlterPage()";
//    rightChangeWid->setCurrentIndex(index+1);

    QToolButton *pButton = qobject_cast<QToolButton *>(sender());

    btnIndex = 0;

    for (; btnIndex < 20; btnIndex++) {

        if (newSongListBtn[btnIndex] == pButton) {
            break;
        }
    }

    if (btnIndex < 20) {
        rightChangeWid->setCurrentIndex(btnIndex + 1);   //我喜欢和歌曲列表
        currentSelectList = btnIndex;
    }
    else {
        rightChangeWid->setCurrentIndex(1);
        currentSelectList = 1;
        if (currentPlayList != 1)
        {
            // 我喜欢列表失去焦点
            for (int i = 0; i < musicListChangeWid[currentSelectList]->musicInfoWidget->count(); i++) {
                musicListChangeWid[currentSelectList]->musicInfoWidget->item(i)->setSelected(false);
            }
        }
    }

    qDebug() << "rightChangeWid   " << pButton->text() << "  " << btnIndex+1;
    qDebug() << "pButton : " << pButton;
    qDebug() << "btnIndex : " << btnIndex;
}


void SideBar::get_listmusic_information(int listindex)
{
    qDebug() << "数据库" <<"歌单歌曲信息加载";
    int i = listindex;
    readmodel->select();
    sidemodel->select();

    musicListChangeWid[i]->musicInfoWidget->clear();

    QString allmusicid = sidemodel->data(sidemodel->index(i,1)).toString();
    allmusiclist = allmusicid.split(",");
    qDebug() << i << "歌单" << allmusicid;
    qDebug() << i << "歌单" << allmusiclist;

    for (int j = 0; j < allmusiclist.size()-1; j++)
    {
        int num1 = allmusiclist.at(j).toInt();
        if(num1 >= 0)
        {
            QString songName = readmodel->data(readmodel->index(num1,1)).toString();
            QString Path = readmodel->data(readmodel->index(num1,2)).toString();
            QString Title = readmodel->data(readmodel->index(num1,3)).toString();
            QString Album = readmodel->data(readmodel->index(num1,4)).toString();
            QString Time = readmodel->data(readmodel->index(num1,7)).toString();


            //            mySideBar->musicListChangeWid[listindex]->allmusic.append(Path);

    //        qDebug() << "Action.date():" << Path ;
            qDebug() << "allmusiclist.size()"<<allmusiclist.size() << "歌单";

            qDebug() <<"j= " << j << "歌单" << allmusiclist.at(j) << "转换成整型" << num1;

            if(songName != ""&&Path != "")
            {
                QListWidgetItem *item1=new QListWidgetItem(musicListChangeWid[i]->musicInfoWidget);
                SongItem *songitem1 = new SongItem;
                musicListChangeWid[i]->musicInfoWidget->setItemWidget(item1,songitem1);
                songitem1->songNameLabel->setText(songName); //歌曲名称
                songitem1->albumLabel->setText(Album);  //专辑
                songitem1->songTimeLabel->setText(Time); //时长
                songitem1->singerLabel->setText(Title); //歌手

                qDebug() << "歌单" << songName << allmusiclist.at(j) << "转换成整型" << num1;
            }


            musicListChangeWid[i]->PlayList->addMedia(QUrl::fromLocalFile(Path));
        }
    }

//    musicListChangeWid[i]->Music->setPlaylist(musicListChangeWid[i]->PlayList);
    musicListChangeWid[i]->songNumberLabel->setText(tr("A total of")+QString::number(musicListChangeWid[i]->musicInfoWidget->count())+tr("The first"));

    qDebug() << "数据库" <<"歌单歌曲信息加载完毕";

}

