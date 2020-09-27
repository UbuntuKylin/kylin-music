#include "mainwid.h"


MainWid::MainWid(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(960,640);
    setFocus();
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);     //窗体透明
    this->setWindowIcon(QIcon(":/img/kylin-music.png"));

    //    this->setStyleSheet("background-color:#FFFFFF;");
    myTitleBar = new TitleBar(this);
    mySideBar = new SideBar(this);
    //    myChangeListWid = new ChangeListWid(this);
    //    myMusicListWid = new MusicListWid(this);

    myPlaySongArea = new PlaySongArea(this);


    QHBoxLayout *mainlayout = new QHBoxLayout(this);

    QVBoxLayout *leftlayout = new QVBoxLayout(this);
    QVBoxLayout *rightlayout = new QVBoxLayout(this);




    leftlayout->addWidget(mySideBar);

    rightlayout->addWidget(myTitleBar, 0, Qt::AlignTop);
    //    rightlayout->addWidget(myChangeListWid,Qt::AlignTop);
    rightlayout->addWidget(mySideBar->rightChangeWid,Qt::AlignTop);

    hSlider = new QSlider(this);
    hSlider->setOrientation(Qt::Horizontal);
    hSlider->setStyleSheet("QSlider::groove:horizontal{height: 2px;background:#FF4848;}\
                           QSlider::add-page:horizontal{background:#ECEEF5;}\
                           QSlider::handle:horizontal{width: 14px;margin: -5 0 -5 0;border-image:url(:/img/default/point.png);}");

    rightlayout->addWidget(hSlider);
    rightlayout->addWidget(myPlaySongArea,0,Qt::AlignBottom);

    rightlayout->setMargin(0);
    rightlayout->setSpacing(0);

    QWidget *rightWid = new QWidget(this);
    rightWid->setObjectName("rightWid");
    rightWid->setLayout(rightlayout);
    rightWid->setStyleSheet("#rightWid{background:#FFFFFF;border-top-right-radius:12px;border-bottom-right-radius:12px;}");

    mainlayout->addLayout(leftlayout);
    mainlayout->addWidget(rightWid);

    mainlayout->setMargin(0);
    mainlayout->setSpacing(0);

    mainWidget = new QWidget(this);
    mainWidget->setLayout(mainlayout);

    mainWidget->setObjectName("mainWidget");
    mainWidget->setStyleSheet("#mianWidget{border-top-left-radius:12px;border-bottom-left-radius:12px;\
                              background:#FFFFFF;}");


    this->setCentralWidget(mainWidget);

    model=new QSqlTableModel(this);
    model->setTable("LocalMusic");
    model->select();
    connect(myPlaySongArea->playBtn,SIGNAL(clicked(bool)),this,SLOT(play_Song()));   //播放歌曲
    connect(mySideBar->myMusicListWid->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_listWidget_doubleClicked(QListWidgetItem*)));
    connect(mySideBar->musicListChangeWid[0]->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_listWidget1_doubleClicked(QListWidgetItem*)));

    connect(myPlaySongArea->lastBtn,SIGNAL(clicked()),this,SLOT(on_lastBtn_clicked())); //上一首
    connect(myPlaySongArea->nextBtn,SIGNAL(clicked()),this,SLOT(on_nextBtn_clicked())); //下一首
    connect(mySideBar->myMusicListWid->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::updatalistwidget);
    connect(mySideBar->musicListChangeWid[0]->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::updatalistwidget);

    connect(mySideBar->myMusicListWid->Music,SIGNAL(positionChanged(qint64)),this,SLOT(positionChange(qint64)));  //滑块进度条位置改变
    connect(mySideBar->musicListChangeWid[0]->Music,SIGNAL(positionChanged(qint64)),this,SLOT(positionChange(qint64)));  //滑块进度条位置改变

    connect(hSlider,SIGNAL(sliderPressed()),this,SLOT(slidePress()));
    connect(hSlider,SIGNAL(sliderMoved(int)),this,SLOT(slideMove(int)));
    connect(hSlider,SIGNAL(sliderReleased()),this,SLOT(slideRelease()));

    connect(mySideBar->myMusicListWid->Music,&QMediaPlayer::stateChanged,this,&MainWid::Music_stateChang);
    connect(mySideBar->musicListChangeWid[0]->Music,&QMediaPlayer::stateChanged,this,&MainWid::Music_stateChang);

    //    connect(myPlaySongArea->playModeBtn,SIGNAL(clicked(bool)),this,SLOT(playmode()));
    connect(myPlaySongArea->playModeBtn,SIGNAL(clicked(bool)),this,SLOT(PlayModeChanged()));

