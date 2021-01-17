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
#include "musicDataBase.h"

ChangeListWid::ChangeListWid(QWidget *parent):QWidget(parent)
{
    this->setObjectName("ChangeListWid");
    this->setAcceptDrops(true);
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

    n_addLocalSong->setGeometry(300,357,126,30);
//    n_addLocalSong->setText("添加本地歌曲");
    n_addLocalSong->setText(tr("Add local songs"));


    n_addLocalFolder->setGeometry(385,357,126,30);
//    n_addLocalFolder->setText("添加本地文件夹");
    n_addLocalFolder->setText(tr("Add local folders"));
    n_addLocalFolder->hide();
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

        n_addLocalSong->setStyleSheet("QPushButton{font-size:14px;color:rgba(249, 249, 249, 1);border-radius:15px;border:2px solid #DDDFE7;"
                                      "font-weight: 400;width:84px;height:14px;}"
                                      "QPushButton::hover{color:#8F9399;}"
                                      "QPushButton::pressed{background:#ECEEF5;}");

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

        n_addLocalSong->setStyleSheet("QPushButton{font-size:14px;color:#303133;border-radius:15px;border:2px solid #DDDFE7;"
                                      "font-weight: 400;width:84px;height:14px;}"
                                      "QPushButton::hover{color:#8F9399;}"
                                      "QPushButton::pressed{background:#ECEEF5;}"
                                      );

//        top_addSongBtn->setStyleSheet("QToolButton{font-size: 14px;line-height: 14px;font-weight: 400;\
//                                       background:#F2F6FD;padding-left:10px;\
//                                       color:#303133;border-radius:15px;}"
//                                       "QToolButton::hover{color:#8F9399;}"
//                                       "QToolButton::pressed{background:#ECEEF5;}");

        n_addLocalFolder->setStyleSheet("font-size:14px;color:#303133;border-radius:15px;border:2px solid #DDDFE7;"
                                        " \
                                        font-weight: 400;width:84px;height:14px;");
    }

}

MusicListWid::MusicListWid(QWidget *parent):QWidget(parent)
{
    this->setObjectName("ChangeListWid");
    setAcceptDrops(true);



    initMusicListWid();
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


    songNumberLabel = new QLabel();
//    songNumberLabel->setGeometry(142,28,40,18);
//    songNumberLabel->setText("共"+QString::number(count)+"首");
    songNumberLabel->setText(tr("A total of")+QString::number(count)+tr("The first"));



    top_addSongBtn = new QToolButton();
//    top_playAllSongBtn = new QToolButton(this);

//    top_playAllSongBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    top_playAllSongBtn->setIcon(QIcon(":/img/default/play_w.png"));
//    top_playAllSongBtn->setIconSize(QSize(16,16));
//    top_playAllSongBtn->setText("播放全部");
//    top_playAllSongBtn->setText(tr("Play all"));
//    top_playAllSongBtn->setFixedSize(100,30);
//    top_playAllSongBtn->setStyleSheet("font-size: 14px;line-height: 14px;font-weight: 400;\
//                                        \
//                                       background:#FF4848;padding-left:10px;\
//                                       color:#FFFFFF;border-radius:15px;");



    top_addSongBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    top_addSongBtn->setIcon(QIcon(":/img/default/add.png"));
    top_addSongBtn->setIcon(QIcon(":/img/default/add.png"));
    top_addSongBtn->setIconSize(QSize(16,16));
//    top_addSongBtn->setText("添加歌曲");
    top_addSongBtn->setText(tr("Add the songs"));
    top_addSongBtn->setFixedSize(100,30);


//    connect(top_addSongBtn,SIGNAL(clicked()),this,SLOT(on_top_addSongBtn_slot()));
    connect(top_addSongBtn,&QToolButton::clicked,this,&MusicListWid::on_top_addSongBtn_slot);

    songLabel = new QLabel();
    singerLabel = new QLabel();
    albumLabel = new QLabel();
    timeLabel = new QLabel();


    hTitleLayout->addWidget(songListLabel,0,Qt::AlignLeft);
    hTitleLayout->addWidget(songNumberLabel,Qt::AlignLeft);
//    hTitleLayout->addWidget(top_playAllSongBtn);
    hTitleLayout->addWidget(top_addSongBtn);



//    hTitleListLayout = new QHBoxLayout(this);

    QHBoxLayout *testLayout = new QHBoxLayout();

    hTitleLayout->addWidget(songLabel,278);
    hTitleLayout->addWidget(singerLabel,174);
    hTitleLayout->addWidget(albumLabel,174);
    hTitleLayout->addWidget(timeLabel,64);
    hTitleLayout->setContentsMargins(30,0,0,0);

    testLayout->addWidget(songListLabel,0,Qt::AlignLeft);
    testLayout->addWidget(songNumberLabel,Qt::AlignLeft);
//    testLayout->addWidget(top_playAllSongBtn);
    testLayout->addWidget(top_addSongBtn);
    testLayout->setContentsMargins(30,0,30,0);

    QWidget *testWid = new QWidget();
    testWid->setLayout(testLayout);
    testWid->setFixedHeight(80);

//    testWid->setStyleSheet("background:red;");

//    testWid->resize(750, 40);

    QWidget *testWid2 = new QWidget();
    testWid2->setLayout(hTitleLayout);
    testWid2->setFixedHeight(40);
//    testWid2->setStyleSheet("background:blue;");

    vMainLayout->addWidget(testWid,Qt::AlignTop);
    vMainLayout->addWidget(testWid2,0,Qt::AlignTop);

    testWid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    musicInfoWidget = new QListWidget();

    vMainLayout->addWidget(musicInfoWidget);

    vMainLayout->setMargin(0);
    vMainLayout->setSpacing(0);

    PlayList = new QMediaPlaylist();

    Music = new QMediaPlayer();

    this->setLayout(vMainLayout);
}


