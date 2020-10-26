#include "changelistwid.h"
#include "songitem.h"

ChangeListWid::ChangeListWid(QWidget *parent):QWidget(parent)
{
    this->setObjectName("ChangeListWid");


    initStack();

    this->resize(750,80);

}


void ChangeListWid::initStack()
{
//    titleFrame = new QFrame(this);

    songListLabel = new QLabel(this);
    songListLabel->setGeometry(30,18,100,30);
    songListLabel->setText("歌曲列表");
    songListLabel->setStyleSheet("width:96px;height:24px;"
                                 "font-size: 24px;\
                                 font-family: PingFangSC-Regular, PingFang SC;\
                                 font-weight: 600;\
                                 border:none;\
                                 color: #303133;\
                                 line-height: 24px;");

    songNumberLabel = new QLabel(this);
    songNumberLabel->setGeometry(142,28,40,18);
    songNumberLabel->setText("共0首");
    songNumberLabel->setStyleSheet("font-size: 14px;\
                                   font-family: PingFangSC-Regular, PingFang SC;\
                                   font-weight: 400;\
                                   border:none;\
                                   color: #8F9399;\
                                   line-height: 14px;");

    nullMusicIconLabel = new QLabel(this);
    nullMusicIconLabel->setGeometry(275,122,200,180);
    nullMusicIconLabel->setStyleSheet("border-image:url(:/img/default/pict1.png)");

    nullMusicLabel = new QLabel(this);
    nullMusicLabel->setGeometry(333,313,84,24);
    nullMusicLabel->setText("还没有歌曲！");
    nullMusicLabel->setStyleSheet("font-size: 14px;\
                                  font-family: PingFangSC-Regular, PingFang SC;\
                                  font-weight: 400;\
                                  border:none;\
                                  color: #000000;\
                                  line-height:24px;");

    n_addLocalSong = new QPushButton(this);
    n_addLocalFolder = new QPushButton(this);

    n_addLocalSong->setGeometry(240,357,126,30);
    n_addLocalSong->setText("添加本地歌曲");
    n_addLocalSong->setStyleSheet("font-size:14px;color:#303133;border-radius:15px;border:2px solid #DDDFE7;"
                                  "font-family: PingFangSC-Regular, PingFang SC;\
                                  font-weight: 400;width:84px;height:14px;");

    n_addLocalFolder->setGeometry(385,357,126,30);
    n_addLocalFolder->setText("添加本地文件夹");
    n_addLocalFolder->setStyleSheet("font-size:14px;color:#303133;border-radius:15px;border:2px solid #DDDFE7;"
                                    "font-family: PingFangSC-Regular, PingFang SC;\
                                    font-weight: 400;width:84px;height:14px;");

}

MusicListWid::MusicListWid(QWidget *parent):QWidget(parent)
{
    this->setObjectName("ChangeListWid");


    initMusicListWid();

//    this->resize(750,80);

}


