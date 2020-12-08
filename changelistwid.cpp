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

#include "changelistwid.h"
#include "songitem.h"
#include "widgetstyle.h"

ChangeListWid::ChangeListWid(QWidget *parent):QWidget(parent)
{
//    this->setObjectName("ChangeListWid");


    initStack();

    this->resize(750,80);
    nullWidgetColor();


}


void ChangeListWid::initStack()
{
//    titleFrame = new QFrame(this);

    songListLabel = new QLabel(this);
    songListLabel->setGeometry(30,18,100,30);
//    songListLabel->setText("歌曲列表");
    songListLabel->setText(tr("The song list"));


    songNumberLabel = new QLabel(this);
    songNumberLabel->setGeometry(142,28,40,18);
//    songNumberLabel->setText("共0首");
    songNumberLabel->setText(tr("A total of 0 first"));

    nullMusicIconLabel = new QLabel(this);
    nullMusicIconLabel->setGeometry(275,122,200,180);
    nullMusicIconLabel->setStyleSheet("border-image:url(:/img/default/pict1.png)");

    nullMusicLabel = new QLabel(this);
    nullMusicLabel->setGeometry(333,313,84,24);
//    nullMusicLabel->setText("还没有歌曲！");
    nullMusicLabel->setText(tr("There are no songs yet!"));


    n_addLocalSong = new QPushButton(this);
    n_addLocalFolder = new QPushButton(this);

//    musicListWidChange = new MusicListWid;

    n_addLocalSong->setGeometry(240,357,126,30);
//    n_addLocalSong->setText("添加本地歌曲");
    n_addLocalSong->setText(tr("Add local songs"));


    n_addLocalFolder->setGeometry(385,357,126,30);
//    n_addLocalFolder->setText("添加本地文件夹");
    n_addLocalFolder->setText(tr("Add local folders"));
}

void ChangeListWid::nullWidgetColor()
{
    if(WidgetStyle::themeColor == 1)
    {
        songListLabel->setStyleSheet("width:96px;height:24px;"
                                     "font-size: 24px;\
                                      \
                                     font-weight: 600;\
                                     color:rgba(249,249,249,1);\
                                     line-height: 24px;");

        songNumberLabel->setStyleSheet("font-size: 14px;\
                                         \
                                        font-weight: 400;\
                                        border:none;\
                                        color: #8F9399;\
                                        line-height: 14px;");

        nullMusicLabel->setStyleSheet("font-size: 14px;\
                                       \
                                      font-weight: 400;\
                                      border:none;\
                                      color: rgba(143, 147, 153, 1);\
                                      line-height:24px;");

        n_addLocalSong->setStyleSheet("font-size:14px;color:rgba(249, 249, 249, 1);border-radius:15px;border:2px solid #DDDFE7;"
                                      " \
                                      font-weight: 400;width:84px;height:14px;");

        n_addLocalFolder->setStyleSheet("font-size:14px;color:rgba(249, 249, 249, 1);border-radius:15px;border:2px solid #DDDFE7;"
                                        " \
                                        font-weight: 400;width:84px;height:14px;");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        songListLabel->setStyleSheet("width:96px;height:24px;"
                                     "font-size: 24px;\
                                      \
                                     font-weight: 600;\
                                     border:none;\
                                     color: #303133;\
                                     line-height: 24px;");

        songNumberLabel->setStyleSheet("font-size: 14px;\
                                        \
                                       font-weight: 400;\
                                       border:none;\
                                       color: #8F9399;\
                                       line-height: 14px;");

        nullMusicLabel->setStyleSheet("font-size: 14px;\
                                       \
                                      font-weight: 400;\
                                      border:none;\
                                      color: #000000;\
                                      line-height:24px;");

        n_addLocalSong->setStyleSheet("font-size:14px;color:#303133;border-radius:15px;border:2px solid #DDDFE7;"
                                      " \
                                      font-weight: 400;width:84px;height:14px;");

        n_addLocalFolder->setStyleSheet("font-size:14px;color:#303133;border-radius:15px;border:2px solid #DDDFE7;"
                                        " \
                                        font-weight: 400;width:84px;height:14px;");
    }

}