//    mySideBar->myMusicListWid->Music->play();
//    mySideBar->myMusicListWid->musicInfoWidget->item(0)->setSelected(true);
//    mySideBar->myMusicListWid->Music->pause();
}

MainWid::~MainWid()
{

}

void MainWid::play_Song()
{
    // 如果有歌曲在播放
    if (mySideBar->myMusicListWid->Music->state()        == QMediaPlayer::PlayingState ||
            mySideBar->musicListChangeWid[0]->Music->state() == QMediaPlayer::PlayingState)
    {
        qDebug()<<"暂停播放";
        myPlaySongArea->playBtn->setFixedSize(36,36);
        myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
        myPlaySongArea->playBtn->setToolTip(" 暂停");
        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
        myPlaySongArea->playBtn->setIconSize(QSize(36,36));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");

        // 根据当前播放的列表进行暂停
        if (mySideBar->currentPlayList == 0)
        {
            mySideBar->myMusicListWid->Music->pause();
        }
        else if (mySideBar->currentPlayList == 1)
        {
            mySideBar->musicListChangeWid[0]->Music->pause();
        }

        return ;
    }

    // 没有歌曲在播放
    else
    {
        myPlaySongArea->playBtn->setFixedSize(36,36);
        myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
        myPlaySongArea->playBtn->setToolTip(" 播放");
        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
        myPlaySongArea->playBtn->setIconSize(QSize(36,36));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");

        // 播放歌曲列表的歌曲
        if (mySideBar->currentPlayList == 0)
        {
            // 播放歌曲，并设置当前播放列表索引
            qDebug() << "播放歌曲列表";
            mySideBar->musicListChangeWid[0]->Music->stop();
            mySideBar->myMusicListWid->Music->play();
            mySideBar->currentPlayList = 0;
        }

        // 播放我喜欢的歌曲
        else if (mySideBar->currentPlayList == 1)
        {
            // 播放歌曲，并设置当前播放列表索引
            qDebug() << "播放我喜欢";
            mySideBar->myMusicListWid->Music->stop();
            mySideBar->musicListChangeWid[0]->Music->play();
            mySideBar->currentPlayList = 1;
        }
    }

    //    if( mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PlayingState)
    //    {

    //        qDebug()<<"播放 播放 myMusicListWid";
    //        myPlaySongArea->playBtn->setFixedSize(36,36);
    //        myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
    //        myPlaySongArea->playBtn->setToolTip(" 暂停");
    //        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
    //        myPlaySongArea->playBtn->setIconSize(QSize(36,36));
    //        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");
    //        mySideBar->myMusicListWid->Music->pause();
    //        return ;

    //    }
    //    if( mySideBar->musicListChangeWid[0]->Music->state() == QMediaPlayer::PlayingState)
    //    {
    //        qDebug()<<"播放 播放";
    //        myPlaySongArea->playBtn->setFixedSize(36,36);
    //        myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
    //        myPlaySongArea->playBtn->setToolTip(" 暂停");
    //        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
    //        myPlaySongArea->playBtn->setIconSize(QSize(36,36));
    //        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");
    //        mySideBar->musicListChangeWid[0]->Music->pause();
    //        return ;
    //    }
    //    if (mySideBar->currentSelectList == 0) {
    //        qDebug()<<"(mySideBar->currentSelectList 0";

    //        if( mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PausedState)
    //        {
    //            mySideBar->musicListChangeWid[0]->Music->stop();
    //            qDebug()<<"暂停 暂停";
    //            myPlaySongArea->playBtn->setFixedSize(36,36);
    //            myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
    //            myPlaySongArea->playBtn->setToolTip(" 播放");
    //            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
    //            myPlaySongArea->playBtn->setIconSize(QSize(36,36));
    //            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");
    //            mySideBar->myMusicListWid->Music->play();
    //        }
    //        else if( mySideBar->musicListChangeWid[0]->Music->state() == QMediaPlayer::PausedState)
    //        {
    //            mySideBar->myMusicListWid->Music->stop();

    //            qDebug()<<"暂停 暂停";
    //            myPlaySongArea->playBtn->setFixedSize(36,36);
    //            myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
    //            myPlaySongArea->playBtn->setToolTip(" 播放");
    //            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
    //            myPlaySongArea->playBtn->setIconSize(QSize(36,36));
    //            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");
    //            mySideBar->musicListChangeWid[0]->Music->play();
    //        }

    //    }
    //    else if (mySideBar->currentSelectList == 1) {
    //        qDebug()<<"(mySideBar->currentSelectList 1";

    //        if( mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PausedState)
    //        {
    //            mySideBar->musicListChangeWid[0]->Music->stop();
    //            qDebug()<<"暂停 暂停";
    //            myPlaySongArea->playBtn->setFixedSize(36,36);
    //            myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
    //            myPlaySongArea->playBtn->setToolTip(" 播放");
    //            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
    //            myPlaySongArea->playBtn->setIconSize(QSize(36,36));
    //            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");
    //            mySideBar->myMusicListWid->Music->play();
    //        }
    //        else if( mySideBar->musicListChangeWid[0]->Music->state() == QMediaPlayer::PausedState)
    //        {
    //            mySideBar->myMusicListWid->Music->stop();

    //            qDebug()<<"暂停 暂停";
    //            myPlaySongArea->playBtn->setFixedSize(36,36);
    //            myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
    //            myPlaySongArea->playBtn->setToolTip(" 播放");
    //            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
    //            myPlaySongArea->playBtn->setIconSize(QSize(36,36));
    //            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px}");
    //            mySideBar->musicListChangeWid[0]->Music->play();
    //        }
    //    }
}