void MusicListWid::on_top_addSongBtn_slot()
{
    songFiles = QFileDialog::getOpenFileNames(this, tr("Open the file"),"","音乐文件(*.mp3 *.ogg *.wav *.wma *.spx *.ape *.flac)");  //歌曲文件
    if(!songFiles.isEmpty())
    {
        for(int i = 0; i < songFiles.size(); i++)
        {
            filepath(songFiles.at(i));
            fileInfo.setFile(musicdataStruct.filepath);
            fileType(fileInfo);          //文件类型
            fileSize(fileInfo);      //文件大小
            fileInformation(musicdataStruct.filepath);          //获取歌曲文件信息
            showFileInformation(musicdataStruct.title,musicdataStruct.singer,musicdataStruct.album,musicdataStruct.time);  //显示获取歌曲文件信息
            filepathHash(musicdataStruct.filepath);                     // 通过路径获取hash
            allmusic.append(musicdataStruct.filepath);
            qDebug()<<musicdataStruct.hash;
            localAllMusicid.append(musicdataStruct.hash);
            PlayList->addMedia(QUrl::fromLocalFile(musicdataStruct.filepath));
            qDebug()<<localAllMusicid;

            g_db->addMusicToLocalMusic(musicdataStruct);
            songNumberLabel->setText(tr("A total of")+QString::number(musicInfoWidget->count())+tr("The first"));
        }
    }
}

QString MusicListWid::filepathHash(QString filePath)
{
    QString musicHash;
    QByteArray qByteArray;
    QCryptographicHash hash(QCryptographicHash::Md5);

    qByteArray.append(filePath);
    hash.addData(qByteArray);
    musicHash.append(hash.result().toHex());
    musicdataStruct.hash = musicHash;
    return musicdataStruct.hash;
}

QString MusicListWid::filepath(QString filepath)
{
    musicdataStruct.filepath = filepath;
    return musicdataStruct.hash;
}

QStringList MusicListWid::fileInformation(QString filepath)
{
    QByteArray byteArray = filepath.toLocal8Bit();
    TagLib::FileRef f(byteArray.data());
//    if(f.isNull())
//    {
//        //can't read this music;
//        continue;
//    }
    TagLib::PropertyMap propertyMap = f.file() ->properties();

    QString musicName = propertyMap["TITLE"].toString().toCString(true);
    if(musicName.isEmpty())
        musicName = fileInfo.baseName();
    QString musicSinger = propertyMap["ARTIST"].toString().toCString(true);
    if(musicSinger.isEmpty())
        musicSinger = "未知歌手";
    QString musicAlbum = propertyMap["ALBUM"].toString().toCString(true);
    if(musicAlbum.isEmpty())
        musicAlbum = "未知专辑";
    TagLib::AudioProperties *properties = f.audioProperties();

    int seconds = properties->length() % 60;
    int minutes = (properties->length() - seconds) / 60;
    QString musicTime = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
    musicdataStruct.title = musicName;
    musicdataStruct.singer = musicSinger;
    musicdataStruct.album = musicAlbum;
    musicdataStruct.time = musicTime;
    QStringList audioFileInformation;
    audioFileInformation << musicdataStruct.title << musicdataStruct.singer
                         << musicdataStruct.album << musicdataStruct.time;
    return audioFileInformation;
}