void MusicListWid::initMusicListWid()
{//    titleFrame = new QFrame(this);



    hTitleLayout = new QHBoxLayout(this);


    vMainLayout = new QVBoxLayout(this);
    vListWidLayout = new QVBoxLayout(this);



    songListLabel = new QLabel(this);
//    songListLabel->setGeometry(30,18,100,30);
    songListLabel->setText("歌曲列表");
    songListLabel->setStyleSheet("width:96px;height:24px;"
                                 "font-size: 24px;\
                                 font-family: PingFangSC-Regular, PingFang SC;\
                                 font-weight: 600;\
                                 border:none;\
                                 color: #303133;\
                                 line-height: 24px;");


    songNumberLabel = new QLabel(this);
//    songNumberLabel->setGeometry(142,28,40,18);
    songNumberLabel->setText("共"+QString::number(count)+"首");
    songNumberLabel->setStyleSheet("font-size: 14px;margin-top:12px;\
                                   font-family: PingFangSC-Regular, PingFang SC;\
                                   font-weight: 400;\
                                   border:none;\
                                   color: #8F9399;\
                                   line-height: 14px;");



    top_addSongBtn = new QToolButton(this);
//    top_playAllSongBtn = new QToolButton(this);

//    top_playAllSongBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//    top_playAllSongBtn->setIcon(QIcon(":/img/default/play_w.png"));
//    top_playAllSongBtn->setIconSize(QSize(16,16));
//    top_playAllSongBtn->setText("播放全部");
//    top_playAllSongBtn->setFixedSize(100,30);
//    top_playAllSongBtn->setStyleSheet("font-size: 14px;line-height: 14px;font-weight: 400;\
//                                       font-family: PingFangSC-Regular;\
//                                       background:#FF4848;padding-left:10px;\
//                                       color:#FFFFFF;border-radius:15px;");



    top_addSongBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    top_addSongBtn->setIcon(QIcon(":/img/default/add.png"));
    top_addSongBtn->setIconSize(QSize(16,16));
    top_addSongBtn->setText(" 添加歌曲");
    top_addSongBtn->setFixedSize(100,30);
    top_addSongBtn->setStyleSheet("QToolButton{font-size: 14px;line-height: 14px;font-weight: 400;\
                                   font-family: PingFangSC-Regular;\
                                   background:#F2F6FD;padding-left:10px;\
                                   color:#303133;border-radius:15px;}"
                                   "QToolButton::hover{color:#8F9399;}"
                                   "QToolButton::pressed{background:#ECEEF5;}");

    connect(top_addSongBtn,SIGNAL(clicked()),this,SLOT(on_top_addSongBtn_slot()));

    songLabel = new QLabel(this);
    singerLabel = new QLabel(this);
    albumLabel = new QLabel(this);
    timeLabel = new QLabel(this);

    songLabel->setText("歌曲");
    songLabel->setStyleSheet("width:32px;height:16px;font-size:14px;font-family: PingFangSC-Regular;"
                                 "font-weight:400;color:#8F9399;line-height:14px;");

    singerLabel->setText("歌手");
    singerLabel->setStyleSheet("width:32px;height:16px;font-size:14px;font-family: PingFangSC-Regular;"
                                 "font-weight:400;color:#8F9399;line-height:14px;");

    albumLabel->setText("专辑");
    albumLabel->setStyleSheet("width:32px;height:16px;font-size:14px;font-family: PingFangSC-Regular;"
                                 "font-weight:400;color:#8F9399;line-height:14px;");

    timeLabel->setText("时长");
    timeLabel->setStyleSheet("width:32px;height:16px;font-size:14px;font-family: PingFangSC-Regular;"
                                 "font-weight:400;color:#8F9399;line-height:14px;");




//    hTitleLayout->addWidget(songListLabel,0,Qt::AlignLeft);
//    hTitleLayout->addWidget(songNumberLabel,Qt::AlignLeft);
//    hTitleLayout->addWidget(top_playAllSongBtn);
//    hTitleLayout->addWidget(top_addSongBtn);


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


//    SongItem *mySongItem = new SongItem(this);
//    SongItem *mySongItem2 = new SongItem(this);
//    SongItem *mySongItem3 = new SongItem(this);

//    vMainLayout->addWidget(mySongItem);
//    vMainLayout->addWidget(mySongItem2);
//    vMainLayout->addWidget(mySongItem3);



//    vMainLayout->addLayout(hTitleLayout);




    testWid->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);



    musicInfoWidget = new QListWidget(this);

    musicInfoWidget->setStyleSheet("QListWidget{background-color:#FFFFFF;border:0px;}"
                                   "QListWidget::item{height:40px;}"
                                   "QListWidget::item:selected{background-color:#FAFAFA;color:#FF4848;}"
                                   "QListWidget::item:hover{background-color:#FAFAFA;}");


    vMainLayout->addWidget(musicInfoWidget);


//    QWidget *info1 = new QWidget(this);
//    testMusicInfo1 = new QListWidgetItem(musicInfoWidget);

//    QWidget *info2 = new QWidget(this);
//    testMusicInfo2 = new QListWidgetItem(musicInfoWidget);

//    QToolButton *infoSongName1 = new QToolButton(info1);

//    QToolButton *infoSongName2 = new QToolButton(info2);

//    QPushButton *infolove1 = new QPushButton(info1);
//    QPushButton *infolove2 = new QPushButton(info2);

//    QPushButton *infoplay1 = new QPushButton(info1);
//    QPushButton *infoplay2 = new QPushButton(info2);

//    QLabel *infosinger1 = new QLabel(info1);
//    QLabel *infosinger2 = new QLabel(info2);

//    QLabel *infoalbum1 = new QLabel(info1);
//    QLabel *infoalbum2 = new QLabel(info2);