void MainWid::contextMenuEvent(QContextMenuEvent *event)     //歌曲列表右键菜单
{
    QMenu *Menu = new QMenu(this);
    Action1 = new QAction(this);
    Action2 = new QAction(this);
    Action3 = new QAction(this);
    Action4 = new QAction(this);
    //    Action5 = new QAction(this);
    //    Action6 = new QAction(this);
    Action7 = new QAction(this);
    //    Action1->setIcon(QIcon(":/image/image/bofang1.png"));
    Action1->setText(play_pause);
    //    Action2->setIcon(QIcon(":/image/image/like_outline_32px_1170275_easyicon.net.png"));
    Action2->setText("下一首");
    //    Action4->setIcon(QIcon(":/image/image/list_32px_1142913_easyicon.net.png"));
    Action3->setText("添加到我喜欢");
    //    Action3->setIcon(QIcon(":/image/image/delete_32px_1168909_easyicon.net.png"));
    Action4->setText("从歌单中删除");
    //    Action5->setIcon(QIcon(":/image/image/inf.png"));
    //    Action5->setText("从本地删除");
    //    Action6->setText("查看本地文件");
    Action7->setText("歌曲信息");

    //    Menu->addAction(Action1);
    //    Menu->addAction(Action2);
    Menu->addAction(Action3);
    Menu->addAction(Action4);
    //    Menu->addAction(Action5);
    //    Menu->addAction(Action6);
    Menu->addAction(Action7);
    connect(Action1,&QAction::triggered,this,&MainWid::Action1_slot);
    connect(Action2,&QAction::triggered,this,&MainWid::Action2_slot);
    connect(Action3,&QAction::triggered,this,&MainWid::Action3_slot);
    connect(Action4,&QAction::triggered,this,&MainWid::Action4_slot);
    //    connect(Action5,&QAction::triggered,this,&musicwid::Action5_slot);
    connect(Action7,&QAction::triggered,this,&MainWid::Action7_slot);
    Menu->exec(QCursor::pos());
    delete Menu;
    delete Action1;
    delete Action2;
    //    delete Action3;
    delete Action4;
    //    delete Action5;
    //    delete Action6;
    delete Action7;
}

// 播放暂停功能
void MainWid::Action1_slot()
{
    if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
    {
        if(mySideBar->myMusicListWid->musicInfoWidget->item(mySideBar->myMusicListWid->PlayList->currentIndex())->isSelected())
        {

            if(mySideBar->myMusicListWid->Music->state()==QMediaPlayer::PlayingState)
            {
                qDebug()<<"播放";
                play_pause="播放";
                Action1->setText(play_pause);
                mySideBar->myMusicListWid->Music->pause();
            }
            else
            {
                qDebug()<<"暂停";
                play_pause="暂停";
                Action1->setText(play_pause);
                mySideBar->myMusicListWid->Music->play();
            }
        }
        else
        {
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row());
            qDebug()<<"666666666666";
            play_pause="播放";
            Action1->setText(play_pause);
            mySideBar->myMusicListWid->Music->play();

        }
    }
    else
    {
        mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
        mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row());
        qDebug()<<"777777777777";
        play_pause="播放";
        Action1->setText(play_pause);
        mySideBar->myMusicListWid->Music->pause();
    }
}

