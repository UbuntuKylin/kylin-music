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
    QVBoxLayout *vNullMainLayout = new QVBoxLayout(this);

    QHBoxLayout *nullIconLabelLayout = new QHBoxLayout(this);
    QHBoxLayout *noSongLayout = new QHBoxLayout(this);
    QHBoxLayout *nullAddSongLayout = new QHBoxLayout(this);
    QVBoxLayout *vAddSongLayout = new QVBoxLayout(this);
    QVBoxLayout *vAddFolderLayout = new QVBoxLayout(this);

    songListLabel = new QLabel(this);
    songListLabel->setGeometry(30,18,120,30);
//    songListLabel->setText("歌曲列表");
    songListLabel->setText(tr("Song list"));


    songNumberLabel = new QLabel(this);
    songNumberLabel->setGeometry(142,28,100,24);
//    songNumberLabel->setText("共0首");
    songNumberLabel->setText(tr("A total of 0 first"));

    nullMusicIconLabel = new QLabel(this);
    nullMusicIconLabel->setFixedSize(200,179);
    nullMusicIconLabel->setStyleSheet("border-image:url(:/img/default/pict1.png)");

    nullMusicLabel = new QLabel(this);
    nullMusicLabel->setFixedSize(200,30);
    nullMusicLabel->setAlignment(Qt::AlignHCenter);
//    nullMusicLabel->setText("还没有歌曲！");
    nullMusicLabel->setText(tr("There are no songs yet!"));


    n_addLocalSong = new QPushButton(this);
    n_addLocalFolder = new QPushButton(this);
//    n_addLocalSong->setText("添加本地歌曲");
    n_addLocalSong->setFixedSize(126,30);
    n_addLocalSong->setText(tr("Add local songs"));

//    n_addLocalFolder->setText("添加本地文件夹");
    n_addLocalFolder->setFixedSize(126,30);
    n_addLocalFolder->setText(tr("Add local folders"));
    n_addLocalFolder->hide();

    nullIconLabelLayout->addWidget(nullMusicIconLabel,0,Qt::AlignHCenter);   //添加空页面默认图片
    noSongLayout->addWidget(nullMusicLabel,0,Qt::AlignHCenter);     //还没有歌曲！


    vAddSongLayout->addWidget(n_addLocalSong,0,Qt::AlignHCenter);
    vAddFolderLayout->addWidget(n_addLocalFolder,0,Qt::AlignHCenter);
    nullAddSongLayout->addStretch();
    nullAddSongLayout->addLayout(vAddSongLayout);
    nullAddSongLayout->setSpacing(20);
    nullAddSongLayout->addLayout(vAddFolderLayout);
    nullAddSongLayout->addStretch();

    vNullMainLayout->addStretch();
    vNullMainLayout->addLayout(nullIconLabelLayout);
    vNullMainLayout->setSpacing(12);
    vNullMainLayout->addLayout(noSongLayout);
    vNullMainLayout->setSpacing(20);
    vNullMainLayout->addLayout(nullAddSongLayout);
    vNullMainLayout->addStretch();

//    vNullMainLayout->setMargin(0);
//    vNullMainLayout->setSpacing(0);
    this->setLayout(vNullMainLayout);
}

