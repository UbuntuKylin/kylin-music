#include "changelistwid.h"

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

    hTitleLayout->addWidget(songLabel);
    hTitleLayout->addWidget(singerLabel);
    hTitleLayout->addWidget(albumLabel);
    hTitleLayout->addWidget(timeLabel);

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
        QString Name=query.value(1).toString();
        QString FileName=query.value(2).toString();
        QString mp3Name=getMp3FileName(Name);

        sqlFilenameList.append(mp3Name);

        if(Name!=""&&FileName!="")
        {
            QListWidgetItem *item1=new QListWidgetItem;
//            item1->setIcon(QIcon(":/image/image/Music_32px_1144946_easyicon.net.png"));
//            item1->setSizeHint(QSize(700,40));
            item1->setText(mp3Name);
            musicInfoWidget->addItem(item1);
            PlayList->addMedia(QUrl::fromLocalFile(FileName));
        }
    }
    count = sqlFilenameList.size();
    songNumberLabel->setText("共"+QString::number(count)+"首");
    qDebug()<<"sqlFilenameList.size() :"<<sqlFilenameList.size();
    Music->setPlaylist(PlayList);
}


void MusicListWid::on_top_addSongBtn_slot()
{
    QSqlQuery query;
    query.exec("select * from LocalMusic");
    //    QFileInfo info;
    QStringList songFiles = QFileDialog::getOpenFileNames(this, "打开文件","Music","music(*.mp3)");  //歌曲文件
    if(!songFiles.isEmpty())
    {
        QProgressDialog *progressdialog=new QProgressDialog(this);
        progressdialog->setWindowModality(Qt::WindowModal);
        progressdialog->setMinimumDuration(100);
        progressdialog->resize(500,200);
        progressdialog->setWindowTitle(tr("添加歌曲"));
        progressdialog->setLabelText(tr("添加中                              ."));
        progressdialog->setCancelButtonText(tr("关闭"));
        progressdialog->setRange(0,songFiles.size()-1);

        qDebug() << "songFiles.size()" << songFiles.size();

        for(int i = 0; i < songFiles.size(); i++)
        {
            QString path=QDir::toNativeSeparators(songFiles.at(i));
            progressdialog->setValue(i);
            if(i==songFiles.size()-1)
            {
                progressdialog->close();
            }
            if(progressdialog->wasCanceled())
            {
                progressdialog->close();
            }
            if(!path.isEmpty())
            {
                bool songExists = false;
                qDebug() << "sqlFilenameList" << sqlFilenameList;
                for (QString& songPlaylist:sqlFilenameList)
                {
                    if(getMp3FileName(path).remove(QString(".mp3")) == songPlaylist)
                    {
                        qDebug()<<"歌曲已存在";
                        songExists = true;
                        break;
                    }
                }
                if(songExists)
                {
                    continue;
                }
            }
            qDebug() << "sqlFilenameList" << sqlFilenameList;
            PlayList->addMedia(QUrl::fromLocalFile(path));
            QString Name = path.split("/").last().remove(QString(".mp3"));            //截取歌曲文件的歌曲名
            QListWidgetItem *listItem = new QListWidgetItem(musicInfoWidget);
            listItem->setText(QString("%1").arg(Name));
            musicInfoWidget->addItem(listItem);
            query.exec(QString("insert into LocalMusic values (%1,'%2','%3',%4)").arg(qrand()%1000000).arg(Name).arg(path).arg(0));
            qDebug()<<query.exec("select * from LocalMusic");

            sqlFilenameList.append(Name);

            qDebug() << "sqlFilenameList" << sqlFilenameList;
            count += 1;
            songNumberLabel->setText("共"+QString::number(count)+"首");
        }
    }
    Music->setPlaylist(PlayList);
//    Music->play();
}

QString MusicListWid::getMp3FileName(QString sqlName)
{
    QStringList sqlList=sqlName.split('/');

    int length=sqlList.length();

    QString songname=sqlList.value(length-1);
    return songname;
}