// 下一首功能
void MainWid::Action2_slot()
{
    if (mySideBar->currentPlayList == 0) {
        mySideBar->musicListChangeWid[0]->Music->stop();

        if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
        {
            int currentIndex=mySideBar->myMusicListWid->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(++currentIndex==mySideBar->myMusicListWid->PlayList->mediaCount())
            {
                currentIndex=0;
            }
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
        }
        mySideBar->myMusicListWid->Music->play();
    }
    else if (mySideBar->currentPlayList == 1) {
        mySideBar->myMusicListWid->Music->stop();

        if(mySideBar->musicListChangeWid[0]->Music->playlist()==mySideBar->musicListChangeWid[0]->PlayList)
        {
            int currentIndex=mySideBar->musicListChangeWid[0]->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(++currentIndex==mySideBar->musicListChangeWid[0]->PlayList->mediaCount())
            {
                currentIndex=0;
            }
            mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(currentIndex);
        }
        mySideBar->musicListChangeWid[0]->Music->play();
    }
}

// 添加到我喜欢
void MainWid::Action3_slot()
{
    mySideBar->favoriteMusicBtn->click();

    model->setTable("LocalMusic");
    model->select();

    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
    QString FileName = model->data(model->index(row,2)).toString();

    MusicListWid *changewid = mySideBar->musicListChangeWid[0];

    if (FileName != "")
    {
        QString path = QDir::toNativeSeparators(FileName);
        if (!path.isEmpty())
        {
            bool songExists = false;
            QStringList sqlFilenameList = changewid->sqlFilenameList;

            for (QString& songPlaylist:sqlFilenameList)
            {
                if(changewid->getMp3FileName(path).remove(QString(".mp3")) == songPlaylist)
                {
                    qDebug()<<"歌曲已存在";
                    songExists = true;
                    return ;
                }
            }
        }

        changewid->PlayList->addMedia(QUrl::fromLocalFile(path));
        QString Name = path.split("/").last().remove(QString(".mp3"));            //截取歌曲文件的歌曲名
        QListWidgetItem *listItem = new QListWidgetItem(changewid->musicInfoWidget);
        listItem->setText(QString("%1").arg(Name));
        changewid->musicInfoWidget->addItem(listItem);

        QSqlQuery query;
        query.exec(QString("insert into FavoriteMusic values (%1,'%2','%3',%4)").arg(qrand()%1000000).arg(Name).arg(path).arg(0));

        changewid->sqlFilenameList.append(Name);
        changewid->count += 1;
        changewid->songNumberLabel->setText("共"+QString::number(changewid->count)+"首");
        qDebug() << changewid->sqlFilenameList;
    }
}