MusicListWid::MusicListWid(QWidget *parent):QWidget(parent)
{
    this->setObjectName("ChangeListWid");


    initMusicListWid();

//    this->resize(750,80);
    localModel = new QSqlTableModel(this);
    musiclistcolor();

}

//歌曲不为零时的切换页面

void MusicListWid::initMusicListWid()
{//    titleFrame = new QFrame(this);



    hTitleLayout = new QHBoxLayout(this);


    vMainLayout = new QVBoxLayout(this);
    vListWidLayout = new QVBoxLayout(this);

    songListLabel = new QLabel(this);
//    songListLabel->setGeometry(30,18,100,30);
//    songListLabel->setText("歌曲列表");
    songListLabel->setText(tr("The song list"));


    songNumberLabel = new QLabel(this);
//    songNumberLabel->setGeometry(142,28,40,18);
//    songNumberLabel->setText("共"+QString::number(count)+"首");
    songNumberLabel->setText(tr("A total of")+QString::number(count)+tr("The first"));



    top_addSongBtn = new QToolButton(this);
//    top_playAllSongBtn = new QToolButton(this);

//    top_playAllSongBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    top_playAllSongBtn->setIcon(QIcon(":/img/default/play_w.png"));
//    top_playAllSongBtn->setIconSize(QSize(16,16));
//    top_playAllSongBtn->setText("播放全部");
//    top_playAllSongBtn->setFixedSize(100,30);
//    top_playAllSongBtn->setStyleSheet("font-size: 14px;line-height: 14px;font-weight: 400;\
//                                        \
//                                       background:#FF4848;padding-left:10px;\
//                                       color:#FFFFFF;border-radius:15px;");



    top_addSongBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    top_addSongBtn->setIcon(QIcon(":/img/default/add.png"));
    top_addSongBtn->setIconSize(QSize(16,16));
//    top_addSongBtn->setText("添加歌曲");
    top_addSongBtn->setText(tr("Add the songs"));
    top_addSongBtn->setFixedSize(100,30);


//    connect(top_addSongBtn,SIGNAL(clicked()),this,SLOT(on_top_addSongBtn_slot()));
    connect(top_addSongBtn,&QToolButton::clicked,this,&MusicListWid::on_top_addSongBtn_slot);

    songLabel = new QLabel(this);
    singerLabel = new QLabel(this);
    albumLabel = new QLabel(this);
    timeLabel = new QLabel(this);


    hTitleLayout->addWidget(songListLabel,0,Qt::AlignLeft);
    hTitleLayout->addWidget(songNumberLabel,Qt::AlignLeft);
//    hTitleLayout->addWidget(top_playAllSongBtn);
    hTitleLayout->addWidget(top_addSongBtn);


//    hTitleListLayout = new QHBoxLayout(this);

    QHBoxLayout *testLayout = new QHBoxLayout(this);

    hTitleLayout->addWidget(songLabel,278);
    hTitleLayout->addWidget(singerLabel,174);
    hTitleLayout->addWidget(albumLabel,174);
    hTitleLayout->addWidget(timeLabel,64);

    testLayout->addWidget(songListLabel,0,Qt::AlignLeft);
    testLayout->addWidget(songNumberLabel,Qt::AlignLeft);
//    testLayout->addWidget(top_playAllSongBtn);
    testLayout->addWidget(top_addSongBtn);

    QWidget *testWid = new QWidget(this);
    testWid->setLayout(testLayout);
    testWid->setFixedHeight(80);

//    testWid->setStyleSheet("background:red;");

//    testWid->resize(750, 40);

    QWidget *testWid2 = new QWidget(this);
    testWid2->setLayout(hTitleLayout);
    testWid2->setFixedHeight(40);
//    testWid2->setStyleSheet("background:blue;");

    vMainLayout->addWidget(testWid,Qt::AlignTop);
    vMainLayout->addWidget(testWid2,0,Qt::AlignTop);





    testWid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);



    musicInfoWidget = new QListWidget(this);




    vMainLayout->addWidget(musicInfoWidget);

    vMainLayout->setMargin(0);
    vMainLayout->setSpacing(0);

    PlayList = new QMediaPlaylist(this);

    Music = new QMediaPlayer(this);

    this->setLayout(vMainLayout);
}