//    QLabel *infotime1 = new QLabel(info1);
//    QLabel *infotime2 = new QLabel(info2);


    PlayList = new QMediaPlaylist(this);

    Music = new QMediaPlayer(this);

    this->setLayout(vMainLayout);
    QSqlQuery query;
    query.exec("select * from LocalMusic");  //本地音乐列表数据

    while(query.next())
    {
        qDebug() << "select * from LocalMusic " << query.value(1).toString()<<query.value(2).toString()
                 <<query.value(3).toString()<<query.value(4).toString();
//        int Id = query.value(0).toInt();
//        qDebug()<<Id;
        QString songName = query.value(1).toString();
        QString Path = query.value(2).toString();
        QString Title = query.value(3).toString();
        QString Album = query.value(4).toString();
        QString Time = query.value(7).toString();

        allmusic.append(Path);

        if(songName != ""&&Path != "")
        {
            QListWidgetItem *item1=new QListWidgetItem(musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            musicInfoWidget->setItemWidget(item1,songitem1);

            songitem1->songNameLabel->setText(songName); //歌曲名称
            songitem1->albumLabel->setText(Album);  //专辑
            songitem1->songTimeLabel->setText(Time); //时长
            songitem1->singerLabel->setText(Title); //歌手

            PlayList->addMedia(QUrl::fromLocalFile(Path));
        }
    }
    count = allmusic.size();
    songNumberLabel->setText("共"+QString::number(count)+"首");
//    qDebug()<<"allmusic.size() :"<<allmusic.size();
    Music->setPlaylist(PlayList);
}


void MusicListWid::on_top_addSongBtn_slot()
{
    QSqlQuery query;
//    query.exec("select * from LocalMusic");
    //    QFileInfo info;
    songFiles = QFileDialog::getOpenFileNames(this, "打开文件","Music","music(*.mp3 *.ogg *.wav *.wma *.spx *.ape *.flac *.amr *.mmf)");  //歌曲文件
    if(!songFiles.isEmpty())
    {

        qDebug() << "songFiles.size()" << songFiles.size();

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
                        qDebug()<<"songPlaylist : "<<songPlaylist;
                        songExists = true;
                        break;
                    }
                }
                if(songExists)
                {
                    continue;
                }
            }
            PlayList->addMedia(QUrl::fromLocalFile(mp3Name));


            fileInfo.setFile(mp3Name);
            qDebug()<<fileInfo.size()<<"1111";
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
//                TagLib::PropertyMap tags = f.file() ->properties();

                //ALBUM专辑 ARTIST艺术家 TITLE标题
//                titleStr = tags["TITLE"].toString().toCString(true);
//                if(titleStr.isEmpty())
                    titleStr = fileInfo.baseName();
                qDebug()<<"titleStr: "<<titleStr;

//                artistStr = tags["ARTIST"].toString().toCString(true);
//                if(artistStr.isEmpty())
                    artistStr = "未知歌手";
                qDebug()<<"artistStr: "<<artistStr;

//                albumStr = tags["ALBUM"].toString().toCString(true);
//                if(albumStr.isEmpty())
                    albumStr = "未知专辑";
                qDebug()<<"albumStr:"<<albumStr;

//                TagLib::AudioProperties *properties = f.audioProperties();
//                qDebug()<<"aaaaa";
//                int seconds = properties->length() % 60;
//                int minutes = (properties->length() - seconds) / 60;
//                qDebug()<<"bbbbbb";
//                timeStr = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
//                qDebug()<<"timeStr"<<timeStr;
                  qDebug() << "mp3size" << mp3Size;
            }
            QListWidgetItem *listItem = new QListWidgetItem(musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            musicInfoWidget->setItemWidget(listItem,songitem1);
            songitem1->songNameLabel->setText(titleStr); //歌曲名称
            songitem1->albumLabel->setText(albumStr);  //专辑
            songitem1->songTimeLabel->setText(timeStr); //时长
            songitem1->singerLabel->setText(artistStr); //歌手

            //            query.exec(QString("insert into LocalMusic (id, MusicName) values (1, '123')"));
            //            query.exec(QString("insert into LocalMusic (id,MusicName,FileName,singal) values (%1,'%2','%3','%4')")
            //                       .arg(qrand()%1000000).arg(titleStr).arg(mp3Name).arg(artistStr));
            query.exec(QString("insert into LocalMusic (musicname,filepath,singer,album,type,size,time) values ('%1','%2','%3','%4','%5','%6','%7')")
                       .arg(titleStr).arg(mp3Name).arg(artistStr).arg(albumStr).arg(type).arg(mp3Size).arg(timeStr));
//            query.exec(QString("insert into LocalMusic values (%1,'%2','%3','%4','%5','%6','%7','%8')")
//                       .arg(qrand()%1000000).arg(titleStr).arg(mp3Name).arg(artistStr).arg(albumStr).arg(type).arg(mp3Size).arg(timeStr));
            qDebug()<< QString("insert into LocalMusic (musicname,filepath,singer,album,type,size,time) values ('%1','%2','%3','%4','%5','%6','%7')")
                       .arg(titleStr).arg(mp3Name).arg(artistStr).arg(albumStr).arg(type).arg(mp3Size).arg(timeStr);
//            songNumberLabel->setText("共"+QString::number(count)+"首");
//            allmusic.append(mp3Name);
            count += 1;
            songNumberLabel->setText("共"+QString::number(count)+"首");
        }
    }
    Music->setPlaylist(PlayList);
}