// 从歌单里删除
void MainWid::Action4_slot()
{
    if (mySideBar->currentSelectList == 0) {
        if(mySideBar->myMusicListWid->Music->playlist() == mySideBar->myMusicListWid->PlayList)
        {
            int Row=mySideBar->myMusicListWid->PlayList->currentIndex();
            int row=mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
            if(row<Row)
            {
                int Position;
                if(mySideBar->myMusicListWid->Music->state()==QMediaPlayer::PlayingState)
                {
                    Position=mySideBar->myMusicListWid->Music->position();
                }
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
                mySideBar->myMusicListWid->musicInfoWidget->takeItem(row);
                mySideBar->myMusicListWid->PlayList->removeMedia(row,row);
                model->setTable("LocalMusic");
                model->select();
                model->removeRow(row);
                model->submitAll();
                mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(Row-1);
                mySideBar->myMusicListWid->Music->setPosition(Position);
                //            hSlider->setValue(Position);
                mySideBar->myMusicListWid->Music->play();
                mySideBar->myMusicListWid->sqlFilenameList.removeAt(row);
                mySideBar->myMusicListWid->count -= 1;
            }
            else
            {
                mySideBar->myMusicListWid->musicInfoWidget->takeItem(row);
                mySideBar->myMusicListWid->PlayList->removeMedia(row,row);
                model->setTable("LocalMusic");
                model->select();
                model->removeRow(row);
                model->submitAll();
                mySideBar->myMusicListWid->sqlFilenameList.removeAt(row);
                mySideBar->myMusicListWid->count -= 1;
            }
        }
        else
        {
            int row=mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
            mySideBar->myMusicListWid->musicInfoWidget->takeItem(row);
            mySideBar->myMusicListWid->PlayList->removeMedia(row,row);
            model->setTable("LocalMusic");
            model->select();
            model->removeRow(row);
            model->submitAll();
            mySideBar->myMusicListWid->sqlFilenameList.removeAt(row);
            mySideBar->myMusicListWid->count -= 1;
        }
        mySideBar->myMusicListWid->songNumberLabel->setText("共"+QString::number(mySideBar->myMusicListWid->count)+"首");
    }
    else if (mySideBar->currentSelectList == 1) {
        if(mySideBar->musicListChangeWid[0]->Music->playlist() == mySideBar->musicListChangeWid[0]->PlayList)
        {
            int Row=mySideBar->musicListChangeWid[0]->PlayList->currentIndex();
            int row=mySideBar->musicListChangeWid[0]->musicInfoWidget->currentIndex().row();
            if(row<Row)
            {
                int Position;
                if(mySideBar->musicListChangeWid[0]->Music->state()==QMediaPlayer::PlayingState)
                {
                    Position=mySideBar->musicListChangeWid[0]->Music->position();
                }
                mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(0);
                mySideBar->musicListChangeWid[0]->musicInfoWidget->takeItem(row);
                mySideBar->musicListChangeWid[0]->PlayList->removeMedia(row,row);
                model->setTable("FavoriteMusic");
                model->select();
                model->removeRow(row);
                model->submitAll();
                mySideBar->musicListChangeWid[0]->Music->setPlaylist(mySideBar->musicListChangeWid[0]->PlayList);
                mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(Row-1);
                mySideBar->musicListChangeWid[0]->Music->setPosition(Position);
                //            hSlider->setValue(Position);
                mySideBar->musicListChangeWid[0]->Music->play();
                mySideBar->musicListChangeWid[0]->sqlFilenameList.removeAt(row);
                mySideBar->musicListChangeWid[0]->count -= 1;
            }
            else
            {
                mySideBar->musicListChangeWid[0]->musicInfoWidget->takeItem(row);
                mySideBar->musicListChangeWid[0]->PlayList->removeMedia(row,row);
                model->setTable("FavoriteMusic");
                model->select();
                model->removeRow(row);
                model->submitAll();
                mySideBar->musicListChangeWid[0]->sqlFilenameList.removeAt(row);
                mySideBar->musicListChangeWid[0]->count -= 1;
            }
        }
        else
        {
            int row=mySideBar->musicListChangeWid[0]->musicInfoWidget->currentIndex().row();
            mySideBar->musicListChangeWid[0]->musicInfoWidget->takeItem(row);
            mySideBar->musicListChangeWid[0]->PlayList->removeMedia(row,row);
            model->setTable("FavoriteMusic");
            model->select();
            model->removeRow(row);
            model->submitAll();
            mySideBar->musicListChangeWid[0]->sqlFilenameList.removeAt(row);
            mySideBar->musicListChangeWid[0]->count -= 1;
        }
        mySideBar->musicListChangeWid[0]->songNumberLabel->setText("共"+QString::number(mySideBar->musicListChangeWid[0]->count)+"首");
        qDebug() << mySideBar->musicListChangeWid[0]->sqlFilenameList;
    }
}

void MainWid::Action7_slot()
{
    if (mySideBar->currentSelectList == 0) {
        int row=mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
        QString temp=mySideBar->myMusicListWid->musicInfoWidget->currentIndex().data().toString();
        QString MusicName=temp.split(" - ").last();
        QString Author=temp.remove(" - "+MusicName);
        QString Album = mySideBar->myMusicListWid->Music->metaData(QMediaMetaData::AlbumTitle).toString();
        QString FileName=model->data(model->index(row,2)).toString();
        QString type = FileName.split(".").last();
        QString size = mySideBar->myMusicListWid->Music->metaData(QMediaMetaData::Size).toString();
        qDebug()<<mySideBar->myMusicListWid->Music->metaData(QMediaMetaData::Size).toString();
        QString time=myPlaySongArea->bottomLeftLabel->text().split("/").last();
        model->setTable("LocalMusic");
        model->select();
        QStringList list;
        list << MusicName << Author << Album
             << type << time
             << FileName;

        //        for(int i = 0;i < list.length();++i)
        //        {

        //        }
        QMessageBox::about(this,tr("歌曲信息"),tr("歌曲名称: %1 \n"
                                              "歌    手: %2 \n"
                                              "专    辑: %3 \n"
                                              "文件类型: %4 \n"
                                              "文件大小: %5 \n"
                                              "文件时长: %6 \n"
                                              "文件位置: %7 \n").arg(MusicName).arg(Author).arg(Album).arg(type).arg(size).arg(time).arg(FileName));
    }
    else if (mySideBar->currentSelectList == 1) {
        int row=mySideBar->musicListChangeWid[0]->musicInfoWidget->currentIndex().row();
        QString temp=mySideBar->musicListChangeWid[0]->musicInfoWidget->currentIndex().data().toString();
        QString MusicName=temp.split(" - ").last();
        QString Author=temp.remove(" - "+MusicName);
        QString Album = mySideBar->musicListChangeWid[0]->Music->metaData(QMediaMetaData::AlbumTitle).toString();
        QString FileName=model->data(model->index(row,2)).toString();
        QString type = FileName.split(".").last();
        QString size = mySideBar->musicListChangeWid[0]->Music->metaData(QMediaMetaData::Size).toString();
        qDebug()<<mySideBar->musicListChangeWid[0]->Music->metaData(QMediaMetaData::Size).toString();
        QString time=myPlaySongArea->bottomLeftLabel->text().split("/").last();
        model->setTable("LocalMusic");
        model->select();
        QStringList list;
        list << MusicName << Author << Album
             << type << time
             << FileName;

        //        for(int i = 0;i < list.length();++i)
        //        {

        //        }
        QMessageBox::about(this,tr("歌曲信息"),tr("歌曲名称: %1 \n"
                                              "歌    手: %2 \n"
                                              "专    辑: %3 \n"
                                              "文件类型: %4 \n"
                                              "文件大小: %5 \n"
                                              "文件时长: %6 \n"
                                              "文件位置: %7 \n").arg(MusicName).arg(Author).arg(Album).arg(type).arg(size).arg(time).arg(FileName));
    }

}