QString MusicListWid::fileSize(QFileInfo fileInfo)
{
    QString musicSize;
    qint64 size = fileInfo.size();   //文件大小
    if(size/1024)
    {
        if(size/1024/1024)
        {
            if(size/1024/1024/1024)
            {
                musicSize = QString::number(size/1024/1024/1024,10)+"GB";
            }
            else
                musicSize = QString::number(size/1024/1024,10)+"MB";
        }
        else
            musicSize = QString::number(size/1024,10)+"KB";
    }
    else
        musicSize = QString::number(size,10)+"B";
    musicdataStruct.size = musicSize;
    return musicdataStruct.size;
}

QString MusicListWid::fileType(QFileInfo fileInfo)
{
    QString musicType = fileInfo.suffix();        //文件类型
    musicdataStruct.filetype = musicType;
    return musicdataStruct.filetype;
}

void MusicListWid::showFileInformation(QString musicName,QString musicSinger,QString musicAlbum,QString musicTime)
{
    QListWidgetItem *listItem = new QListWidgetItem(musicInfoWidget);
    SongItem *songitem1 = new SongItem;
    musicInfoWidget->setItemWidget(listItem,songitem1);
    songitem1->song_singer_albumText(musicName,musicSinger,musicAlbum); //歌曲名称 歌手 专辑
    songitem1->songTimeLabel->setText(musicTime); //时长
}

void MusicListWid::addFile(const QStringList &addFile)
{
//    QSqlQuery query;
//    if(!addFile.isEmpty())
//    {
//        for(int i = 0; i < addFile.size(); i++)
//        {
//            Dir = QDir::toNativeSeparators(addFile.at(i));
//             = addFile.at(i);
//            if(!.isEmpty())
//            {
//                bool songExists = false;
//                for(QString &songPlaylist:allmusic)
//                {
//                    if( == songPlaylist)    //通过路径判断歌曲是否存在
//                    {
//                        qDebug()<<"歌曲已存在 (通过路径判断歌曲是否存在)";
////                        qDebug()<<"songPlaylist : "<<songPlaylist;
//                        songExists = true;
//                        break;
//                    }
//                }
//                if(songExists)
//                {
//                    continue;
//                }

//                fileInfo.setFile();

//                musicType = fileInfo.suffix();        //文件类型

//                qint64 musicFileSize = fileInfo.size();
//                if(musicFileSize/1024)
//                {
//                    if(musicFileSize/1024/1024)
//                    {
//                        if(musicFileSize/1024/1024/1024)
//                        {
//                            musicSize = QString::number(musicFileSize/1024/1024/1024,10)+"GB";
//                        }
//                        else
//                            musicSize = QString::number(musicFileSize/1024/1024,10)+"MB";
//                    }
//                    else
//                        musicSize = QString::number(musicFileSize/1024,10)+"KB";
//                }
//                else
//                    musicSize = QString::number(musicFileSize,10)+"B";
//                byteArray = .toLocal8Bit();

//                if(.split(".").last() != "amr" && .split(".").last() != "mmf")
//                {
//                    TagLib::FileRef f(byteArray.data());
//                    if(f.isNull())
//                    {
//                        //can't read this music;
//                        continue;
//                    }
//                    TagLib::PropertyMap propertyMap = f.file() ->properties();

//                    musicName = propertyMap["TITLE"].toString().toCString(true);
//                    if(musicName.isEmpty())
//                        musicName = fileInfo.baseName();
//                    musicSinger = propertyMap["ARTIST"].toString().toCString(true);
//                    if(musicSinger.isEmpty())
//                        musicSinger = "未知歌手";
//                    musicAlbum = propertyMap["ALBUM"].toString().toCString(true);
//                    if(musicAlbum.isEmpty())
//                        musicAlbum = "未知专辑";
//                    TagLib::AudioProperties *properties = f.audioProperties();

//                    int seconds = properties->length() % 60;
//                    int minutes = (properties->length() - seconds) / 60;
//                    musicTime = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
//                }
//                else
//                {
//                    TagLib::FileRef f(byteArray.data());

//                        musicName = fileInfo.baseName();

//                        musicSinger = "未知歌手";

//                        musicAlbum = "未知专辑";

//                      qDebug() << "musicSize" << musicSize;
//                }
//                QListWidgetItem *listItem = new QListWidgetItem(musicInfoWidget);
//                SongItem *songitem1 = new SongItem;
//                musicInfoWidget->setItemWidget(listItem,songitem1);
//                songitem1->song_singer_albumText(musicName,musicSinger,musicAlbum); //歌曲名称 歌手 专辑
//                songitem1->songTimeLabel->setText(musicTime); //时长

//                localModel->setTable("LocalMusic");
//                localModel->select();

//                int id = 0;
//                if (localModel->rowCount() != 0)
//                {
//                    int num = localModel->data(localModel->index(localModel->rowCount()-1,0)).toInt();
//                    id = num + 1;
//                }

//                allmusic.append();
//                localAllMusicid.append(QString::number(id));
//                PlayList->addMedia(QUrl::fromLocalFile());

//                query.exec(QString("insert into LocalMusic (id,musicname,filepath,singer,album,type,size,time) values (%1,'%2','%3','%4','%5','%6','%7','%8')")
//                           .arg(id).arg(musicName).arg().arg(musicSinger).arg(musicAlbum).arg(musicType).arg(musicSize).arg(musicTime));

//                songNumberLabel->setText(tr("A total of")+QString::number(musicInfoWidget->count())+tr("The first"));
//            }
//        }
//        Music->setPlaylist(PlayList);
////        Music->play();
////        Music->stop();
//        int position;
//        if(Music->state() == QMediaPlayer::PlayingState)
//        {
//            position = Music->position();
//            Music->setPosition(position);
//            Music->play();
//            isStartPlay = true;
//        }
//        else
//        {
//            Music->stop();
//            isStartPlay = false;
//        }
//    }
}