void MusicListWid::on_top_addSongBtn_slot()
{
    QSqlQuery query;

    songFiles = QFileDialog::getOpenFileNames(this, tr("Open the file"),"","音乐文件(*.mp3 *.ogg *.wav *.wma *.spx *.ape *.flac)");  //歌曲文件
    if(!songFiles.isEmpty())
    {

        for(int i = 0; i < songFiles.size(); i++)
        {
            Dir = QDir::toNativeSeparators(songFiles.at(i));
            mp3Name = songFiles.at(i);
            if(!mp3Name.isEmpty())
            {
                bool songExists = false;
                for(QString &songPlaylist:allmusic)
                {
                    if(mp3Name == songPlaylist)    //通过路径判断歌曲是否存在
                    {
                        qDebug()<<"歌曲已存在 (通过路径判断歌曲是否存在)";
//                        qDebug()<<"songPlaylist : "<<songPlaylist;
                        songExists = true;
                        break;
                    }
                }
                if(songExists)
                {
                    continue;
                }

                fileInfo.setFile(mp3Name);

                type = fileInfo.suffix();        //文件类型

                qint64 fileSize = fileInfo.size();
                if(fileSize/1024)
                {
                    if(fileSize/1024/1024)
                    {
                        if(fileSize/1024/1024/1024)
                        {
                            mp3Size = QString::number(fileSize/1024/1024/1024,10)+"GB";
                        }
                        else
                            mp3Size = QString::number(fileSize/1024/1024,10)+"MB";
                    }
                    else
                        mp3Size = QString::number(fileSize/1024,10)+"KB";
                }
                else
                    mp3Size = QString::number(fileSize,10)+"B";
                bytes = mp3Name.toLocal8Bit();

                if(mp3Name.split(".").last() != "amr" && mp3Name.split(".").last() != "mmf")
                {
                    TagLib::FileRef f(bytes.data());
                    if(f.isNull())
                    {
                        //can't read this music;
                        continue;
                    }
                    TagLib::PropertyMap tags = f.file() ->properties();

                    //ALBUM专辑 ARTIST艺术家 TITLE标题
                    titleStr = tags["TITLE"].toString().toCString(true);
                    if(titleStr.isEmpty())
                        titleStr = fileInfo.baseName();
    //                qDebug()<<"titleStr: "<<titleStr;

                    artistStr = tags["ARTIST"].toString().toCString(true);
                    if(artistStr.isEmpty())
                        artistStr = "未知歌手";
    //                qDebug()<<"artistStr: "<<artistStr;

                    albumStr = tags["ALBUM"].toString().toCString(true);
                    if(albumStr.isEmpty())
                        albumStr = "未知专辑";
    //                qDebug()<<"albumStr:"<<albumStr;

                    TagLib::AudioProperties *properties = f.audioProperties();

                    int seconds = properties->length() % 60;
                    int minutes = (properties->length() - seconds) / 60;
                    timeStr = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
    //                qDebug()<<"timeStr"<<timeStr;
    //                qDebug() << "mp3size" << mp3Size;
                }
                else
                {
                    TagLib::FileRef f(bytes.data());

                        titleStr = fileInfo.baseName();

                        artistStr = "未知歌手";

                        albumStr = "未知专辑";

                      qDebug() << "mp3size" << mp3Size;
                }
                QListWidgetItem *listItem = new QListWidgetItem(musicInfoWidget);
                SongItem *songitem1 = new SongItem;
                musicInfoWidget->setItemWidget(listItem,songitem1);
//                songitem1->songNameLabel->setText(titleStr); //歌曲名称
//                songitem1->albumLabel->setText(albumStr);  //专辑
//                songitem1->songTimeLabel->setText(timeStr); //时长
//                songitem1->singerLabel->setText(artistStr); //歌手
                songitem1->song_singer_albumText(titleStr,artistStr,albumStr); //歌曲名称 歌手 专辑
                songitem1->songTimeLabel->setText(timeStr); //时长

                localModel->setTable("LocalMusic");
                localModel->select();

                int id = 0;
                if (localModel->rowCount() != 0)
                {
                    int num = localModel->data(localModel->index(localModel->rowCount()-1,0)).toInt();
                    id = num + 1;
                }

                allmusic.append(mp3Name);
                localAllMusicid.append(QString::number(id));
                PlayList->addMedia(QUrl::fromLocalFile(mp3Name));

                query.exec(QString("insert into LocalMusic (id,musicname,filepath,singer,album,type,size,time) values (%1,'%2','%3','%4','%5','%6','%7','%8')")
                           .arg(id).arg(titleStr).arg(mp3Name).arg(artistStr).arg(albumStr).arg(type).arg(mp3Size).arg(timeStr));

                songNumberLabel->setText(tr("A total of")+QString::number(musicInfoWidget->count())+tr("The first"));
            }
        }
        Music->setPlaylist(PlayList);
    }
    isStartPlay = false;
}