void MainWid::updatalistwidget(int value)//更新listWidget
{
    if (mySideBar->currentPlayList == 0) {
        mySideBar->myMusicListWid->musicInfoWidget->item(value)->setSelected(true);
        qDebug()<<"update";
    }
    else if (mySideBar->currentPlayList == 1) {
        mySideBar->musicListChangeWid[0]->musicInfoWidget->item(value)->setSelected(true);
        qDebug()<<"update";
    }
}

void MainWid::updateSongPlaying()
{
    if (mySideBar->currentSelectList == 0) {

        //        int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
        //        QString MusicName = model->data(model->index(row,2)).toString().split("/").last().remove(QString(".mp3"));

        //        mySideBar->myMusicListWid->Music->

        int row=mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
        QString temp=mySideBar->myMusicListWid->musicInfoWidget->currentIndex().data().toString();
        QString MusicName=temp.split(" - ").last();

        myPlaySongArea->songNameofNowPlaying->setText("正在播放：" + MusicName);
    }
    else if (mySideBar->currentSelectList == 1) {
        int row = mySideBar->musicListChangeWid[0]->musicInfoWidget->currentIndex().row();
        QString MusicName = model->data(model->index(row,2)).toString().split("/").last().remove(QString(".mp3"));

        myPlaySongArea->songNameofNowPlaying->setText("正在播放：" + MusicName);
    }
}

void MainWid::on_listWidget_doubleClicked(QListWidgetItem *item)//双击播放playlist
{
    mySideBar->musicListChangeWid[0]->Music->stop();
    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
    mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row());
    //    mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->row(item));
    mySideBar->myMusicListWid->Music->play();
    mySideBar->currentPlayList = 0;
}

void MainWid::on_listWidget1_doubleClicked(QListWidgetItem *item)
{
    mySideBar->myMusicListWid->Music->stop();
    mySideBar->musicListChangeWid[0]->Music->setPlaylist(mySideBar->musicListChangeWid[0]->PlayList);
    mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(mySideBar->musicListChangeWid[0]->musicInfoWidget->currentIndex().row());
    //    mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->row(item));
    mySideBar->musicListChangeWid[0]->Music->play();
    mySideBar->currentPlayList = 1;

    //    QString temp=mySideBar->musicListChangeWid[0]->musicInfoWidget->currentIndex().data().toString();
    //    QString MusicName=temp.split(" - ").last();

    //    myPlaySongArea->songNameofNowPlaying->setText("正在播放：" + MusicName);

}

void MainWid::Music_stateChang(QMediaPlayer::State state)//播放状态改变
{

    if (mySideBar->currentPlayList == 0) {
        if(state==QMediaPlayer::PlayingState)
        {
            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
            myPlaySongArea->playBtn->setToolTip("播放");

        }
        else
        {
            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
            myPlaySongArea->playBtn->setToolTip("暂停");

        }
    }
    else if (mySideBar->currentPlayList == 1) {
        if(state==QMediaPlayer::PlayingState)
        {
            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
            myPlaySongArea->playBtn->setToolTip("播放");


        }
        else
        {
            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
            myPlaySongArea->playBtn->setToolTip("暂停");

        }
    }
    //    updateSongPlaying();
}