void MusicListWid::get_localmusic_information(QString tableName)
{
    int ret = -1;
    QList<musicDataStruct> resList;
    if(tableName == "LocalMusic") {
        ret = g_db->getSongInfoListFromLocalMusic(resList);
    } else {
        ret = g_db->getSongInfoListFromPlayList(resList, tableName);
    }
    //qDebug()<<"==================="<<resList.size()<< ",ret = " << ret << "tablename is " << tableName;
    this->tableName = tableName;
    for(int i = 0;i < resList.size(); i++)
    {
        if(resList.at(i).hash != "")
        {
            QListWidgetItem *item = new QListWidgetItem(this->musicInfoWidget);
            SongItem *songitem = new SongItem;
            this->musicInfoWidget->setItemWidget(item,songitem);
            //qDebug()<<resList.at(i).title;
            this->localAllMusicid.append(resList.at(i).hash);
            songitem->song_singer_albumText(resList.at(i).title,resList.at(i).singer,resList.at(i).album); //歌曲名称 歌手 专辑
            songitem->songTimeLabel->setText(resList.at(i).time); //时长
            this->PlayList->addMedia(QUrl::fromLocalFile(resList.at(i).filepath));
        }
    }
    qDebug() << "从数据库获取歌曲信息加载完毕" ;
}


QStringList MusicListWid::get_info_from_db(int musicid)
{

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

    qDebug() << "从数据库获取该歌曲信息:" << musicinfo;

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
                                       background:#393A3B;padding-left:10px;\
                                       color:#F9F9F9;border-radius:15px;}"
                                       "QToolButton::hover{color:#8F9399;}"
                                       "QToolButton::pressed{background:#ECEEF5;}");

        songNumberLabel->setStyleSheet("font-size: 14px;margin-top:12px;\
                                       font-weight: 400;\
                                       border:none;\
                                       color: #8F9399;\
                                       line-height: 14px;");

        songListLabel->setStyleSheet("width:96px;height:24px;"
                                      "font-size: 24px;\
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
                                       background:#F2F6FD;padding-left:10px;\
                                       color:#303133;border-radius:15px;}"
                                       "QToolButton::hover{color:#8F9399;}"
                                       "QToolButton::pressed{background:#ECEEF5;}");

        songNumberLabel->setStyleSheet("font-size: 14px;margin-top:12px;\
                                       font-weight: 400;\
                                       border:none;\
                                       color: #8F9399;\
                                       line-height: 14px;");

        songListLabel->setStyleSheet("width:96px;height:24px;"
                                      "font-size: 24px;\
                                      font-weight: 600;\
                                      border:none;\
                                      color: #303133;\
                                      line-height: 24px;");

    }
}