void MusicListWid::addSong(const QStringList &add_song)
{

}

void MusicListWid::get_localmusic_information()
{
    qDebug() << "数据库" << "获取歌曲列表歌曲信息" ;
    QSqlQuery query;
    query.exec("select * from LocalMusic");  //本地音乐列表数据

    while(query.next())
    {
//        qDebug() << "select * from LocalMusic " << query.value(1).toString() << query.value(2).toString()
//                 << query.value(3).toString() << query.value(4).toString() << query.value(7).toString();

        int Id = query.value(0).toInt();
//        qDebug()<<Id;
        QString songName = query.value(1).toString();
        QString Path = query.value(2).toString();
        QString Title = query.value(3).toString();
        QString Album = query.value(4).toString();
        QString Time = query.value(7).toString();

        QFileInfo fileinfo(Path);
        if(!fileinfo.exists())
        {
            continue;
        }

        if(songName != "" && Path != "")
        {
            allmusic.append(Path);
            // 添加到本地音乐所有id中
            localAllMusicid.append(QString::number(Id));

            QListWidgetItem *item1=new QListWidgetItem(musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            musicInfoWidget->setItemWidget(item1,songitem1);

//            songitem1->songNameLabel->setText(songName); //歌曲名称
//            songitem1->singerLabel->setText(Title); //歌手
//            songitem1->albumLabel->setText(Album);  //专辑
            songitem1->song_singer_albumText(songName,Title,Album); //歌曲名称 歌手 专辑
            songitem1->songTimeLabel->setText(Time); //时长

            PlayList->addMedia(QUrl::fromLocalFile(Path));
        }
    }

    qDebug() << "数据库" << "歌曲信息加载完毕" ;
    count = allmusic.size();
//    songNumberLabel->setText("共"+QString::number(musicInfoWidget->count())+"首");
    songNumberLabel->setText(tr("A total of")+QString::number(musicInfoWidget->count())+tr("The first"));

//    qDebug()<<"allmusic.size() :"<<allmusic.size();
    Music->setPlaylist(PlayList);
}

QStringList MusicListWid::get_info_from_db(int musicid)
{
    qDebug() << "数据库" << "从数据库获取该歌曲信息" ;

    QStringList musicinfo;
    localModel->setTable("LocalMusic");
    localModel->select();

    QString musicname = localModel->data(localModel->index(musicid,1)).toString();
    QString filepath = localModel->data(localModel->index(musicid,2)).toString();
    QString singer = localModel->data(localModel->index(musicid,3)).toString();
    QString album = localModel->data(localModel->index(musicid,4)).toString();
    QString type = localModel->data(localModel->index(musicid,5)).toString();
    QString size = localModel->data(localModel->index(musicid,6)).toString();
    QString time = localModel->data(localModel->index(musicid,7)).toString();

    musicinfo << musicname << filepath << singer << album << type << size << time;

    qDebug() << "数据库" << "musicinfo" << musicinfo;

    return musicinfo;
}

void MusicListWid::musiclistcolor()
{
    if(WidgetStyle::themeColor == 1 )
    {
        this->setObjectName("changelistwid");
        this->setStyleSheet("#changelistwid{background-color:#252526;}");
        musicInfoWidget->setStyleSheet("QListWidget{background-color:#252526;border:0px;}"
                                       "QListWidget::item{height:40px;}"
                                       "QListWidget::item:selected{background-color:#303032;}"
                                       "QListWidget::item:hover{background-color:#303032;}");

//        songLabel->setText("歌曲");
        songLabel->setText(tr("song"));
        songLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

//        singerLabel->setText("歌手");
        singerLabel->setText(tr("singer"));
        singerLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

//        albumLabel->setText("专辑");
        albumLabel->setText(tr("The album"));
        albumLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

//        timeLabel->setText("时长");
        timeLabel->setText(tr("The length"));
        timeLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

        top_addSongBtn->setStyleSheet("QToolButton{font-size: 14px;line-height: 14px;font-weight: 400;\
                                        \
                                       background:#393A3B;padding-left:10px;\
                                       color:#F9F9F9;border-radius:15px;}"
                                       "QToolButton::hover{color:#8F9399;}"
                                       "QToolButton::pressed{background:#ECEEF5;}");

        songNumberLabel->setStyleSheet("font-size: 14px;margin-top:12px;\
                                        \
                                       font-weight: 400;\
                                       border:none;\
                                       color: #8F9399;\
                                       line-height: 14px;");

        songListLabel->setStyleSheet("width:96px;height:24px;"
                                      "font-size: 24px;\
                                       \
                                      font-weight: 600;\
                                      border:none;\
                                      color: #F9F9F9;\
                                      line-height: 24px;");

    }
    else if (WidgetStyle::themeColor == 0)
    {
//        this->setStyleSheet("background-color:#FFFFFF;");
        this->setObjectName("changelistwid");
        this->setStyleSheet("#changelistwid{background-color:#FFFFFF;}");

        musicInfoWidget->setStyleSheet("QListWidget{background-color:#FFFFFF;border:0px;}"
                                       "QListWidget::item{height:40px;}"
                                       "QListWidget::item:selected{background-color:#F0F0F0;}"
                                       "QListWidget::item:hover{background-color:#F0F0F0;}");

//        songLabel->setText("歌曲");
        songLabel->setText(tr("song"));
        songLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

//        singerLabel->setText("歌手");
        singerLabel->setText(tr("singer"));
        singerLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

//        albumLabel->setText("专辑");
        albumLabel->setText(tr("The album"));
        albumLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

//        timeLabel->setText("时长");
        timeLabel->setText(tr("The length"));
        timeLabel->setStyleSheet("width:32px;height:16px;font-size:14px; "
                                     "font-weight:400;color:#8F9399;line-height:14px;");

        top_addSongBtn->setStyleSheet("QToolButton{font-size: 14px;line-height: 14px;font-weight: 400;\
                                        \
                                       background:#F2F6FD;padding-left:10px;\
                                       color:#303133;border-radius:15px;}"
                                       "QToolButton::hover{color:#8F9399;}"
                                       "QToolButton::pressed{background:#ECEEF5;}");

        songNumberLabel->setStyleSheet("font-size: 14px;margin-top:12px;\
                                        \
                                       font-weight: 400;\
                                       border:none;\
                                       color: #8F9399;\
                                       line-height: 14px;");

        songListLabel->setStyleSheet("width:96px;height:24px;"
                                      "font-size: 24px;\
                                       \
                                      font-weight: 600;\
                                      border:none;\
                                      color: #303133;\
                                      line-height: 24px;");

    }
}

void MusicListWid::dragEnterEvent(QDragEnterEvent *event)
{
    // 判断拖拽文件类型，文件名 接收该动作
    if(!event->mimeData()->hasFormat("text/uri-list"))
    {
        event->setDropAction(Qt::CopyAction);
        event->acceptProposedAction();
    }
    QWidget::dragEnterEvent(event);
}

void MusicListWid::dragMoveEvent(QDragMoveEvent *event)
{
//    event->setDropAction(Qt::MoveAction);
}

void MusicListWid::dragLeaveEvent(QDragLeaveEvent *event)
{
//    QWidget::dragLeaveEvent(event);
}

void MusicListWid::dropEvent(QDropEvent *event)
{
    // 判断拖拽文件类型，文件名 接收该动作
    if(!event->mimeData()->hasFormat("text/uri-list"))
    {
        return ;
    }

    auto urls = event->mimeData()->urls();
    qDebug()<<"urls urls urls urls urls urls urls:"<<urls;
    QStringList localpath;
    for(auto &url : urls)
    {
        localpath << url.toLocalFile();
    }

    if(!localpath.isEmpty())
    {
        addSong(localpath);
    }
}