void MainWid::on_lastBtn_clicked()             //上一首
{
    if (mySideBar->currentPlayList == 0) {
        mySideBar->musicListChangeWid[0]->Music->stop();

        if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
        {
            int currentIndex=mySideBar->myMusicListWid->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(--currentIndex < 0)
            {
                currentIndex = 0;
            }
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
        }
        mySideBar->myMusicListWid->Music->play();
    }
    else if (mySideBar->currentPlayList == 1) {
        mySideBar->myMusicListWid->Music->stop();

        if(mySideBar->musicListChangeWid[0]->Music->playlist()==mySideBar->musicListChangeWid[0]->PlayList)
        {
            int currentIndex=mySideBar->musicListChangeWid[0]->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(--currentIndex < 0)
            {
                currentIndex = 0;
            }
            mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(currentIndex);
        }
        mySideBar->musicListChangeWid[0]->Music->play();
    }
    //    updateSongPlaying();
}

void MainWid::on_nextBtn_clicked()      //下一首
{
    if (mySideBar->currentPlayList == 0) {
        mySideBar->musicListChangeWid[0]->Music->stop();

        if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
        {

            int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(++currentIndex == mySideBar->myMusicListWid->PlayList->mediaCount())
            {
                currentIndex = 0;
            }
            qDebug() << "mySideBar->myMusicListWid " << currentIndex;
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
            qDebug() << "mySideBar->myMusicListWid";
        }
        mySideBar->myMusicListWid->Music->play();
    }
    else if (mySideBar->currentPlayList == 1) {
//        mySideBar->myMusicListWid->Music->stop();

        if(mySideBar->musicListChangeWid[0]->Music->playlist()==mySideBar->musicListChangeWid[0]->PlayList)
        {
            int currentIndex=mySideBar->musicListChangeWid[0]->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(++currentIndex==mySideBar->musicListChangeWid[0]->PlayList->mediaCount())
            {
                currentIndex=0;
            }
            mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(currentIndex);
        }
        mySideBar->musicListChangeWid[0]->Music->play();
    }
    //    updateSongPlaying();
}


void MainWid::positionChange(qint64 position)
{
    if (mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PlayingState) {
        hSlider->setMaximum(mySideBar->myMusicListWid->Music->duration() / 1000);//设置滑块的长度范围为音乐长.00度
        hSlider->setValue(position / 1000);//如果音乐进度改变，则改变滑块显示位置
        moved = position;//让播放时间等于音乐进度（毫秒）
        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
        QString str_time = moveTime.toString("mm:ss");
        QTime totalTime(0,(mySideBar->myMusicListWid->Music->duration()/60000) % 60,(mySideBar->myMusicListWid->Music->duration() / 1000) % 60);
        myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

        // 判断顺序播放模式下最后一首歌，播放完停止播放
        if (mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Sequential) {

            if (mySideBar->myMusicListWid->PlayList->currentIndex() == mySideBar->myMusicListWid->PlayList->mediaCount() - 1) {

                if (mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PlayingState) {
                    qDebug()<<"列表播放结束 停止  " << moved << "  " << mySideBar->myMusicListWid->Music->duration();
                    if (mySideBar->myMusicListWid->Music->duration() > 0) {
                        if (moved > mySideBar->myMusicListWid->Music->duration() - 1000) {
                            qDebug()<<"列表播放结束 停止" ;
                            mySideBar->myMusicListWid->Music->stop();
                            moved = 0;
                        }
                    }

                }
            }
        }
    }
    else if (mySideBar->musicListChangeWid[0]->Music->state() == QMediaPlayer::PlayingState) {
        hSlider->setMaximum(mySideBar->musicListChangeWid[0]->Music->duration() / 1000);//设置滑块的长度范围为音乐长.00度
        hSlider->setValue(position / 1000);//如果音乐进度改变，则改变滑块显示位置
        moved = position;//让播放时间等于音乐进度（毫秒）
        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
        QString str_time = moveTime.toString("mm:ss");
        QTime totalTime(0,(mySideBar->musicListChangeWid[0]->Music->duration()/60000) % 60,(mySideBar->musicListChangeWid[0]->Music->duration() / 1000) % 60);
        myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

        // 判断顺序播放模式下最后一首歌，播放完停止播放
        if (mySideBar->musicListChangeWid[0]->PlayList->playbackMode() == QMediaPlaylist::Sequential) {

            if (mySideBar->musicListChangeWid[0]->PlayList->currentIndex() == mySideBar->musicListChangeWid[0]->PlayList->mediaCount() - 1) {

                if (mySideBar->musicListChangeWid[0]->Music->state() == QMediaPlayer::PlayingState) {
                    qDebug()<<"列表播放结束 停止  " << moved << "  " << mySideBar->musicListChangeWid[0]->Music->duration();
                    if (mySideBar->musicListChangeWid[0]->Music->duration() > 0) {
                        if (moved > mySideBar->musicListChangeWid[0]->Music->duration() - 1000) {
                            qDebug()<<"列表播放结束 停止" ;
                            mySideBar->musicListChangeWid[0]->Music->stop();
                            moved = 0;
                        }
                    }

                }
            }
        }
    }
}