void ChangeListWid::nullWidgetColor()
{
    if(WidgetStyle::themeColor == 1)
    {
        songListLabel->setStyleSheet("width:120px;height:30px;"
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
        songListLabel->setStyleSheet("width:120px;height:30px;"
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

        n_addLocalFolder->setStyleSheet("font-size:14px;color:#303133;border-radius:15px;border:2px solid #DDDFE7;"
                                        "font-weight: 400;width:84px;height:14px;");
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
    songListLabel->setText(tr("Song list"));


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
    top_addSongBtn->setText(tr("   Add"));
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
    hTitleLayout->setContentsMargins(30,0,0,0);

    testLayout->addWidget(songListLabel,0,Qt::AlignLeft);
    testLayout->addWidget(songNumberLabel,Qt::AlignLeft);
//    testLayout->addWidget(top_playAllSongBtn);
    testLayout->addWidget(top_addSongBtn);
    testLayout->setContentsMargins(30,0,30,0);

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

    //Music = new QMediaPlayer(this);
    Music = nullptr;
    this->setLayout(vMainLayout);
}
void MusicListWid::initialQMediaPlayer()
{
    if(Music == nullptr)
    {
        Music = new QMediaPlayer(this);
    }
    m_musicInitialed = true;
}

void MusicListWid::on_top_addSongBtn_slot()
{
    songFiles = QFileDialog::getOpenFileNames(this, tr("Open the file"),"","音乐文件(*.mp3 *.ogg *.wma *.spx *.flac)");  //歌曲文件
//    if(!songFiles.isEmpty())
//    {
//        for(int i = 0; i < songFiles.size(); i++)
//        {
//            filepath(songFiles.at(i));
//            fileInfo.setFile(musicdataStruct.filepath);
//            fileType(fileInfo);          //文件类型
//            fileSize(fileInfo);      //文件大小
//            fileInformation(musicdataStruct.filepath);//获取歌曲文件信息
////            filepathHash(musicdataStruct.filepath);// 通过路径获取hash
//            ret = g_db->addMusicToLocalMusic(musicdataStruct);
//            if (ret == DB_OP_SUCC) {
//                showFileInformation(musicdataStruct.title,musicdataStruct.singer,musicdataStruct.album,musicdataStruct.time);  //显示获取歌曲文件信息
//                localAllMusicid.append(musicdataStruct.filepath);
//                PlayList->addMedia(QUrl::fromLocalFile(musicdataStruct.filepath));
//                qDebug()<<"添加歌曲文件路径 ==== "<<musicdataStruct.filepath;
//                songNumberLabel->setText(tr("A total of")+QString::number(musicInfoWidget->count())+tr("The first"));
//            } else {
//                qDebug()<<"歌曲已存在";
//            }
//        }
//    }
    addFile(songFiles);
}

void MusicListWid::addFile(const QStringList &addFile)
{
    int ret;
    if(!addFile.isEmpty())
    {
        for(int i = 0; i < addFile.size(); i++)
        {
            filepath(addFile.at(i));
            fileInfo.setFile(musicdataStruct.filepath);
            fileType(fileInfo);          //文件类型
            fileSize(fileInfo);      //文件大小
            if(musicdataStruct.filetype == "mp3" || musicdataStruct.filetype == "ogg" ||
                    musicdataStruct.filetype == "wma" || musicdataStruct.filetype == "spx"||
                    musicdataStruct.filetype == "flac")
            {
                fileInformation(musicdataStruct.filepath);//获取歌曲文件信息
                //            filepathHash(musicdataStruct.filepath);// 通过路径获取hash
                ret = g_db->addMusicToLocalMusic(musicdataStruct);
                if (ret == DB_OP_SUCC) {
                    showFileInformation(musicdataStruct.title,musicdataStruct.singer,musicdataStruct.album,musicdataStruct.time);  //显示获取歌曲文件信息
                    localAllMusicid.append(musicdataStruct.filepath);
                    PlayList->addMedia(QUrl::fromLocalFile(musicdataStruct.filepath));
                    songNumberLabel->setText(tr("A total of")+QString::number(musicInfoWidget->count())+tr("The first"));
                } else {
                    qDebug()<<"歌曲已存在";
                }
            }
            else
            {
                QMessageBox::about(this,"提示信息","添加文件失败");
            }
        }
    }
}

//QString MusicListWid::filepathHash(QString filePath)
//{
//    QString musicHash;
//    QByteArray qByteArray;
//    QCryptographicHash hash(QCryptographicHash::Md5);

//    qByteArray.append(filePath);
//    hash.addData(qByteArray);
//    musicHash.append(hash.result().toHex());
//    musicdataStruct.hash = musicHash;
//    return musicdataStruct.hash;
//}

QString MusicListWid::filepath(QString filepath)
{
    musicdataStruct.filepath = filepath;
    return musicdataStruct.filepath;
}

inline int MusicListWid::preNum(unsigned char byte) {
    unsigned char mask = 0x80;
    int num = 0;
    for (int i = 0; i < 8; i++) {
        if ((byte & mask) == mask) {
            mask = mask >> 1;
            num++;
        } else {
            break;
        }
    }
    return num;
}

QString MusicListWid::filterTextCode(QString str)
{
    QByteArray strdata =str.toLocal8Bit();
    int len = strdata.length();
    int num = 0;
    int i = 0;
    while (i < len) {
        if ((strdata[i] & 0x80) == 0x00) {
            // 0XXX_XXXX
            i++;
            continue;
        }
        else if ((num = preNum(strdata[i])) > 2) {
            // 110X_XXXX 10XX_XXXX
            // 1110_XXXX 10XX_XXXX 10XX_XXXX
            // 1111_0XXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
            // 1111_10XX 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
            // 1111_110X 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX 10XX_XXXX
            // preNUm() 返回首个字节8个bits中首�?0bit前面1bit的个数，该数量也是该字符所使用的字节数
            i++;
            for(int j = 0; j < num - 1; j++) {
                //判断后面num - 1 个字节是不是都是10开
                if ((strdata[i] & 0xc0) != 0x80) {
                    return "";
                }
                i++;
            }
        } else {
            //其他情况说明不是utf-8
            return "";
        }
    }
    return str;
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
    if(filterTextCode(musicName).isEmpty())
        musicName = fileInfo.completeBaseName();
    QString musicSinger = propertyMap["ARTIST"].toString().toCString(true);
    if(filterTextCode(musicSinger).isEmpty())
        musicSinger = "未知歌手";
    QString musicAlbum = propertyMap["ALBUM"].toString().toCString(true);
    if(filterTextCode(musicAlbum).isEmpty())
        musicAlbum = "未知专辑";
    TagLib::AudioProperties *properties = f.audioProperties();
    if(properties == nullptr)
    {
        QMessageBox::about(this,"提示信息","添加文件失败");
        return songFiles;
    }

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
    SongItem *songitem = new SongItem;
    musicInfoWidget->setItemWidget(listItem,songitem);
    songitem->song_singer_albumText(musicName,musicSinger,musicAlbum); //歌曲名称 歌手 专辑
    songitem->songTimeLabel->setText(musicTime); //时长
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
    if(ret == DB_OP_SUCC)
    {
        this->tableName = tableName;
        for(int i = 0;i < resList.size(); i++)
        {
            if(resList.at(i).filepath != "")
            {
                QListWidgetItem *item = new QListWidgetItem(this->musicInfoWidget);
                SongItem *songitem = new SongItem;
                this->musicInfoWidget->setItemWidget(item,songitem);
                this->localAllMusicid.append(resList.at(i).filepath);
                songitem->song_singer_albumText(resList.at(i).title,resList.at(i).singer,resList.at(i).album); //歌曲名称 歌手 专辑
                songitem->songTimeLabel->setText(resList.at(i).time); //时长
                this->PlayList->addMedia(QUrl::fromLocalFile(resList.at(i).filepath));
            }
        }
    }
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