void MainWid::slidePress() //滑动条鼠标按下
{
    if (mySideBar->currentPlayList == 0) {
        mySideBar->myMusicListWid->Music->pause();
    }
    else if (mySideBar->currentPlayList == 1) {
        mySideBar->musicListChangeWid[0]->Music->pause();
    }
}

void MainWid::slideMove(int position)  //播放歌曲滑块移动
{


    //    qDebug() << "usicwid::slideMove(int position)" << position;
    //    qDebug() << "moved  " << moved << "        " << Music->duration() / 1000;
    //    hSlider->setMaximum(Music->duration() / 1000);//设置滑块的长度范围为音乐长.00度

    if (mySideBar->currentPlayList == 0) {

        if (position == 0)
            return ;

        moved = position * 1000;
        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
        QString str_time = moveTime.toString("mm:ss");
        QTime totalTime(0,(mySideBar->myMusicListWid->Music->duration()/60000) % 60,(mySideBar->myMusicListWid->Music->duration() / 1000) % 60);


        if (moved <= mySideBar->myMusicListWid->Music->duration()) {
            hSlider->setValue(moved / 1000);//如果音乐进度改变，则改变滑块显示位置
            myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间
            mySideBar->myMusicListWid->Music->setPosition(moved);
        }
    }
    else if (mySideBar->currentPlayList == 1) {

        if (position == 0)
            return ;

        moved = position * 1000;
        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
        QString str_time = moveTime.toString("mm:ss");
        QTime totalTime(0,(mySideBar->musicListChangeWid[0]->Music->duration()/60000) % 60,(mySideBar->musicListChangeWid[0]->Music->duration() / 1000) % 60);


        if (moved <= mySideBar->musicListChangeWid[0]->Music->duration()) {
            hSlider->setValue(moved / 1000);//如果音乐进度改变，则改变滑块显示位置
            myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间
            mySideBar->musicListChangeWid[0]->Music->setPosition(moved);
        }
    }
}

void MainWid::slideRelease()   //滑动条鼠标弹起
{
    if (mySideBar->currentPlayList == 0)
        mySideBar->myMusicListWid->Music->play();
    else if (mySideBar->currentPlayList == 1)
        mySideBar->musicListChangeWid[0]->Music->play();
}

void MainWid::PlayModeChanged()
{
    if (mySideBar->currentPlayList == 0) {
        if(mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Loop)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/random.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/random.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 随机播放 ");

            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::Random);
            qDebug()<<"随机播放";
        }
        else if(mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Random)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/sequence.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 顺序播放 ");

            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
            qDebug()<<"顺序播放";
        }
        else if(mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Sequential)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/oneloop.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/oneloop.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 单曲循环 ");

            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
            qDebug()<<"单曲循环";
        }
        else if(mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/listloop.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/listloop.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 列表循环 ");

            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::Loop);
            qDebug()<<"列表循环";
        }
    }
    else if (mySideBar->currentPlayList == 1) {
        if(mySideBar->musicListChangeWid[0]->PlayList->playbackMode() == QMediaPlaylist::Loop)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/random.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/random.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 随机播放 ");

            mySideBar->musicListChangeWid[0]->PlayList->setPlaybackMode(QMediaPlaylist::Random);
            qDebug()<<"随机播放";
        }
        else if(mySideBar->musicListChangeWid[0]->PlayList->playbackMode() == QMediaPlaylist::Random)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/sequence.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 顺序播放 ");

            mySideBar->musicListChangeWid[0]->PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
            qDebug()<<"顺序播放";
        }
        else if(mySideBar->musicListChangeWid[0]->PlayList->playbackMode() == QMediaPlaylist::Sequential)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/oneloop.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/oneloop.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 单曲循环 ");

            mySideBar->musicListChangeWid[0]->PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
            qDebug()<<"单曲循环";
        }
        else if(mySideBar->musicListChangeWid[0]->PlayList->playbackMode() == QMediaPlaylist::CurrentItemInLoop)
        {
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;border-image:url(:/img/default/listloop.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/listloop.png);}");
            myPlaySongArea->playModeBtn->setToolTip(" 列表循环 ");

            mySideBar->musicListChangeWid[0]->PlayList->setPlaybackMode(QMediaPlaylist::Loop);
            qDebug()<<"列表循环";
        }
    }
}
