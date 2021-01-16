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

#include "mainwid.h"
#include "songitem.h"
#include "miniwidget.h"
#include "widgetstyle.h"
#include "myapplication.h"
#include "xatom-helper.h"

MainWid::MainWid(QString str, QWidget *parent)
    : QMainWindow(parent)
{
    // 用户手册功能
    setAcceptDrops(true);
    mDaemonIpcDbus = new DaemonIpcDbus();

    gsettingInit();//初始化gsetting

    Single(str);   //单例

    dbusInit();//初始化dbus

    styleInit();//初始化控件及样式

    actionInit();//初始化事件

}

void MainWid::styleInit()
{
    //    promptMessage();  //提示信息
        setMinimumSize(960,640);
    //    setFocus();
    //    this->setWindowFlags(Qt::FramelessWindowHint);            //
        this->setAttribute(Qt::WA_TranslucentBackground, true);     //窗体透明
    //    this->setStyleSheet("border-radius:6px;" );//主窗体圆角(注意：窗体透明与主窗体圆角要搭配使用否则无效)

        //窗体显示在中间
        QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
        this->move((availableGeometry.width() - this->width())/2, (availableGeometry.height() - this->height())/2);

    //    this->setWindowTitle(tr("麒麟音乐"));
        this->setWindowTitle(tr("Kylin music"));
        this->setWindowIcon(QIcon(":/img/kylin-music.png"));

        //    this->setStyleSheet("background-color:#FFFFFF;");
        myTitleBar = new TitleBar(this);
        mySideBar = new SideBar(this);
        nullMusicWidget = new ChangeListWid(this);   //空页面

        //    myMusicListWid = new MusicListWid(this);

        myPlaySongArea = new PlaySongArea(this);


        QHBoxLayout *mainlayout = new QHBoxLayout(this);

        QVBoxLayout *leftlayout = new QVBoxLayout(this);
        rightlayout = new QVBoxLayout(this);

        model = new QSqlTableModel(this);
        model->setTable("LocalMusic");
        model->select();

        model_1 = new QSqlTableModel(this);
        model_1->setTable("NewPlayList");
        model_1->select();

        leftlayout->addWidget(mySideBar);

        rightlayout->addWidget(myTitleBar, 0, Qt::AlignTop);

        if(mySideBar->myMusicListWid->musicInfoWidget->count() == 0)
        {
            rightlayout->addWidget(nullMusicWidget,Qt::AlignTop);
            mySideBar->rightChangeWid->hide();
        }
        else
        {
            rightlayout->addWidget(mySideBar->rightChangeWid,Qt::AlignTop);
            nullMusicWidget->hide();
        }

    //    nullMusicWidget->hide();

    //    rightlayout->addWidget(mySideBar->rightChangeWid,Qt::AlignTop);
    //    mySideBar->rightChangeWid->hide();

        hSlider = new Slider(this);
        hSlider->installEventFilter(this);
        hSlider->setOrientation(Qt::Horizontal);
        hSlider->setStyleSheet("QSlider::groove:horizontal{height: 2px;background:#3790FA;}\
                                QSlider::add-page:horizontal{background:#ECEEF5;}\
                                QSlider::handle:horizontal{width: 14px;margin: -5 0 -5 0;border-image:url(:/img/default/point.png);}");


        rightlayout->addWidget(hSlider);
        rightlayout->addWidget(myPlaySongArea,0,Qt::AlignBottom);

        vSlider = new Slider;
        vSlider->installEventFilter(this);
        vSlider->setOrientation(Qt::Vertical);
    //    vSlider->setFixedSize(100,12);
    //    vSlider->setToolTip("音量调节");
        vSlider->setToolTip(tr("volume"));
        vSlider->setMinimum(0);
        vSlider->setMaximum(100);
        vSliderWid = new QWidget();
        vSliderWid->setFixedSize(30,90);
        vSliderWid->setWindowFlags(Qt::FramelessWindowHint);

        HLayout = new QHBoxLayout;

        HLayout->addWidget(vSlider);
        vSliderWid->setLayout(HLayout);
        vSliderWid->setStyleSheet("background-color:white;");

        connect(vSlider,&QSlider::valueChanged,this,&MainWid::changeVolume);
    //    connect(myPlaySongArea->volumeBtn,&QPushButton::clicked,this,&MainWid::show_volumeBtn);

        rightlayout->setMargin(0);
        rightlayout->setSpacing(0);

        rightWid = new QWidget(this);
        rightWid->setObjectName("rightWid");
        rightWid->setLayout(rightlayout);

        mainlayout->addLayout(leftlayout);
        mainlayout->addWidget(rightWid);

        mainlayout->setMargin(0);
        mainlayout->setSpacing(0);

        mainWidget = new QWidget(this);
        mainWidget->setLayout(mainlayout);

        mainWidget->setObjectName("mainWidget");

        m_MiniWidget = new miniWidget;
        MotifWmHints hints;
        hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
        hints.functions = MWM_FUNC_ALL;
        hints.decorations = MWM_DECOR_BORDER;
        XAtomHelper::getInstance()->setWindowMotifHint(m_MiniWidget->winId(), hints);

        if (WidgetStyle::themeColor == 1)
        {
            mainWidget->setStyleSheet("#mainWidget{background:#252526;}");
            rightWid->setStyleSheet("#rightWid{background:#252526;}");
        }
        else if(WidgetStyle::themeColor == 0)
        {
            mainWidget->setStyleSheet("#mainWidget{background:#FFFFFF;}");
            rightWid->setStyleSheet("#rightWid{background:#FFFFFF;}");
        }
}

void MainWid::dbusInit()
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(sessionBus.registerService("org.ukui.kylin_music"))
    {
        sessionBus.registerObject("/org/ukui/kylin_music",this,
                                  QDBusConnection::ExportAllContents);
        qDebug("dbus register ok.");
    }
}

void MainWid::gsettingInit()
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        themeData = new QGSettings(FITTHEMEWINDOW);
        if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
            WidgetStyle::themeColor = 1;
        }
        else
        {
            WidgetStyle::themeColor = 0;
        }

        connect(themeData,&QGSettings::changed,this,[=]()
        {
            qDebug() << "主题颜色" << themeData->get("style-name").toString();
            if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
                WidgetStyle::themeColor = 1;
                changeDarkTheme();
            }
            else
            {
                WidgetStyle::themeColor = 0;
                changeLightTheme();
            }
        });
    }
}

void MainWid::actionInit()
{
    connect(myTitleBar->miniBtn,&QPushButton::clicked,this,&MainWid::slot_showMiniWidget);
    connect(m_MiniWidget->m_recoveryWinBtn,&QPushButton::clicked,this,&MainWid::slot_recoverNormalWidget);

    this->setCentralWidget(mainWidget);

    connect(myPlaySongArea->playBtn,SIGNAL(clicked(bool)),this,SLOT(play_Song()));   //播放歌曲

    connect(mySideBar->myMusicListWid->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_listWidget_doubleClicked(QListWidgetItem*)));

    //播放列表右键菜单
    mySideBar->myMusicListWid->musicInfoWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mySideBar->myMusicListWid->musicInfoWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(on_musicInfoWidget_customContextMenuRequested(const QPoint&)));

    connect(myPlaySongArea->lastBtn,SIGNAL(clicked()),this,SLOT(on_lastBtn_clicked())); //上一首
//    connect(myPlaySongArea->lastBtn,SIGNAL(clicked()),this,SLOT(on_playlist_lastBtn_clicked())); //上一首
    connect(myPlaySongArea->nextBtn,SIGNAL(clicked()),this,SLOT(on_nextBtn_clicked())); //下一首
//    connect(myPlaySongArea->nextBtn,SIGNAL(clicked()),this,SLOT(on_playlist_nextBtn_clicked())); //下一首
    connect(mySideBar->myMusicListWid->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::updatalistwidget);

    connect(mySideBar->myMusicListWid->Music,SIGNAL(positionChanged(qint64)),this,SLOT(positionChange(qint64)));  //滑块进度条位置改变
    connect(mySideBar->myMusicListWid->Music,SIGNAL(durationChanged(qint64)),this,SLOT(durationChange(qint64)));
    connect(mySideBar->myMusicListWid->Music,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(currentMediaChanged(QMediaContent)));

    connect(hSlider,SIGNAL(sliderPressed()),this,SLOT(slidePress()));
//    connect(hSlider,SIGNAL(sliderMoved(int)),this,SLOT(slideMove(int)));
    connect(hSlider,SIGNAL(sliderReleased()),this,SLOT(slideRelease()));
    connect(hSlider,&QSlider::valueChanged,this,&MainWid::setPosition);
//    connect(hSlider,&MusicListWid::on_top_addSongBtn_slot,this,&MainWid::setHsliderPosition);

    connect(mySideBar->myMusicListWid->Music,&QMediaPlayer::stateChanged,this,&MainWid::Music_stateChang);

    connect(myPlaySongArea->playModeBtn,SIGNAL(clicked(bool)),this,SLOT(PlayModeChanged()));

    //迷你模式槽函数绑定
    connect(m_MiniWidget->m_nextBtn,&QPushButton::clicked,this,&MainWid::on_nextBtn_clicked);
    connect(m_MiniWidget->m_playStateBtn,&QPushButton::clicked,this,&MainWid::play_Song);
    connect(m_MiniWidget->m_preBtn,&QPushButton::clicked,this,&MainWid::on_lastBtn_clicked);
    connect(m_MiniWidget->m_orderBtn,&QPushButton::clicked,this,&MainWid::PlayModeChanged);
    connect(m_MiniWidget->m_closeBtn,&QPushButton::clicked,this,&MainWid::slot_closeMiniWidget);

    //历史播放列表相关
    connect(myPlaySongArea->listBtn,&QPushButton::clicked,this,&MainWid::showBeforeList);

    myPlaySongArea->mybeforeList->raise();

    //播放列表右键菜单
    mySideBar->songListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mySideBar->songListWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(on_sidebarWidget_customContextMenuRequested(const QPoint&)));

    //提示信息弹窗
    connect(mySideBar->renameSongListPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(renameThisSongList()));
    connect(mySideBar->promptSongListPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(promptThisSongList())); //删除歌单提示信息
    connect(mySideBar->promptExistListPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(renameSongListCon()));
    connect(mySideBar->promptRenamePlayList->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(promptRenamePlayList()));
    connect(mySideBar->promptRemovePlayList->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(promptRemovePlayList()));
    connect(mySideBar->renameSongListPup->enterLineEdit,SIGNAL(returnPressed()),this,SLOT(renameThisSongList()));
    connect(myTitleBar->aboutAct,SIGNAL(triggered(bool)),this,SLOT(showAboutWidget()));

//    connect(mySideBar->myMusicListWid,&MusicListWid::fromFilemanager,mySideBar->myMusicListWid,&MusicListWid::addFile);

    QShortcut *addvSlider=new QShortcut(Qt::Key_W,this);
    connect(addvSlider,&QShortcut::activated,this,&MainWid::addvSlider_slot);
    QShortcut *subvSlider=new QShortcut(Qt::Key_S,this);
    connect(subvSlider,&QShortcut::activated,this,&MainWid::subvSlider_slot);

    connect(myPlaySongArea->mybeforeList->emptyBtn,SIGNAL(clicked(bool)),this,SLOT(clear_HistoryPlayList()));
//    connect(mySideBar->likeAct,&QAction::triggered,this,&MainWid::deleteMusicFromSongList);

    mySideBar->sidemodel->select();
    for(int i = 0;i < mySideBar->sidemodel->rowCount();i++)
    {
        connect(mySideBar->musicListChangeWid[i]->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,SLOT(on_musicListChangeWid_doubleClicked(QListWidgetItem*)));
//        connect(mySideBar->musicListChangeWid[i]->PlayList,&QMediaPlaylist::currentIndexChanged,
//            this,&MainWid::updataplaylistwidget);
        connect(mySideBar->musicListChangeWid[i]->Music,&QMediaPlayer::stateChanged,
            this,&MainWid::Music_playlist_stateChang);
        connect(mySideBar->musicListChangeWid[i]->Music,SIGNAL(positionChanged(qint64)),
            this,SLOT(playlist_positionChange(qint64)));  //滑块进度条位置改变
        connect(mySideBar->musicListChangeWid[i]->Music,SIGNAL(durationChanged(qint64)),
            this,SLOT(playlist_durationChange(qint64)));
    }
    //深色模式
    connect(myTitleBar->darkThemeAct,&QAction::triggered,this,&MainWid::changeDarkTheme);
    connect(myTitleBar->lightThemeAct,&QAction::triggered,this,&MainWid::changeLightTheme);

//    connect(myTitleBar->darkThemeAct,SIGNAL(triggered(bool)),this,changeDarkTheme());
//    connect(myTitleBar->lightThemeAct,SIGNAL(triggered(bool)),this,changeLightTheme());

    connect(nullMusicWidget->n_addLocalSong,&QPushButton::clicked,this,&MainWid::nullMusicWidgetAddSong);
//    connect(nullMusicWidget->n_addLocalFolder,&QPushButton::clicked,this,&MainWid::nullMusicWidgetAddFile);  // 空页面添加文件夹
    // 拖拽添加歌曲
    connect(this,&MainWid::addFile,mySideBar->myMusicListWid,&MusicListWid::addFile);
//    connect(this,&MainWid::musicDbus,mykylinMuisc,&KylinMuisc::processArgs);
    // 显示搜索结果界面
    connect(myTitleBar->searchBtn,&QPushButton::clicked,this,&MainWid::showSearchResultWidget);
    // 隐藏搜索结果界面
    connect(mySideBar->PlayListBtn, &QToolButton::clicked, this, &MainWid::hideSearchResultWidget);
    for (int i = 0; i < mySideBar->songListWidget->count(); i++)
    {
        connect(mySideBar->newSongListBtn[i], &QToolButton::clicked, this, &MainWid::hideSearchResultWidget);
    }
    get_historyplaylist_information();
    initSystemTray();
    if(argName != "")
    {
        kylin_music_play_request(argName);
    }
}


int MainWid::kylin_music_play_request(QString path)
{
    qDebug()<<"path : "<<path;
    QStringList qStringListPath ;
    qStringListPath << path;
    qDebug()<<"qStringListPath : "<<qStringListPath;
    processArgs(qStringListPath);
    qDebug()<<"qStringListPath : "<<qStringListPath;
    if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
    {
        rightlayout->replaceWidget(nullMusicWidget,mySideBar->rightChangeWid);
        nullMusicWidget->hide();
        mySideBar->rightChangeWid->show();
    }
    return 0;
}

//void MainWid::paintEvent(QPaintEvent *event)
//{
//    //上半部分阴影
//    QPainterPath painterPath;
//    painterPath.setFillRule(Qt::WindingFill);//设置填充方式
//    QPainter painter(this);
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.fillPath(painterPath, QBrush(Qt::white));
//    //下半部分阴影
//    QPainterPath painterPath2;
//    painterPath2.setFillRule(Qt::WindingFilSinglel);

//}

//MainWid::MainWid(const QString &arch, const QString &snap, QWidget *parent)
//    : QMainWindow(parent)
////    , m_controllerWorker(controller)
//    , m_moveGui(false)
//    , m_resizeGui(false)
//    , m_arch(arch)
//    , m_snap(snap)
//{
//}

// 实现键盘响应
void MainWid::keyPressEvent(QKeyEvent *event)
{
    // F1快捷键打开用户手册
    if (event->key() == Qt::Key_F1) {
        if (!mDaemonIpcDbus->daemonIsNotRunning()){
            // F1快捷键打开用户手册，如kylin-music
            //由于是小工具类，下面的showGuide参数要填写"tools/kylin-recorder"
            mDaemonIpcDbus->showGuide("tools/kylin-music");
        }
    }
}

void MainWid::Single(QString path)   //单例
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString lockPath = homePath.at(0) + "/.config/kylin-music-lock";
    int fd = open(lockPath.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd < 0) { exit(1); }
    qDebug()<<"kylin-music is already running!";
    if (lockf(fd, F_TLOCK, 0))
    {
        QDBusInterface interface( "org.ukui.kylin_music", "/org/ukui/kylin_music","org.ukui.kylin_music.play", QDBusConnection::sessionBus());
        QDBusReply<int> reply = interface.call( "kylin_music_play_request", path);
        qDebug() << "file path is " << path;
        if ( reply.isValid() && reply.value() == 0)
        {
            qDebug( "%d", reply.value());          // prints 4
        }
        else
        {
            qDebug() << "fail";
        }
        exit(0);
    }
    argName = path;
//    if(path != "")
//    {
//        kylin_music_play_request(path);
//    }
}
MainWid::~MainWid()
{
    m_MiniWidget->~miniWidget();

}
#include <QPropertyAnimation>
void MainWid::slot_showMiniWidget()
{
//    this->hide();   //fyf
    // 添加过渡动画
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    connect(animation, &QPropertyAnimation::valueChanged, [&](QVariant value){
        update();
    });
    connect(animation, &QPropertyAnimation::finished, [&](){
        hide();
        setWindowOpacity(1);
    });

    QPropertyAnimation *animation_mini = new QPropertyAnimation(m_MiniWidget, "windowOpacity");
    animation_mini->setDuration(200);
    animation_mini->setStartValue(0);
    animation_mini->setEndValue(1);
    connect(animation_mini, &QPropertyAnimation::valueChanged, [&](QVariant value){
        m_MiniWidget->update();
    });

    m_MiniWidget->setWindowOpacity(0);
    m_MiniWidget->showNormal();
    m_MiniWidget->activateWindow();
    animation->start(QAbstractAnimation::DeleteWhenStopped);
    animation_mini->start(QAbstractAnimation::DeleteWhenStopped);
    // 添加过渡动画 =======================

//    m_MiniWidget->showNormal();

}

void MainWid::slot_closeMiniWidget()
{
    this->close();
}

void MainWid::slot_recoverNormalWidget()
{
    m_MiniWidget->hide();
    this->showNormal();
}

void MainWid::promptMessage()
{
    showPromptMessage();
    promptMessageTimer = new QTimer(this);
    promptMessageTimer->start(1000);
    promptMessageTimer->setSingleShot(true);
    connect(promptMessageTimer,&QTimer::timeout,this,&MainWid::closePromptMessage);
}

void MainWid::play_Song()
{
    if(mySideBar->currentSelectList == -1)
    {
        if (mySideBar->myMusicListWid->musicInfoWidget->count() != 0)
        {
            if(mySideBar->myMusicListWid->isStartPlay)
            {
                mySideBar->myMusicListWid->isStartPlay = false;
                qDebug()<<"currentPlayList : "<<mySideBar->currentPlayList;
                myPlaySongArea->playBtn->setFixedSize(36,36);
                myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);

//                myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
                myPlaySongArea->playBtn->setIconSize(QSize(36,36));
                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

                //迷你模式样式
//                m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/play2.png"));
                m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

                mySideBar->myMusicListWid->Music->pause();
            }
            //
            else
            {
                mySideBar->myMusicListWid->isStartPlay = true;
                myPlaySongArea->playBtn->setFixedSize(36,36);
                myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
//                myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
                myPlaySongArea->playBtn->setIconSize(QSize(36,36));
                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");


                //迷你模式样式
//                m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/pause2.png"));
                m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

                qDebug() << "播放歌曲列表";
    //            mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
//                mySideBar->myMusicListWid->Music->play();
                albumCover_local_playlist();  //本地和歌单默认封面
//                int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
//                mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
//                model->select();
//                QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
//                mySideBar->myMusicListWid->Music->play();

//                 int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
                int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
                int music = mySideBar->myMusicListWid->localAllMusicid[currentIndex].toInt();
                 model->setTable("LocalMusic");
                 model->select();
                 QString Name = model->data(model->index(music, 1)).toString();
                 mySideBar->myMusicListWid->Music->play();
                 qDebug()<< "Name :  " <<Name;
                 myPlaySongArea->songText(Name); // 正在播放
                 m_MiniWidget->m_songNameLab->setText(Name);
//                albumCover_local();           //本地专辑
            }
        }
    }
    else if(mySideBar->currentPlayList >= 0)
    {
        if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->state() == QMediaPlayer::PlayingState)
        {
            qDebug()<<"暂停播放";
            myPlaySongArea->playBtn->setFixedSize(36,36);
            myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
//            myPlaySongArea->playBtn->setToolTip(tr("播放"));
//            myPlaySongArea->playBtn->setToolTip(tr("play"));
//            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
            myPlaySongArea->playBtn->setIconSize(QSize(36,36));
            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

            //迷你模式样式
//            m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/play2.png"));
            m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
            // 根据当前播放的列表进行暂停

            mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->pause();
            return ;
        }

        // 没有歌曲在播放
        else
        {
            myPlaySongArea->playBtn->setFixedSize(36,36);
            myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);

//            myPlaySongArea->playBtn->setToolTip("暂停");
//            myPlaySongArea->playBtn->setToolTip(tr("pause"));
//            myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
            myPlaySongArea->playBtn->setIconSize(QSize(36,36));
            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/play.png);}");

            //迷你模式样式
//            m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/pause2.png"));
            m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

            // 播放歌曲，并设置当前播放列表索引
            qDebug() << "播放歌单";
            mySideBar->myMusicListWid->Music->stop();
            mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
//            albumCover_local_playlist();  //本地和歌单默认封面
//            playlist_playInformation();   //歌单播放信息
//            new_PlayList();               //判断本地歌单歌曲是否存在
//            albumCover_playlist();        //歌单专辑
        }
    }
}
//void MainWid::setHsliderPosition()
//{
//    int position;
//    if(mySideBar->myMusicListWid->Music->state()==QMediaPlayer::PlayingState)
//    {
//        position = mySideBar->myMusicListWid->Music->position();
//        mySideBar->myMusicListWid->Music->setPosition(position);
//        hSlider->setValue(position);
//        mySideBar->myMusicListWid->Music->play();
//        mySideBar->myMusicListWid->isStartPlay = true;
//    }
//    else
//    {
//        mySideBar->myMusicListWid->Music->stop();
//        mySideBar->myMusicListWid->isStartPlay = false;
//    }
//}

void MainWid::on_sidebarWidget_customContextMenuRequested(const QPoint &pos)     //歌曲列表右键菜单
{
    QListWidgetItem *curItem1 = mySideBar->songListWidget->itemAt(pos);

    if(curItem1 == NULL)
    {
        return;
    }
    sideMenu = new QMenu(this);
    actionPlay = new QAction(this);
    actionRename = new QAction(this);
    actionDelete = new QAction(this);

//    actionPlay->setText("播放/暂停");
//    actionPlay->setText(tr("Play/Pause"));
//    actionRename->setText("重命名");
    actionRename->setText(tr("rename"));
//    actionDelete->setText("删除歌单");
    actionDelete->setText(tr("Delete the playlist"));

//    sideMenu->addAction(actionPlay);
    sideMenu->addAction(actionRename);
    sideMenu->addAction(actionDelete);

    connect(actionDelete,SIGNAL(triggered(bool)),this,SLOT(deleteThisSongList()));
    connect(actionRename,SIGNAL(triggered(bool)),this,SLOT(showRenameDlg()));

    sideMenu->exec(QCursor::pos());

    delete sideMenu;
    delete actionPlay;
    delete actionRename;
    delete actionDelete;
}

void MainWid::on_musicInfoWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *curItem1=mySideBar->myMusicListWid->musicInfoWidget->itemAt(pos);

    if(curItem1 == NULL)
    {
        return;
    }

    Menu = new QMenu(mySideBar->myMusicListWid->musicInfoWidget);
    playAct = new QAction(this);
    nextAct = new QAction(this);
    likeAct = new QAction(this);
    deleAct = new QAction(this);
    listAct = new QAction(this);
    //    lookAct = new QAction(this);
    songAct = new QAction(this);
    //    playAct->setIcon(QIcon(":/image/image/bofang1.png"));

//    playAct->setText("播放");
    playAct->setText(tr("play"));
    //    nextAct->setIcon(QIcon(":/image/image/like_outline_32px_1170275_easyicon.net.png"));
//    nextAct->setText("下一首");
    nextAct->setText(tr("The following piece"));
    //    deleAct->setIcon(QIcon(":/image/image/list_32px_1142913_easyicon.net.png"));
//    likeAct->setText("添加到我喜欢");
    likeAct->setText(tr("Add to what I like"));
    //    likeAct->setIcon(QIcon(":/image/image/delete_32px_1168909_easyicon.net.png"));
//    deleAct->setText("从歌曲列表中删除");
    deleAct->setText(tr("Remove from the song list"));
//    listAct->setText("添加到歌单");
    listAct->setText(tr("Add to playlist"));
    //    listAct->setIcon(QIcon(":/image/image/inf.png"));
    //    listAct->setText("从本地删除");
    //    lookAct->setText("查看本地文件");
//    songAct->setText("歌曲信息");
    songAct->setText(tr("Song information"));

//    Menu->addAction(playAct);
//    Menu->addAction(nextAct);
//    Menu->addAction(likeAct);
//    Menu->addAction(listAct);

    QMenu *menu5 = new QMenu(this);
//    menu5->setTitle("添加到歌单");
    menu5->setTitle(tr("Add to playlist"));
    model_1->setTable("NewPlayList");
    model_1->select();

    for(int i = 0; i < model_1->rowCount(); i++)
    {

        QString listName = model_1->data(model_1->index(i,0)).toString();
        QAction *listaction = new QAction(this);
        listaction->setData(i);
        listaction->setText(listName);
        menu5->addAction(listaction);

        qDebug() << "listindexlistactionlistaction " << i;
    }
//    connect(menu5,&QMenu::triggered(QAction* listact) ,this,&MainWid::add_music_to_songlist(QAction *listact));

    connect(menu5, SIGNAL(triggered(QAction*)), this, SLOT(add_music_to_songlist(QAction*)));


    Menu->addMenu(menu5);


    Menu->addAction(deleAct);
    //    Menu->addAction(listAct);
    //    Menu->addAction(lookAct);
    Menu->addAction(songAct);

//    connect(playAct,&QAction::triggered,this,&MainWid::playOrPauseAct);  //jialin
//    connect(nextAct,&QAction::triggered,this,&MainWid::playNextSongAct);
//    connect(likeAct,&QAction::triggered,this,&MainWid::add_music_to_songlist);
    connect(deleAct,&QAction::triggered,this,&MainWid::deleteMusicFromLocalList);
//    connect(listAct,&QAction::triggered,this,&MainWid::add_music_to_songlist);
    connect(songAct,&QAction::triggered,this,&MainWid::getSongInfoAct);
    Menu->exec(QCursor::pos());
    delete Menu;
    delete playAct;
    delete nextAct;
    //    delete likeAct;
    delete deleAct;
    //    delete listAct;
    //    delete lookAct;
    delete songAct;

}


// 右键播放暂停功能
void MainWid::playOrPauseAct()
{
    if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
    {
        if(mySideBar->myMusicListWid->musicInfoWidget->item(mySideBar->myMusicListWid->PlayList->currentIndex())->isSelected())
        {

            if(mySideBar->myMusicListWid->Music->state()==QMediaPlayer::PlayingState)
            {
                qDebug()<<"暂停";
                playAct->setText(tr("pause"));
                mySideBar->myMusicListWid->Music->pause();
            }
            else
            {
                qDebug()<<"播放";
                playAct->setText(tr("play"));
                if (mySideBar->currentPlayList != -1 && mySideBar->currentPlayList != -2) {
                    mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
                }
                if (mySideBar->myMusicListWid->Music->playlist() != mySideBar->myMusicListWid->PlayList) {
                    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
                }
                mySideBar->myMusicListWid->Music->play();
            }
        }
        else
        {
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row());
            playAct->setText(tr("play"));
            if (mySideBar->currentPlayList != -1 && mySideBar->currentPlayList != -2) {
                mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
            }
            if (mySideBar->myMusicListWid->Music->playlist() != mySideBar->myMusicListWid->PlayList) {
                mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
            }
            mySideBar->myMusicListWid->Music->play();

        }
    }
    else
    {
        mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
        mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row());
        playAct->setText(tr("play"));
        if (mySideBar->currentPlayList != -1 && mySideBar->currentPlayList != -2) {
            mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
        }
        if (mySideBar->myMusicListWid->Music->playlist() != mySideBar->myMusicListWid->PlayList) {
            mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
        }
        mySideBar->myMusicListWid->Music->play();
    }
}

// 下一首功能
void MainWid::playNextSongAct()
{
    if (mySideBar->currentSelectList == -1)
    {
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
        if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
        {
            int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(++currentIndex == mySideBar->myMusicListWid->PlayList->mediaCount())
            {
                currentIndex = 0;
            }
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
        }
        mySideBar->myMusicListWid->Music->play();
    }
    else if (mySideBar->currentPlayList >= 0)
    {
        mySideBar->myMusicListWid->Music->stop();

        if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->playlist() ==
                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList)
        {
            int currentIndex = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();
            //        mpwid->setCurrentIndex(0);
            //        pushbuttonstyle();
            if(++currentIndex == mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->mediaCount())
            {
                currentIndex = 0;
            }
            mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(currentIndex);
        }
        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
    }
}

void MainWid::add_music_to_songlist(QAction *listact)   //添加歌曲到歌单
{
    int listindex = listact->data().toInt();

    model->setTable("LocalMusic");
    model->select();
    model_1->select();

    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();

    QString musicidStr = mySideBar->myMusicListWid->localAllMusicid[row];
    int musicIndex = musicidStr.toInt();  //jialin

    QString idListstr = model_1->data(model_1->index(listindex, 1)).toString();

    QStringList idStringList = idListstr.split(",");

    if(idStringList.indexOf(musicidStr) != -1)
    {
        QMessageBox *message = new QMessageBox(this);
//        message->about(this,tr("提示信息"),tr("歌单中歌曲已存在！"));
        message->about(this,tr("Prompt information"),tr("The songs in the playlist already exist!"));
        return ;
    }
    else
    {
        idListstr = idListstr.append(musicidStr + ",");
        model_1->setData(model_1->index(listindex,1),idListstr);

        model->submitAll();
        model->select();

        model_1->submitAll();
        model_1->select();

        qDebug() << "listindexlistindexlistindex1  " <<listindex;

        QString newid = model_1->data(model_1->index(listindex, 1)).toString();

        QString songName = model->data(model->index(musicIndex,1)).toString();
        QString Path = model->data(model->index(musicIndex,2)).toString();
        QString Title = model->data(model->index(musicIndex,3)).toString();
        QString Album = model->data(model->index(musicIndex,4)).toString();
        QString Time = model->data(model->index(musicIndex,7)).toString();

        if(songName != ""&&Path != "")
        {
            QListWidgetItem *item1=new QListWidgetItem(mySideBar->musicListChangeWid[listindex]->musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            mySideBar->musicListChangeWid[listindex]->musicInfoWidget->setItemWidget(item1,songitem1);
//            songitem1->songNameLabel->setText(songName); //歌曲名称
//            songitem1->albumLabel->setText(Album);  //专辑
//            songitem1->songTimeLabel->setText(Time); //时长
//            songitem1->singerLabel->setText(Title); //歌手
            qDebug()<<"songName : "<<songName<<"  "<<"Title : "<<Title<<"  "<<"Album : "<<Album;
            songitem1->song_singer_albumText(songName,Title,Album); //歌曲名称 歌手 专辑
            songitem1->songTimeLabel->setText(Time); //时长

            mySideBar->musicListChangeWid[listindex]->PlayList->addMedia(QUrl::fromLocalFile(Path));

            mySideBar->musicListChangeWid[listindex]->musicInfoWidget->show();
        }

//        mySideBar->musicListChangeWid[listindex]->Music->setPlaylist(mySideBar->musicListChangeWid[listindex]->PlayList);
//        mySideBar->musicListChangeWid[listindex]->songNumberLabel->setText("共"+QString::number(mySideBar->musicListChangeWid[listindex]->musicInfoWidget->count())+"首");
        mySideBar->musicListChangeWid[listindex]->songNumberLabel->setText(
                    tr("A total of")+QString::number(mySideBar->musicListChangeWid[listindex]->musicInfoWidget->count())+tr("The first"));
    }

    connect(mySideBar->musicListChangeWid[listindex]->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this,SLOT(on_musicListChangeWid_doubleClicked(QListWidgetItem*)));
//    connect(mySideBar->musicListChangeWid[listindex]->PlayList,&QMediaPlaylist::currentIndexChanged,
//        this,&MainWid::updataplaylistwidget);
    connect(mySideBar->musicListChangeWid[listindex]->Music,&QMediaPlayer::stateChanged,
        this,&MainWid::Music_playlist_stateChang);
    connect(mySideBar->musicListChangeWid[listindex]->Music,SIGNAL(positionChanged(qint64)),
        this,SLOT(playlist_positionChange(qint64)));  //滑块进度条位置改变
    connect(mySideBar->musicListChangeWid[listindex]->Music,SIGNAL(durationChanged(qint64)),
        this,SLOT(playlist_durationChange(qint64)));
}

void MainWid::deleteThisSongList()
{
//    QMessageBox::about(this,"提示信息","确定删除此歌单？");
//    QMessageBox messageBox;
//    messageBox.information(this, "提示信息", "确认删除此歌单？", QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    model_1->select();

//    QMessageBox messageBox(QMessageBox::NoIcon,
//                               "删除歌单", "你确定要删除吗?",
//                               QMessageBox::Yes | QMessageBox::No, NULL);
//    QMessageBox messageBox(QMessageBox::NoIcon,
//                               tr("Delete the playlist"), tr("Are you sure you want to delete it?"),
//                               QMessageBox::Yes | QMessageBox::No, NULL);
//    int result = messageBox.exec();
//    if (result == QMessageBox::Yes)
//    {
//        int row = mySideBar->songListWidget->currentIndex().row();
//        if(row != 0)
//        {
//            model_1->removeRow(row);
//            model_1->submitAll();
//            model_1->select();
//            qDebug() << "model_1->rowCount()" << model_1->rowCount();

//            mySideBar->songListWidget->removeItemWidget(mySideBar->songListWidget->item(row));
//            delete mySideBar->songListWidget->item(row);

//        }
//        else
//        {
////            QMessageBox::about(this,"提示信息","默认歌单无法删除！");
//            QMessageBox::about(this,tr("Prompt information"),tr("Default playlist cannot be deleted!"));
//        }
//    }
//    else
//    {
//        messageBox.close();
//    }
    mySideBar->promptSongListPup->stackWid->setCurrentIndex(1);
    mySideBar->promptSongListPup->pupDialog->show();
    model_1->select();
}



void MainWid::showRenameDlg()
{
    mySideBar->renameSongListPup->stackWid->setCurrentIndex(0);
    mySideBar->renameSongListPup->pupDialog->raise();
    mySideBar->renameSongListPup->pupDialog->show();
    mySideBar->renameSongListPup->enterLineEdit->setFocus();
}

void MainWid::renameThisSongList()
{
    QString newListName = mySideBar->renameSongListPup->enterLineEdit->text();
    mySideBar->renameSongListPup->enterLineEdit->clear();
    model_1->select();
    int row = mySideBar->songListWidget->currentIndex().row();
//    QString oldListName = model_1->data(model_1->index(row,0)).toString();
//    QString
    if(row != 0)
    {
        if(mySideBar->allListName.indexOf(newListName) != -1)
        {
            mySideBar->promptSongListPup->pupDialog->hide();
//            mySideBar->renameSongListPup->pupDialog->hide();
//            QMessageBox::about(this,tr("提示信息"),tr("歌单名已存在！"));
//            QMessageBox::about(this,tr("Prompt information"),tr("Single name already exists!"));
            mySideBar->promptExistListPup->stackWid->setCurrentIndex(3);
            mySideBar->promptExistListPup->pupDialog->show();
            qDebug()<<"歌单名已存在";
        }
        else
        {
            mySideBar->allListName[row] = newListName;
            mySideBar->newSongListBtn[row]->setText(newListName);
            mySideBar->musicListChangeWid[row]->songListLabel->setText(newListName);
            model_1->setData(model_1->index(row,0),newListName);
            model_1->submitAll();
            model_1->select();
        }
    }
    else
    {
        mySideBar->promptRemovePlayList->pupDialog->hide();
//        QMessageBox::about(this,"提示信息","默认歌单无法重命名！");
//        QMessageBox::about(this,tr("Prompt information"),tr("Default playlist cannot be renamed!"));
        mySideBar->promptRemovePlayList->stackWid->setCurrentIndex(4);
        mySideBar->promptRemovePlayList->pupDialog->show();

    }
    model_1->select();
    mySideBar->renameSongListPup->pupDialog->hide();
}

void MainWid::promptThisSongList()      //删除歌单提示信息
{
    int row = mySideBar->songListWidget->currentIndex().row();
    if(row != 0)
    {
        model_1->removeRow(row);
        model_1->submitAll();
        model_1->select();
        mySideBar->songListWidget->removeItemWidget(mySideBar->songListWidget->item(row));
        delete mySideBar->songListWidget->item(row);
        int counter = mySideBar->musicListChangeWid[row]->musicInfoWidget->count();
        QListWidgetItem *item;
        for(int index = 0;index < counter;index++)
        {
            item = mySideBar->musicListChangeWid[row]->musicInfoWidget->takeItem(0);
            delete item;
        }
        mySideBar->musicListChangeWid[row]->Music->stop();
//        myPlaySongArea->songText("");
//        myPlaySongArea->bottomLeftLabel->setText("");
        int thisrow = mySideBar->musicListChangeWid[row]->PlayList->currentIndex();

        qDebug()<<mySideBar->musicListChangeWid[row]->musicInfoWidget->count();
        qDebug()<<"thisrow : "<<thisrow;
        qDebug()<<"thisrow : "<<thisrow;
        update();
    }
    else
    {
        mySideBar->promptSongListPup->pupDialog->hide();
//            QMessageBox::about(this,"提示信息","默认歌单无法删除！");
//        QMessageBox::about(this,tr("Prompt information"),tr("Default playlist cannot be deleted!"));
        mySideBar->promptRenamePlayList->stackWid->setCurrentIndex(2);
        mySideBar->promptRenamePlayList->pupDialog->show();
    }
    mySideBar->promptSongListPup->pupDialog->hide();
}

void MainWid::renameSongListCon()
{
    mySideBar->promptExistListPup->pupDialog->hide();
}

void MainWid::promptRemovePlayList()
{
    mySideBar->promptRemovePlayList->pupDialog->hide();
}


void MainWid::promptRenamePlayList()
{
    mySideBar->promptRenamePlayList->pupDialog->hide();
}

// 从歌曲列表里删除
void MainWid::deleteMusicFromLocalList()
{
    // 删除歌曲时跳过高亮变换
    skipPlayHighlight = true;
    int Row = mySideBar->myMusicListWid->PlayList->currentIndex();      //判断当前播放歌曲和当前点击歌曲
    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();

    // 获取歌曲对应的歌曲id
    QString musicId = mySideBar->myMusicListWid->localAllMusicid[row];

    qDebug() << "歌单QString musicId " << musicId;
    // 获取歌曲id对应的数据索引
    qDebug()<<"musicId.toInt() : "<<musicId.toInt();
    int musicIndex = musicId.toInt();
    qDebug()<<musicIndex;

    model->setTable("LocalMusic");
    model->select();
    model->setData(model->index(musicIndex, 1), "");
    model->setData(model->index(musicIndex, 2), "");

    mySideBar->myMusicListWid->allmusic.removeAt(row);
    mySideBar->myMusicListWid->localAllMusicid.removeAt(row);
    mySideBar->myMusicListWid->musicInfoWidget->removeItemWidget(mySideBar->myMusicListWid->musicInfoWidget->item(row));
    delete mySideBar->myMusicListWid->musicInfoWidget->item(row);

    //  如果是当前播放的，则停止播放
    if (Row == row)
    {
        qDebug()<<"Row == row";
//        if(mySideBar->myMusicListWid->musicInfoWidget->count() == 1)
//        {
//            qDebug()<<"Row == row";
//            mySideBar->myMusicListWid->Music->stop();
//            mySideBar->myMusicListWid->PlayList->removeMedia(row);
//            model->submitAll();
//            QPixmap default_photo = QPixmap(":/img/fengmian.png");
//            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
//            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
//            m_MiniWidget->m_coverLabel->setPixmap(default_photo);

//    //        int musicid = mySideBar->myMusicListWid->localAllMusicid[Row].toInt();
//    //        QString mp3Name = model->data(model->index(musicid, 1)).toString();
//            myPlaySongArea->songText("");
//            myPlaySongArea->bottomLeftLabel->setText("");
//        }
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
            qDebug()<<"Row == row   mySideBar->myMusicListWid->musicInfoWidget->count() - 1";
            if (Row == mySideBar->myMusicListWid->musicInfoWidget->count() - 1)
            {
                if(mySideBar->myMusicListWid->musicInfoWidget->count() == 1)
                {
                    qDebug()<<"Row == row";
                    mySideBar->myMusicListWid->Music->stop();
                    mySideBar->myMusicListWid->PlayList->removeMedia(row);
                    model->removeRow(Row);
                    model->submitAll();
                    QPixmap default_photo = QPixmap(":/img/fengmian.png");
                    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
                    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
                    m_MiniWidget->m_coverLabel->setPixmap(default_photo);

            //        int musicid = mySideBar->myMusicListWid->localAllMusicid[Row].toInt();
            //        QString mp3Name = model->data(model->index(musicid, 1)).toString();
                    myPlaySongArea->songText("");
                    myPlaySongArea->bottomLeftLabel->setText("");
                }
                else
                {
                    mySideBar->myMusicListWid->Music->stop();
                    mySideBar->myMusicListWid->PlayList->removeMedia(Row);
                    model->removeRow(Row);
                    model->submitAll();
                    mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
                    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
                }
            }
            else
            {
                mySideBar->myMusicListWid->Music->stop();
                mySideBar->myMusicListWid->PlayList->removeMedia(row);
                model->removeRow(row);
                model->submitAll();
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(row);
                mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
            }
        }
//        else
//        {
//            rightlayout->replaceWidget(nullMusicWidget,mySideBar->rightChangeWid);
//            nullMusicWidget->show();
//            mySideBar->rightChangeWid->hide();
//        }
    }
    if (Row > row)
    {
        int position;
        if(mySideBar->myMusicListWid->Music->state()==QMediaPlayer::PlayingState)
        {
            position=mySideBar->myMusicListWid->Music->position();
        }
        mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
        mySideBar->myMusicListWid->currentPlayIndex = 0;

        mySideBar->myMusicListWid->PlayList->removeMedia(row, row);

        mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
        mySideBar->myMusicListWid->PlayList->setCurrentIndex(Row - 1);
        mySideBar->myMusicListWid->currentPlayIndex = Row - 1;

        mySideBar->myMusicListWid->Music->setPosition(position);
        hSlider->setValue(position);
        mySideBar->myMusicListWid->Music->play();
    }
    else if (Row < row)
    {
        mySideBar->myMusicListWid->PlayList->removeMedia(row, row);
        model->removeRow(row);
        model->submitAll();
    }
//    mySideBar->myMusicListWid->PlayList->removeMedia(row);

//    if(mySideBar->myMusicListWid->musicInfoWidget->count() == 0)
//    {
////        rightlayout->replaceWidget(nullMusicWidget,mySideBar->rightChangeWid);
//        mySideBar->rightChangeWid->hide();
//        nullMusicWidget->show();
////        rightlayout->addWidget(mySideBar->);
//    }

    model_1->select();

    // 从每个歌单中删除此id
    for (int i = 0; i < model_1->rowCount(); i++)
    {
        QString idListstr = model_1->data(model_1->index(i, 1)).toString();
        qDebug() << "判断" << idListstr.split(",");
        QStringList idListstrList = idListstr.split(",");
        int tempmusicrow = idListstrList.indexOf(musicId);

        qDebug() << "tempmusicrow" << tempmusicrow << musicId;

        if (tempmusicrow == -1)
        {
            continue ;
        }

        idListstr = idListstr.replace(musicId + ",", "");
        model_1->setData(model_1->index(i,1),idListstr);
        model_1->submitAll();
        model_1->select();
        mySideBar->get_listmusic_information(i);

//        mySideBar->musicListChangeWid[i]->allmusic.removeAt(tempmusicrow);
//        mySideBar->musicListChangeWid[i]->musicInfoWidget->removeItemWidget(mySideBar->musicListChangeWid[i]->musicInfoWidget->item(tempmusicrow));
//        delete mySideBar->musicListChangeWid[i]->musicInfoWidget->item(tempmusicrow);
//        mySideBar->musicListChangeWid[i]->musicInfoWidget->takeItem(tempmusicrow);

//        mySideBar->musicListChangeWid[i]->PlayList->removeMedia(tempmusicrow);
//        mySideBar->musicListChangeWid[i]->Music->setPlaylist(mySideBar->musicListChangeWid[i]->PlayList);

//        mySideBar->musicListChangeWid[i]->songNumberLabel->setText("共"+QString::number(mySideBar->musicListChangeWid[i]->musicInfoWidget->count())+"首");
    }

    model->submitAll();
    model->select();

    model_1->submitAll();
    model_1->select();

    qDebug() << "model->rowCount()" << model->rowCount();
    //                mySideBar->myMusicListWid->allmusic.removeAt(row);

//    mySideBar->myMusicListWid->songNumberLabel->setText("共"+QString::number(mySideBar->myMusicListWid->musicInfoWidget->count())+"首");
    mySideBar->myMusicListWid->songNumberLabel->setText(
                tr("A total of")+QString::number(mySideBar->myMusicListWid->musicInfoWidget->count())+tr("The first"));
    // 删除歌曲结束时开启高亮变换
    skipPlayHighlight = false;
}

//void MainWid::deleteMusicFromSongList()
//{
//    // 删除歌曲时跳过高亮变换
//    skipPlaylistHighlight = true;
//    qDebug()<<"currentSelectList currentSelectList : "<<mySideBar->currentSelectList;
//    int tempmusicRow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();                 //正在播放
//    int tempmusicrow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->currentIndex().row();    //选中

//    int templistrow = mySideBar->currentSelectList;
//    QString tempAllId = model_1->data(model_1->index(templistrow,1)).toString();
//    int tempid = tempAllId.split(",")[tempmusicrow].toInt();

//    QString idstr = model_1->data(model_1->index(templistrow, 1)).toString();
//    idstr = idstr.replace(QString::number(tempid) + ",", "");

//    model_1->setData(model_1->index(templistrow,1),idstr);


//    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->localAllMusicid.removeAt(tempmusicrow);
//    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->
//            removeItemWidget(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->item(tempmusicrow));
//    delete mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->item(tempmusicrow);


//    if (tempmusicRow > tempmusicrow)
//    {

////        musicListChangeWid[templistrow]->PlayList->setCurrentIndex(0);
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->removeMedia(tempmusicrow, tempmusicrow);
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->setPlaylist(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList);
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(tempmusicRow - 1);

////        musicListChangeWid[templistrow]->Music->setPosition(position);
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
//    }
//    else if (tempmusicRow == tempmusicrow)
//    {
//        qDebug()<<"currentSelectList currentSelectList : "<<mySideBar->currentSelectList;
//        if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count() == 1)
//        {
//            qDebug()<<"currentSelectList currentSelectList : ";
//            mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
//            myPlaySongArea->songText("");
//            myPlaySongArea->bottomLeftLabel->setText("");
//        }
//        else if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count() > 1)
//        {
//            if (tempmusicRow == mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count() - 1)
//            {
//                qDebug()<<"currentSelectList currentSelectList : "<<mySideBar->currentSelectList;
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->removeMedia(tempmusicRow);
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(0);
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->setPlaylist(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList);
//            }
//            else
//            {
//                qDebug()<<"currentSelectList : "<<mySideBar->currentSelectList;
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
//                qDebug()<<"currentSelectList : "<<mySideBar->currentSelectList;
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->removeMedia(tempmusicRow);
//                qDebug()<<"currentSelectList : "<<mySideBar->currentSelectList;
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(tempmusicRow);
//                qDebug()<<"currentSelectList : "<<mySideBar->currentSelectList;
//                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->setPlaylist(mySideBar->musicListChangeWid[templistrow]->PlayList);
//            }
//        }
//    }
//    else if (tempmusicRow < tempmusicrow)
//    {
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->removeMedia(tempmusicrow, tempmusicrow);
//    }

//    model_1->submitAll();
//    model_1->select();

////        musicListChangeWid[currentSelectList]->songNumberLabel->setText("共"+QString::number(musicListChangeWid[currentSelectList]->musicInfoWidget->count())+"首");
//    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->songNumberLabel->setText(
//                tr("A total of")+QString::number(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count())+tr("The first"));
//    // 删除歌曲结束时开启高亮变换
//    skipPlaylistHighlight = false;
//}

//歌曲信息
void MainWid::getSongInfoAct()
{
    mySongInfoWidget = new SongInfoWidget;
    mySongInfoWidget->songInfoDlg->show();

    model->setTable("LocalMusic");
    model->select();
    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();

    int musicid = mySideBar->myMusicListWid->localAllMusicid[row].toInt();

    QString titleStr  = model->data(model->index(musicid, 1)).toString();
    QString pathStr   = model->data(model->index(musicid, 2)).toString();
    QString artistStr = model->data(model->index(musicid, 3)).toString();
    QString albumStr  = model->data(model->index(musicid, 4)).toString();
    QString type      = model->data(model->index(musicid, 5)).toString();
    QString mp3Size   = model->data(model->index(musicid, 6)).toString();
    QString timeStr   = model->data(model->index(musicid, 7)).toString();

//    mySongInfoWidget->songInfoEdit->append(titleStr);
//    mySongInfoWidget->songInfoEdit->append(artistStr);
//    mySongInfoWidget->songInfoEdit->append(albumStr);
//    mySongInfoWidget->songInfoEdit->append(type);
//    mySongInfoWidget->songInfoEdit->append(mp3Size);
//    mySongInfoWidget->songInfoEdit->append(timeStr);
//    mySongInfoWidget->songInfoEdit->append(pathStr);

//    mySongInfoWidget->titleLab ->setText(tr("歌曲名称:"));
    mySongInfoWidget->titleLab ->setText(tr("The song name:"));
//    mySongInfoWidget->artistLab->setText(tr("歌曲歌手:"));
    mySongInfoWidget->artistLab->setText(tr("singer:"));
//    mySongInfoWidget->albumLab ->setText(tr("歌曲专辑:"));
    mySongInfoWidget->albumLab ->setText(tr("album:"));
//    mySongInfoWidget->typeLab  ->setText(tr("文件类型:"));
    mySongInfoWidget->typeLab  ->setText(tr("The file type:"));
//    mySongInfoWidget->sizeLab  ->setText(tr("文件大小:"));
    mySongInfoWidget->sizeLab  ->setText(tr("The file size:"));
//    mySongInfoWidget->timeLab  ->setText(tr("文件时长:"));
    mySongInfoWidget->timeLab  ->setText(tr("File length:"));
//    mySongInfoWidget->pathLab  ->setText(tr("文件位置:"));
    mySongInfoWidget->pathLab  ->setText(tr("File location:"));


    mySongInfoWidget->musicNameEdit->setText(titleStr);
    mySongInfoWidget->singerNameEdit->setText(artistStr);
    mySongInfoWidget->albumNameEdit->setText(albumStr);
    mySongInfoWidget->fileTypeLab->setText(" "+type);
    mySongInfoWidget->fileSizeLab->setText(" "+mp3Size);
    mySongInfoWidget->fileTimeLab->setText(" "+timeStr);
//    mySongInfoWidget->filePathLab->setText(" "+pathStr);
    QString showpathStr   = "";
    if(pathStr.length() > 30)
    {
        showpathStr = pathStr.mid(0,29);
        showpathStr.append("...");
        mySongInfoWidget->filePathLab->setText(showpathStr);
        mySongInfoWidget->filePathLab->setToolTip(pathStr);
    }
    else
    {
        mySongInfoWidget->filePathLab->setText(pathStr);
//        mySongInfoWidget->filePathLab->setToolTip(pathStr);
    }

//        QStringList list;
//        list << MusicName << Author << Album
//             << type << time
//             << FileName;

//        for(int i = 0;i < list.length();++i)
//        {

//        }
//    QMessageBox::about(this,tr("歌曲信息"),tr("歌曲名称: %1 \n"
//                                                    "歌手: %2 \n"
//                                                    "专辑: %3 \n"
//                                                    "文件类型: %4 \n"
//                                                    "文件大小: %5 \n"
//                                                    "文件时长: %6 \n"
//                                                    "文件位置: %7 \n").arg(titleStr)
//                                                                    .arg(artistStr)
//                                                                    .arg(albumStr)
//                                                                    .arg(type)
//                                                                    .arg(mp3Size)
//                                                                    .arg(timeStr)
//                                                                    .arg(pathStr));
//    QMessageBox::about(this,tr("Song information"),QString(tr("The song name") + ": %1 \n" +
//                                                   tr("singer")        + ": %2 \n" +
//                                                   tr("The album")     + ": %3 \n" +
//                                                   tr("The file type") + ": %4 \n" +
//                                                   tr("The file size") + ": %5 \n" +
//                                                   tr("File length")   + ": %6 \n" +
//                                                   tr("File location") + ": %7 \n")
//                                                                    .arg(titleStr)
//                                                                    .arg(artistStr)
//                                                                    .arg(albumStr)
//                                                                    .arg(type)
//                                                                    .arg(mp3Size)
//                                                                    .arg(timeStr)
//                                                                    .arg(pathStr));
}

void MainWid::updatalistwidget(int value)//更新listWidget
{
    if (skipPlayHighlight)
    {
        return ;
    }

    mySideBar->myMusicListWid->musicInfoWidget->item(value)->setSelected(true);
    if(mySideBar->myMusicListWid->currentPlayIndex != -1)
    {
//        lastPlayIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
        QWidget* lastitemWid = mySideBar->myMusicListWid->musicInfoWidget->itemWidget
                (mySideBar->myMusicListWid->musicInfoWidget->item
                (mySideBar->myMusicListWid->currentPlayIndex));
        QList<QLabel*> lastsongItemList = lastitemWid->findChildren<QLabel*>();
        for(int i = 0; i < lastsongItemList.count(); i++)
        {
            lastsongItemList[i]->setStyleSheet("width: 182px;\
                                           height: 14px;\
                                           font-size: 14px;\
                                           font-weight: 400;\
                                           color:#303133;\
                                           line-height: 14px;");
        }
    }
    mySideBar->myMusicListWid->currentPlayIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
    QWidget* itemWid = mySideBar->myMusicListWid->musicInfoWidget->itemWidget
            (mySideBar->myMusicListWid->musicInfoWidget->item
            (mySideBar->myMusicListWid->currentPlayIndex));
    QList<QLabel*> songItemList = itemWid->findChildren<QLabel*>();
    for(int i = 0; i < songItemList.count(); i++)
    {
        songItemList[i]->setStyleSheet("width: 182px;\
                                       height: 14px;\
                                       font-size: 14px;\
                                       font-weight: 400;\
                                       color: #3790FA;\
                                       line-height: 14px;");
    }

    skipPlayHighlight = false;
}


void MainWid::updataplaylistwidget(int value)//更新playlistWidget
{
    if (skipPlayHighlight)
    {
        return ;
    }
    mySideBar->musicListChangeWid[mySideBar->btnIndex]->musicInfoWidget->item(value)->setSelected(true);
    if(mySideBar->musicListChangeWid[mySideBar->btnIndex]->currentPlayIndex != -1)
    {
//        lastPlayIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
        QWidget* lastitemWid = mySideBar->musicListChangeWid[mySideBar->btnIndex]->musicInfoWidget->itemWidget
                (mySideBar->musicListChangeWid[mySideBar->btnIndex]->musicInfoWidget->item
                (mySideBar->musicListChangeWid[mySideBar->btnIndex]->currentPlayIndex));
        QList<QLabel*> lastsongItemList = lastitemWid->findChildren<QLabel*>();
        for(int i = 0; i < lastsongItemList.count(); i++)
        {
            lastsongItemList[i]->setStyleSheet("width: 182px;\
                                               height: 14px;\
                                               font-size: 14px;\
                                               font-weight: 400;\
                                               color:#303133;\
                                               line-height: 14px;");
        }
    }

    mySideBar->musicListChangeWid[mySideBar->btnIndex]->currentPlayIndex = mySideBar->musicListChangeWid[mySideBar->btnIndex]->PlayList->currentIndex();

    QWidget* itemWid = mySideBar->musicListChangeWid[mySideBar->btnIndex]->musicInfoWidget->itemWidget
            (mySideBar->musicListChangeWid[mySideBar->btnIndex]->musicInfoWidget->item
            (mySideBar->musicListChangeWid[mySideBar->btnIndex]->currentPlayIndex));
    QList<QLabel*> songItemList = itemWid->findChildren<QLabel*>();
    for(int i = 0; i < songItemList.count(); i++)
    {
        songItemList[i]->setStyleSheet("width: 182px;\
                                       height: 14px;\
                                       font-size: 14px;\
                                       font-weight: 400;\
                                       color: #3790FA;\
                                       line-height: 14px;");
    }
}

void MainWid::updateSongPlaying()
{
    if (mySideBar->currentSelectList == -1)
    {
        int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();

        QString mp3Name = model->data(model->index(row, 1)).toString();
        myPlaySongArea->songNameofNowPlaying->setText(mp3Name);  //正在播放

        //迷你模式正在播放
        m_MiniWidget->m_songNameLab->setText(mp3Name);
    }
    else if (mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->state() == QMediaPlayer::PlayingState)
    {
        int row = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->currentIndex().row();

        QString mp3Name = model_1->data(model_1->index(row, 1)).toString();
        myPlaySongArea->songNameofNowPlaying->setText(mp3Name);  //正在播放
        //迷你模式正在播放

        m_MiniWidget->m_songNameLab->setText(mp3Name);
    }
}

void MainWid::on_listWidget_doubleClicked(QListWidgetItem *item)//双击本地音乐播放playlist
{
    QPixmap default_photo = QPixmap(":/img/fengmian.png");
    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
    m_MiniWidget->m_coverLabel->setPixmap(default_photo);

//    qDebug()<<"currentSelectList : "<<mySideBar->currentSelectList;
//    qDebug()<<"currentPlayList : "<<mySideBar->currentPlayList;

//    if (mySideBar->currentPlayList != -1 && mySideBar->currentPlayList != -2) {
//        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
//    }
//    if (mySideBar->myMusicListWid->Music->playlist() != mySideBar->myMusicListWid->PlayList) {
//        mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
//    }
    mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
    mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row());

    connect(mySideBar->myMusicListWid->PlayList, &QMediaPlaylist::currentIndexChanged, this,&MainWid::currentPlayHighlight);
    //    mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->row(item));

    model->setTable("LocalMusic");
    model->select();
    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
    int musicid = mySideBar->myMusicListWid->localAllMusicid[row].toInt();
    QString mp3Name = model->data(model->index(musicid, 1)).toString();
//    myPlaySongArea->songNameofNowPlaying->setText(mp3Name);  //正在播放
    QString mp3Name1 = model->data(model->index(musicid, 1)).toString();
    //迷你模式正在播放
    m_MiniWidget->m_songNameLab->setText(mp3Name);

    QString artistStr = model->data(model->index(musicid, 3)).toString();
//    QString albumStr = model->data(model->index(row, 4)).toString();
    QString timeStr   = model->data(model->index(musicid, 7)).toString();
    QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
    HistoryListItem *besongitem1 = new HistoryListItem;
    myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);

    qDebug()<<"双击 mp3Name : "<<mp3Name;
    myPlaySongArea->songText(mp3Name); // 正在播放

    besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
    besongitem1->songTimeLabel->setText(timeStr);

    QSqlQuery query;
    query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
               .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
    qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
              .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
    local_Music();  //判断本地播放列表中歌曲是否存在
    mySideBar->myMusicListWid->isStartPlay = true;
//    TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//    qDebug()<<"local_Musiclocal_Musiclocal_Musiclocal_Music";
//    TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//    qDebug()<<"local_Musiclocal_Musiclocal_Musiclocal_Music";
//    if (!frameList.isEmpty())
//    {
//        TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//        QPixmap photo;
//        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//        photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//        myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//        m_MiniWidget->m_coverLabel->setPixmap(photo);

//        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//    }
//    qDebug()<<"local_Musiclocal_Musiclocal_Musiclocal_Music";
}

// 双击歌单播放
void MainWid::on_musicListChangeWid_doubleClicked(QListWidgetItem *item)
{
    QPixmap default_photo = QPixmap(":/img/fengmian.png");
    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
    m_MiniWidget->m_coverLabel->setPixmap(default_photo);

    qDebug() << "双击歌单播放";

    mySideBar->myMusicListWid->Music->stop();
    qDebug() << "mySideBar->currentPlayList : " << mySideBar->currentPlayList;
    qDebug() << "mySideBar->currentSelectList : " << mySideBar->currentSelectList;

//    if (mySideBar->currentPlayList != -1 && mySideBar->currentPlayList != -2) {
//        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
//    }

//    mySideBar->currentPlayList = mySideBar->currentSelectList;

//    if (mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->playlist() !=
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList) {
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->setPlaylist(
//        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList);
//    }
    if (mySideBar->currentPlayList >= 0)
        if (mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->state() == QMediaPlayer::PlayingState)
            mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();

    mySideBar->currentPlayList = mySideBar->currentSelectList;


    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->setPlaylist(
                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList);

    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(
                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->currentIndex().row());
    //    mySideBar->myMusicListWid->PlayList->setCurrentIndex(mySideBar->myMusicListWid->musicInfoWidget->row(item));
//    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();

    int tempmusicrow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->currentIndex().row();
    int templistrow = mySideBar->currentSelectList;
    QString tempAllId = model_1->data(model_1->index(templistrow,1)).toString();
    int tempid = tempAllId.split(",")[tempmusicrow].toInt();
    qDebug()<<"双击 tempid : "<<tempid;
    int row = tempid;
    qDebug()<<"row : "<<row;

    int thisrow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();
    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(thisrow);
    mySideBar->sidemodel->setTable("NewPlayList");
    mySideBar->sidemodel->select();
    QString allmusicid = mySideBar->sidemodel->data(mySideBar->sidemodel->index(mySideBar->currentSelectList,1)).toString();
    int thisid = allmusicid.split(",")[thisrow].toInt();
    QStringList thismusicinfo = mySideBar->myMusicListWid->get_info_from_db(thisid);
    QString mp3Name   = thismusicinfo[0];
    QString mp3Name1  = thismusicinfo[0];
    QString artistStr = thismusicinfo[2];
    QString timeStr   = thismusicinfo[6];

    QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
    HistoryListItem *besongitem1 = new HistoryListItem;
    myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
    myPlaySongArea->songText(mp3Name); // 正在播放
    besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
    besongitem1->songTimeLabel->setText(timeStr);
    //迷你模式正在播放
    m_MiniWidget->m_songNameLab->setText(mp3Name);

    QSqlQuery query;
    query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
               .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
    qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
              .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));

    new_PlayList();
//    mySideBar->currentPlayList = 1;    // jialin

    qDebug() << "双击歌单播放 " << mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();

//    TagLib::MPEG::File fx(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//    TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//    if (!frameList.isEmpty())
//    {
//        TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//        QPixmap photo;
//        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//        photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//        myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//        m_MiniWidget->m_coverLabel->setPixmap(photo);

//        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
//    }
}

void MainWid::Music_stateChang(QMediaPlayer::State state)//播放状态改变
{

    if(state==QMediaPlayer::PlayingState)
    {
//        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        //迷你模式播放按钮
//        m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/pause2.png"));
        m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
        m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

//         myPlaySongArea->playBtn->setToolTip(tr("pause"));

    }
    else
    {
//        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        //迷你模式播放按钮
//        m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/play2.png"));
        m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
        m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

//        myPlaySongArea->playBtn->setToolTip(tr("play"));


    }
    //    updateSongPlaying();
}

void MainWid::Music_playlist_stateChang(QMediaPlayer::State state)//播放状态改变
{
    if(state==QMediaPlayer::PlayingState)
    {
//        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        //迷你模式播放按钮
//        m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/pause2.png"));
        m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
        m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
//        myPlaySongArea->playBtn->setToolTip(tr("pause"));
    }
    else
    {
//        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/play2.png"));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        //迷你模式播放按钮
//        m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/pause2.png"));
        m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
        m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
//        myPlaySongArea->playBtn->setToolTip(tr("play"));
    }
}


void MainWid::on_lastBtn_clicked()             //上一首
{
    if (mySideBar->currentSelectList == -1)
    {
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
            QPixmap default_photo = QPixmap(":/img/fengmian.png");
            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
            m_MiniWidget->m_coverLabel->setPixmap(default_photo);
            //        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();

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
                int musicid = mySideBar->myMusicListWid->localAllMusicid[currentIndex].toInt();
                QString mp3Name = model->data(model->index(musicid, 1)).toString();
                QString mp3Name1 = model->data(model->index(musicid, 1)).toString();
                QString artistStr = model->data(model->index(musicid, 3)).toString();
                QString timeStr   = model->data(model->index(musicid, 7)).toString();
                QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
                HistoryListItem *besongitem1 = new HistoryListItem;
                myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
                myPlaySongArea->songText(mp3Name); // 正在播放

                besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
                besongitem1->songTimeLabel->setText(timeStr);
                //迷你模式正在播放
                m_MiniWidget->m_songNameLab->setText(mp3Name);

                QSqlQuery query;
                query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                           .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
                qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                          .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
                myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                            tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
            }
            //        mySideBar->myMusicListWid->Music->play();

            local_Music();
        }

        //上一首时歌曲封面输出
//        TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//        TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//        if (!frameList.isEmpty())
//        {
//            TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//            QPixmap photo;
//            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
//            photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//    //        m_MiddleWidget->m_PlaySongInfoWidget->cdLabel->setPixmap(photo);
//            myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//            m_MiniWidget->m_coverLabel->setPixmap(photo);
//    //        photo = photo.scaled(42,42,Qt::KeepAspectRatio);
//    //        m_MiniWidget->m_photoLabel->setPixmap(photo);
//            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
//    //        m_turntableWidget->pix_cd = photo; m_turntableWidget->update();
//        }


    }
    else if (mySideBar->currentPlayList >= 0)
    {
        if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count() > 0)
        {
            QPixmap default_photo = QPixmap(":/img/fengmian.png");
            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
            m_MiniWidget->m_coverLabel->setPixmap(default_photo);
            mySideBar->myMusicListWid->Music->stop();

            if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->playlist()==mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList)
            {
                int currentIndex = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();
                //        mpwid->setCurrentIndex(0);
                //        pushbuttonstyle();
                if(--currentIndex < 0)
                {
                    currentIndex = 0;
                }
                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(currentIndex);

                int templistrow = mySideBar->currentSelectList;
                QString tempAllId = model_1->data(model_1->index(templistrow,1)).toString();
                qDebug()<<"tempAllId : "<<tempAllId;
                QStringList list = tempAllId.split(",");
                qDebug()<<"list : "<<list<<"长度："<<list.count();
                for(int i = 0;i < list.count()-1;i++)
                {
                    if(currentIndex == i)
                    {
                        QString listlist = list.at(currentIndex);
                        qDebug()<<"listlist : "<<listlist;
                        int row = listlist.toInt();
                        QString mp3Name = model->data(model->index(row, 1)).toString();
                        QString mp3Name1 = model->data(model->index(row, 1)).toString();
                        QString artistStr = model->data(model->index(row, 3)).toString();
                        QString timeStr   = model->data(model->index(row, 7)).toString();
                        QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
                        HistoryListItem *besongitem1 = new HistoryListItem;
                        myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
                        myPlaySongArea->songText(mp3Name); // 正在播放

                        besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
                        besongitem1->songTimeLabel->setText(timeStr);
                        //迷你模式正在播放
                        m_MiniWidget->m_songNameLab->setText(mp3Name);

                        QSqlQuery query;
                        query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                                   .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
                        qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                                  .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
                        myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                                    tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
                    }
                }
            }
    //        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();

            new_PlayList();

//            TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//            TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//            if (!frameList.isEmpty())
//            {
//                TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//                QPixmap photo;
//                photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
//                photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//        //        m_MiddleWidget->m_PlaySongInfoWidget->cdLabel->setPixmap(photo);
//                myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//                m_MiniWidget->m_coverLabel->setPixmap(photo);
//        //        photo = photo.scaled(42,42,Qt::KeepAspectRatio);
//        //        m_MiniWidget->m_photoLabel->setPixmap(photo);
//                photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
//        //        m_turntableWidget->pix_cd = photo; m_turntableWidget->update();
//                qDebug()<<"cxz";
//            }
        }
    }
    //    updateSongPlaying();
}


void MainWid::on_nextBtn_clicked()      //下一首
{
    if (mySideBar->currentSelectList == -1)
    {
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
            QPixmap default_photo = QPixmap(":/img/fengmian.png");
            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
            m_MiniWidget->m_coverLabel->setPixmap(default_photo);

    //        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();

            if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
            {

                int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
                if(++currentIndex == mySideBar->myMusicListWid->PlayList->mediaCount())
                {
                    currentIndex = 0;
                }
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
                int musicid = mySideBar->myMusicListWid->localAllMusicid[currentIndex].toInt();
                QString mp3Name = model->data(model->index(musicid, 1)).toString();
                QString mp3Name1 = model->data(model->index(musicid, 1)).toString();
                QString artistStr = model->data(model->index(musicid, 3)).toString();
                QString timeStr   = model->data(model->index(musicid, 7)).toString();
                QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
                HistoryListItem *besongitem1 = new HistoryListItem;
                myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
                myPlaySongArea->songText(mp3Name); // 正在播放

                besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
                besongitem1->songTimeLabel->setText(timeStr);
                //迷你模式正在播放
                m_MiniWidget->m_songNameLab->setText(mp3Name);

                QSqlQuery query;
                query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                           .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
                qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                          .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
                myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                            tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
            }
    //        mySideBar->myMusicListWid->Music->play();
            local_Music();

            //下一首时封面输出
//            TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//            TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//            if (!frameList.isEmpty())
//            {
//                TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//                QPixmap photo;
//                photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//                photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//                myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//                m_MiniWidget->m_coverLabel->setPixmap(photo);

//                photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//            }
        }

    }

    else if (mySideBar->currentPlayList >= 0)
    {
        if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count() > 0)
        {
            QPixmap default_photo = QPixmap(":/img/fengmian.png");
            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
            m_MiniWidget->m_coverLabel->setPixmap(default_photo);

    //        mySideBar->myMusicListWid->Music->stop();

            if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->playlist() ==
                    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList)
            {
                int currentIndex=mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();

                if(++currentIndex==mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->mediaCount())
                {
                    currentIndex = 0;
                }
                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(currentIndex);

                int templistrow = mySideBar->currentSelectList;
                QString tempAllId = model_1->data(model_1->index(templistrow,1)).toString();
                QStringList list = tempAllId.split(",");
                for(int i = 0;i < list.count()-1;i++)
                {
                    if(currentIndex == i)
                    {
                        QString listlist = list.at(currentIndex);
                        qDebug()<<"listlist : "<<listlist;
                        int row = listlist.toInt();
                        QString mp3Name = model->data(model->index(row, 1)).toString();
                        QString mp3Name1 = model->data(model->index(row, 1)).toString();
                        QString artistStr = model->data(model->index(row, 3)).toString();
                        QString timeStr   = model->data(model->index(row, 7)).toString();
                        QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
                        HistoryListItem *besongitem1 = new HistoryListItem;
                        myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
                        myPlaySongArea->songText(mp3Name); // 正在播放

                        besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
                        besongitem1->songTimeLabel->setText(timeStr);
                        //迷你模式正在播放
                        m_MiniWidget->m_songNameLab->setText(mp3Name);

                        QSqlQuery query;
                        query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                                   .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
                        qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
                                  .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
                        myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                                    tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
                    }
                }
            }
    //        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
            new_PlayList();
    //        TagLib::MPEG::File fx(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
    //        TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
    //        if (!frameList.isEmpty())
    //        {
    //            TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
    //            QPixmap photo;
    //            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

    //            photo = photo.scaled(40,40,Qt::KeepAspectRatio);
    //            myPlaySongArea->coverPhotoLabel->setPixmap(photo);
    //            m_MiniWidget->m_coverLabel->setPixmap(photo);

    //            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

    //        }
        }
    }
    //    updateSongPlaying();
}

void MainWid::positionChange(qint64 position)    //更新播放位置
{
    if (mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PlayingState)
    {
//        hSlider->setMaximum(mySideBar->myMusicListWid->Music->duration() / 1000);//设置滑块的长度范围为音乐长.00度
        hSlider->setValue(static_cast<int>(position));
//        hSlider->setValue(position / 1000);//如果音乐进度改变，则改变滑块显示位置
//        moved = position;//让播放时间等于音乐进度（毫秒）
        QTime duration(0, static_cast<int>(position) / 60000, static_cast<int>((position % 60000) / 1000.0));
//        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
        QString str_time = duration.toString("mm:ss");
        QTime totalTime(0,(mySideBar->myMusicListWid->Music->duration()/60000) % 60,
                       (mySideBar->myMusicListWid->Music->duration() / 1000) % 60);
        myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

        //迷你模式时间输出
        m_MiniWidget->m_timeLab->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

        // 判断顺序播放模式下最后一首歌，播放完停止播放
        if (mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Sequential)
        {

            if (mySideBar->myMusicListWid->PlayList->currentIndex() == mySideBar->myMusicListWid->PlayList->mediaCount() - 1)
            {

                if (mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PlayingState)
                {
                    if (mySideBar->myMusicListWid->Music->duration() > 0)
                    {
//                        if (position > mySideBar->myMusicListWid->Music->duration() - 1000)
//                        {
//                            qDebug()<<"列表播放结束 停止" ;
//                            mySideBar->myMusicListWid->Music->stop();
//                            position = 0;
//                        }
                        if (str_time == totalTime.toString("mm:ss")&&str_time != "00:00")
                        {
                            qDebug()<<"列表播放结束 停止" ;
                            mySideBar->myMusicListWid->Music->stop();
                            position = 0;
                            hSlider->setValue(position);
                            myPlaySongArea->bottomLeftLabel->setText(tr("00:00") + "/" + totalTime.toString("mm:ss"));
                        }
                    }
                }
            }
        }
    }
}

void MainWid::playlist_positionChange(qint64 position)
{
    if (mySideBar->currentPlayList >= 0)
    {
//        hSlider->setMaximum(mySideBar->myMusicListWid->Music->duration() / 1000);//设置滑块的长度范围为音乐长.00度
        hSlider->setValue(static_cast<int>(position));
//        hSlider->setValue(position / 1000);//如果音乐进度改变，则改变滑块显示位置
//        moved = position;//让播放时间等于音乐进度（毫秒）
        QTime duration(0, static_cast<int>(position) / 60000, static_cast<int>((position % 60000) / 1000.0));
//        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
        QString str_time = duration.toString("mm:ss");
        QTime totalTime(0,(mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->duration()/60000) % 60,
                       (mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->duration() / 1000) % 60);
        myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间
        //迷你模式时间输出
        m_MiniWidget->m_timeLab->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

        // 判断顺序播放模式下最后一首歌，播放完停止播放
        if (mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->playbackMode() == QMediaPlaylist::Sequential)
        {
            if (mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->currentIndex() ==
                    mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->mediaCount() - 1)
            {
                if (mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->state() == QMediaPlayer::PlayingState)
                {
                    if (mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->duration() > 0)
                    {
//                        if (position > mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->duration() - 1000)
//                        {
//                            qDebug()<<"列表播放结束 停止" ;
//                            mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
//                            position = 0;
//                        }
                        if (str_time == totalTime.toString("mm:ss")&&str_time != "00:00")
                        {
                            qDebug()<<"列表播放结束 停止" ;
                            mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->stop();
                            position = 0;
                            hSlider->setValue(position);
                            myPlaySongArea->bottomLeftLabel->setText(tr("00:00") + "/" + totalTime.toString("mm:ss"));
                        }
                    }
                }
            }
        }
    }
}

void MainWid::playlist_durationChange(qint64 duration)
{
    if(mySideBar->currentSelectList >= 0)
    {
        hSlider->setRange(0,static_cast<int>(duration));
        hSlider->setEnabled(duration>0);
        hSlider->setPageStep(static_cast<int>(duration)/10);
    }
}

void MainWid::durationChange(qint64 duration)   //更新播放进度
{
    hSlider->setRange(0,static_cast<int>(duration));
    hSlider->setEnabled(duration>0);
    hSlider->setPageStep(static_cast<int>(duration)/10);
}

void MainWid::currentMediaChanged(QMediaContent content)
{
    Q_UNUSED(content);

    int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
    qDebug()<<"playMode : "<<playMode;
    if(playMode == 0)
    {
        if(currentIndex != -1)
        {
            int music = mySideBar->myMusicListWid->localAllMusicid[currentIndex].toInt();
            QPixmap default_photo = QPixmap(":/img/fengmian.png");
            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
            m_MiniWidget->m_coverLabel->setPixmap(default_photo);
            QString mp3Name = model->data(model->index(music, 1)).toString();
            //迷你模式正在播放
            m_MiniWidget->m_songNameLab->setText(mp3Name);
            myPlaySongArea->songText(mp3Name); // 正在播放
            model->setTable("LocalMusic");
            model->select();
            //            QString name = model->data(model->index(music,1)).toString();
            QString mp3Path = model->data(model->index(music, 2)).toString();
            //            qDebug()<< "name :  " <<name;
            //            myPlaySongArea->songText(name); // 正在播放
            qDebug()<<"mp3Path : "<<mp3Path;
            QFileInfo file(mp3Path);
            if(file.exists())
            {
                mySideBar->myMusicListWid->Music->play();
            }
            else
            {
                qDebug()<<"mp3Path is not exists !";
//                promptMessage();
                if (mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Sequential)
                {
                    if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
                    {
                        if(currentIndex == mySideBar->myMusicListWid->musicInfoWidget->count() - 1)
                        {

                            mySideBar->myMusicListWid->Music->stop();
                            mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
                            QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
                            mySideBar->myMusicListWid->Music->play();
                            qDebug()<< "mp3Name :  " <<mp3Name;
                            myPlaySongArea->songText(mp3Name); // 正在播放
                            m_MiniWidget->m_songNameLab->setText(mp3Name);
                        }
                        else
                        {
                            qDebug()<< "mp3Name :   :   :   :  ";

                            //                            QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("警告"),tr("暂无法打开文件，请检查路径和文件名称"),QMessageBox::Yes);
                            //                            warn->button(QMessageBox::Yes)->setText("确定");
                            //                            warn->exec();
//                            promptMessage();
                            qDebug()<<"mp3Namemp3Name";
                            mySideBar->myMusicListWid->Music->stop();
                            QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
                            mySideBar->myMusicListWid->Music->play();
                            myPlaySongArea->songText(mp3Name); // 正在播放
                            QPixmap default_photo = QPixmap(":/img/fengmian.png");
                            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
                            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
                            m_MiniWidget->m_coverLabel->setPixmap(default_photo);
                        }
                    }
                }
            }
        }
//        TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//        TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//        if (!frameList.isEmpty())
//        {
//            TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//            QPixmap photo;
//            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//            photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//            myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//            m_MiniWidget->m_coverLabel->setPixmap(photo);

//            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
//        }
    }
    else if(playMode == 1)
    {
        int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
        QPixmap default_photo = QPixmap(":/img/fengmian.png");
        default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
        myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
        m_MiniWidget->m_coverLabel->setPixmap(default_photo);

        model->setTable("LocalMusic");
        model->select();
        int musicid = mySideBar->myMusicListWid->localAllMusicid[currentIndex].toInt();
        QString mp3Name = model->data(model->index(musicid, 1)).toString();

        //迷你模式正在播放
        m_MiniWidget->m_songNameLab->setText(mp3Name);
        myPlaySongArea->songText(mp3Name); // 正在播放
//        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
//        mySideBar->myMusicListWid->Music->play();
//        myPlaySongArea->playBtn->setIcon(QIcon(":/img/default/pause2.png"));
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
//        myPlaySongArea->playBtn->setIconSize(myPlaySongArea->lastBtn->size());
//        myPlaySongArea->bottomLeftLabel->setText(model->data(model->index(musicid,7)).toString());

        TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
        TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
        if (!frameList.isEmpty())
        {
            TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
            QPixmap photo;
            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

            photo = photo.scaled(40,40,Qt::KeepAspectRatio);
            myPlaySongArea->coverPhotoLabel->setPixmap(photo);
            m_MiniWidget->m_coverLabel->setPixmap(photo);

            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
        }
    }
    else if(playMode == 2)
    {
        int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
        int music = mySideBar->myMusicListWid->localAllMusicid[currentIndex].toInt();
        QPixmap default_photo = QPixmap(":/img/fengmian.png");
        default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
        myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
        m_MiniWidget->m_coverLabel->setPixmap(default_photo);
        QString mp3Name = model->data(model->index(music, 1)).toString();
        //迷你模式正在播放
        m_MiniWidget->m_songNameLab->setText(mp3Name);
        myPlaySongArea->songText(mp3Name); // 正在播放
        model->setTable("LocalMusic");
        model->select();
        if(currentIndex != -1)
        {
            qDebug()<<"currentIndex123 : "<<currentIndex;
//            QString name = model->data(model->index(music,1)).toString();
            QString mp3Path = model->data(model->index(music, 2)).toString();
//            qDebug()<< "name :  " <<name;
//            myPlaySongArea->songText(name); // 正在播放
            qDebug()<<"mp3Path : "<<mp3Path;
            QFileInfo file(mp3Path);
            if(file.exists())
            {
                mySideBar->myMusicListWid->Music->play();
            }
            else
            {
                qDebug()<<"mp3Path is not exists !";
//                promptMessage();
                if (mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Sequential)
                {
                    if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
                    {
                        if(currentIndex == mySideBar->myMusicListWid->musicInfoWidget->count() - 1)
                        {

                            mySideBar->myMusicListWid->Music->stop();
                            mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
                            QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
                            mySideBar->myMusicListWid->Music->play();
                            qDebug()<< "mp3Name :  " <<mp3Name;
                            myPlaySongArea->songText(mp3Name); // 正在播放
                            m_MiniWidget->m_songNameLab->setText(mp3Name);
                        }
                        else
                        {
                            mySideBar->myMusicListWid->Music->stop();
                            QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
                            mySideBar->myMusicListWid->Music->play();
                            myPlaySongArea->songText(mp3Name); // 正在播放
                            QPixmap default_photo = QPixmap(":/img/fengmian.png");
                            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
                            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
                            m_MiniWidget->m_coverLabel->setPixmap(default_photo);
                        }
                    }
                }
            }
        }
    }
    return;
}

void MainWid::slidePress() //滑动条鼠标按下
{
    if (mySideBar->currentSelectList == -1)
    {
        mySideBar->myMusicListWid->Music->pause();
    }
    else if (mySideBar->currentPlayList >= 0)
    {
        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->pause();
    }
}



void MainWid::slideRelease()   //滑动条鼠标弹起
{
    if (mySideBar->currentSelectList == -1)
    {
        mySideBar->myMusicListWid->Music->play();
    }
    else if (mySideBar->currentPlayList >= 0)
    {
        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->play();
    }
}

//void MainWid::playlist_slideRelease()
//{
//    mySideBar->musicListChangeWid[mySideBar->btnIndex]->Music->play();
//}

void MainWid::setPosition(int position)
{
    if (mySideBar->currentSelectList == -1)
    {
        if (qAbs(mySideBar->myMusicListWid->Music->position() - position) > 99)
            mySideBar->myMusicListWid->Music->setPosition(position);
    }
    else if (mySideBar->currentPlayList >= 0)
    {
        if (qAbs(mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->position() - position) > 99)
            mySideBar->musicListChangeWid[mySideBar->currentPlayList]->Music->setPosition(position);
    }
}

bool MainWid::eventFilter(QObject *obj, QEvent *event)   //鼠标滑块点击
{
    if(obj == hSlider)
    {
        if (event->type()==QEvent::MouseButtonPress)           //判断类型
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
            if (mouseEvent->button() == Qt::LeftButton) //判断左键
            {
               int dur = hSlider->maximum() - hSlider->minimum();
               int pos = hSlider->minimum() + dur * ((double)mouseEvent->x() / hSlider->width());
               if(pos != hSlider->sliderPosition())
                {
                  hSlider->setValue(pos);
                }
            }
        }
    }
    return QObject::eventFilter(obj,event);
}

void MainWid::get_historyplaylist_information()
{
    QSqlQuery query;
    query.exec("select * from HistoryPlayList");  //历史列表数据
    while(query.next())
    {
//        qDebug() << "select * from LocalMusic " << query.value(1).toString() << query.value(2).toString()
//                 << query.value(3).toString() << query.value(4).toString() << query.value(7).toString();

        QString Name = query.value(1).toString();
        QString singer = query.value(2).toString();
        QString Time = query.value(3).toString();
        if (Name == "")
        {
            continue;
        }
        // 添加到本地音乐所有id中
//        localAllMusicid.append(QString::number(Id));

        if(Name != ""&&singer != "")
        {
            QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
            HistoryListItem *besongitem1 = new HistoryListItem;
            myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);

//            besongitem1->songNameLabel->setText(Name); //歌曲名称
//            besongitem1->singerLabel->setText(singer); //歌手
//            besongitem1->songTimeLabel->setText(Time); //时长
            besongitem1->song_singerText(Name,singer); //历史列表
            besongitem1->songTimeLabel->setText(Time); //时长

//            PlayList->addMedia(QUrl::fromLocalFile(Name));
        }

    }
    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
//    qDebug()<<"allmusic.size() :"<<allmusic.size();

}

void MainWid::PlayModeChanged()
{
    if (mySideBar->currentSelectList == -1)
    {
        switch (playMode)
        {
        case 0:
            ++playMode;
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one"));
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/oneloop.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/oneloop.png);}");
            myPlaySongArea->playModeBtn->setToolTip(tr(" Single cycle "));  //单曲循环
            m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-only"));
            m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/oneloop.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/oneloop.png);}");
            m_MiniWidget->playModeMenu->setTitle(tr(" Single cycle "));
            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
            qDebug()<<"CurrentItemInLoop :"<<playMode;
            break;

        case 1:
            ++playMode;
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/listloop.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/listloop.png);}");
            myPlaySongArea->playModeBtn->setToolTip(tr(" A list of circulation "));  //列表循环
            m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
            m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/listloop.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/listloop.png);}");
            m_MiniWidget->playModeMenu->setTitle(tr(" A list of circulation "));
            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::Loop);
            qDebug()<<"Loop :"<<playMode;
            break;

        case 2:
            ++playMode;
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/random.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/random.png);}");
            myPlaySongArea->playModeBtn->setToolTip(tr(" Random broadcast "));  // 随机播放
            m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
            m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/random.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/random.png);}");
            m_MiniWidget->playModeMenu->setTitle(tr(" Random broadcast "));
            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::Random);
            qDebug()<<"Random :"<<playMode;
            break;

       case 3:
            playMode = 0;
            myPlaySongArea->playModeBtn->setFixedSize(16,16);
            myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
            myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
            myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/sequence.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");
            myPlaySongArea->playModeBtn->setToolTip(tr(" Order of play "));  //顺序播放
            m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
            m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/sequence.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");
            m_MiniWidget->playModeMenu->setTitle(tr(" Order of play "));
            mySideBar->myMusicListWid->PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
            qDebug()<<"Sequential :"<<playMode;
        default:
            break;
        }
    }
    else if (mySideBar->currentPlayList >= 0)
    {
        switch (playMode)
        {
            case 0:
                ++playMode;
                myPlaySongArea->playModeBtn->setFixedSize(16,16);
                myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
                myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat-one"));
                myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                           "QPushButton::hover{border-image:url(:/img/hover/oneloop.png);}"
                                                           "QPushButton::pressed{border-image:url(:/img/clicked/oneloop.png);}");
                myPlaySongArea->playModeBtn->setToolTip(tr(" Single cycle "));  //单曲循环
                m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-only"));
                m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/oneloop.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/oneloop.png);}");
                mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
                break;

            case 1:
                ++playMode;
                myPlaySongArea->playModeBtn->setFixedSize(16,16);
                myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
                myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
                myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                           "QPushButton::hover{border-image:url(:/img/hover/listloop.png);}"
                                                           "QPushButton::pressed{border-image:url(:/img/clicked/listloop.png);}");
                myPlaySongArea->playModeBtn->setToolTip(tr(" A list of circulation "));  //列表循环
                m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
                m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/listloop.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/listloop.png);}");
                mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::Loop);
                break;

            case 2:
                ++playMode;
                myPlaySongArea->playModeBtn->setFixedSize(16,16);
                myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
                myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
                myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                           "QPushButton::hover{border-image:url(:/img/hover/random.png);}"
                                                           "QPushButton::pressed{border-image:url(:/img/clicked/random.png);}");
                myPlaySongArea->playModeBtn->setToolTip(tr(" Random broadcast "));  // 随机播放
                m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
                m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/random.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/random.png);}");
                mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::Random);
                break;

           case 3:
                playMode = 0;
                myPlaySongArea->playModeBtn->setFixedSize(16,16);
                myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
                myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
                myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                           "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}"
                                                           "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");
                myPlaySongArea->playModeBtn->setToolTip(tr(" Order of play "));   //顺序播放
                m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
                m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                        "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");
                mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
                break;
            default:
                break;
        }
    }
}


void MainWid::initSystemTray()
{
    connect(m_MiniWidget->playMode_Loop_Action,&QAction::triggered,[&]()
    {
        playMode = 0;
        myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
        myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                   "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");
        myPlaySongArea->playModeBtn->setToolTip(tr(" Order of play "));
        m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-repeat"));
        m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                "QPushButton::hover{border-image:url(:/img/clicked/sequence.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/sequence.png);}");
        m_MiniWidget->playModeMenu->setTitle(tr(" Order of play "));
        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::Loop);
    });
    connect(m_MiniWidget->playMode_Random_Action,&QAction::triggered,[&]()
    {
        playMode = 1;
        myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
        myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-only"));
        myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/oneloop.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/oneloop.png);}");
        myPlaySongArea->playModeBtn->setToolTip(tr(" Single cycle "));  //单曲循环
        m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-only"));
        m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                "QPushButton::hover{border-image:url(:/img/hover/oneloop.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/oneloop.png);}");
        m_MiniWidget->playModeMenu->setTitle(tr(" Single cycle "));
        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::Random);
    });
    connect(m_MiniWidget->playMode_Sequential_Action,&QAction::triggered,[&]()
    {
        playMode = 2;
        myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
        myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
        myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/listloop.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/listloop.png);}");
        myPlaySongArea->playModeBtn->setToolTip(tr(" A list of circulation "));  //列表循环
        m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("mail-send-receive-symbolic"));
        m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                "QPushButton::hover{border-image:url(:/img/hover/listloop.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/listloop.png);}");
        m_MiniWidget->playModeMenu->setTitle(tr(" A list of circulation "));
        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
    });
    connect(m_MiniWidget->playMode_CurrentItemInLoop_Action,&QAction::triggered,[&]()
    {
        playMode = 3;
        myPlaySongArea->playModeBtn->setCursor(Qt::PointingHandCursor);
        myPlaySongArea->playModeBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
        myPlaySongArea->playModeBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/random.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/random.png);}");
        myPlaySongArea->playModeBtn->setToolTip(tr(" Random broadcast "));  // 随机播放
        m_MiniWidget->m_orderBtn->setIcon(QIcon::fromTheme("media-playlist-shuffle"));
        m_MiniWidget->m_orderBtn->setStyleSheet("QPushButton{background:transparent;}"
                                                "QPushButton::hover{border-image:url(:/img/hover/random.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/random.png);}");
        m_MiniWidget->playModeMenu->setTitle(tr(" Random broadcast "));
        mySideBar->musicListChangeWid[mySideBar->currentPlayList]->PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    });
}

//显示历史播放列表
void MainWid::showBeforeList()
{
    if((myPlaySongArea->listBtn->isChecked()) == true)
    {
        //历史播放列表输出
        myPlaySongArea->mybeforeList->show();
    }
    else
    {
        myPlaySongArea->mybeforeList->hide();
    }
}

void MainWid::show_volumeBtn()
{
    if(vSliderWid->isHidden())
    {
        vSliderWid->show();
    }
    else
    {
        vSliderWid->hide();
    }
}

void MainWid::changeVolume(int values)
{
    if(values == 0)
    {
        myPlaySongArea->volumeBtn->setIcon(QIcon(":/img/clicked/vol_close.png"));
    }
    else if(values > 0&&values <= 50)
    {
        myPlaySongArea->volumeBtn->setIcon(QIcon(":/img/clicked/vol_small.png"));
    }
    else
    {
        myPlaySongArea->volumeBtn->setIcon(QIcon(":/img/clicked/vol_large.png"));
    }
    mySideBar->myMusicListWid->Music->setVolume(values);
}

void MainWid::mousePressEvent(QMouseEvent *event)
{
    vSlider->hide();
}

void MainWid::addvSlider_slot()
{

    vSlider->show();
    int values = vSlider->value();
    if(values < 100)
    {
        values++;
        vSlider->setValue(values);
    }
}
void MainWid::subvSlider_slot()
{
    vSlider->show();
    int values = vSlider->value();
    if(values > 0)
    {

        values--;
        vSlider->setValue(values);
    }
}

void MainWid::showAboutWidget()
{
    qDebug() << "关于";
//    myTitleBar->aboutWidget->raise();

//    myTitleBar->aboutWidget->show();
    aboutWidget = new AllPupWindow(this);
    aboutWidget->resize(420,300);
    aboutWidget->cancelBtn->hide();

//    aboutWidget->hide();
    aboutWidget->pupDialog->show();



}

void MainWid::changeDarkTheme()
{
    WidgetStyle::themeColor = 1;
    mySideBar->newSonglistPup->dlgcolor();
    mySideBar->renameSongListPup->dlgcolor();
    mySideBar->sidecolor();
    myPlaySongArea->playcolor();
    m_MiniWidget->minicolor();
    myTitleBar->titlecolor();
    mySideBar->myMusicListWid->musicInfoWidget->clear();     //主题切换清理musicInfoWidget引起闪退
    mySideBar->myMusicListWid->get_localmusic_information();
    nullMusicWidget->nullWidgetColor();
    mySideBar->myMusicListWid->musiclistcolor();
    for (int i = 0; i < mySideBar->songListWidget->count(); i++) {
        mySideBar->musicListChangeWid[i]->musiclistcolor();
        mySideBar->musicListChangeWid[i]->musicInfoWidget->clear();
        mySideBar->get_listmusic_information(i);
        mySideBar->newSongListBtn[i]->setIcon(QIcon(":/img/default/songlist_w .png"));

        mySideBar->newSongListBtn[i]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                         font-size: 14px;\
                                         \
                                        background-color:#1F2022;\
                                        color:#F9F9F9;border-radius:16px;}"
                                        "QToolButton::hover{background-color:#303032;border-radius:16px;}"
                                        "QToolButton::pressed{background-color:#303032;border-radius:16px;}");
    }
    mainWidget->setStyleSheet("#mainWidget{background:#252526;}");
    rightWid->setStyleSheet("#rightWid{background:#252526;}");
}

void MainWid::changeLightTheme()
{
    WidgetStyle::themeColor = 0;
    mySideBar->newSonglistPup->dlgcolor();
    mySideBar->renameSongListPup->dlgcolor();
    mySideBar->sidecolor();
    myPlaySongArea->playcolor();
    m_MiniWidget->minicolor();
    myTitleBar->titlecolor();
    mySideBar->myMusicListWid->musicInfoWidget->clear();
    mySideBar->myMusicListWid->get_localmusic_information();
    nullMusicWidget->nullWidgetColor();

    mySideBar->myMusicListWid->musiclistcolor();
    for (int i = 0; i < mySideBar->songListWidget->count(); i++) {
        mySideBar->musicListChangeWid[i]->musicInfoWidget->clear();
        mySideBar->get_listmusic_information(i);
        mySideBar->musicListChangeWid[i]->musiclistcolor();

        mySideBar->newSongListBtn[i]->setIcon(QIcon(":/img/default/songlist.png"));

        mySideBar->newSongListBtn[i]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                         font-size: 14px;\
                                         \
                                        background-color:#FAFAFA;\
                                        color:#303133;border-radius:16px;}"
                                        "QToolButton::hover{background-color:#EEEEEE;border-radius:16px;}"
                                        "QToolButton::pressed{background-color:#EEEEEE;border-radius:16px;}");
    }
    mainWidget->setStyleSheet("#mainWidget{background:#FFFFFF;}");
    rightWid->setStyleSheet("#rightWid{background:#FFFFFF;}");

}

// 空页面添加歌曲
void MainWid::nullMusicWidgetAddSong()
{
    mySideBar->myMusicListWid->on_top_addSongBtn_slot();
    if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
    {
        rightlayout->replaceWidget(nullMusicWidget,mySideBar->rightChangeWid);
        nullMusicWidget->hide();
        mySideBar->rightChangeWid->show();

//        rightlayout->addWidget(mySideBar->);
    }
}

void MainWid::nullMusicWidgetAddFile()
{
    QString DirName = QFileDialog::getExistingDirectory(this, "打开本地歌曲文件夹","",
                                                    QFileDialog::ShowDirsOnly
                                                  | QFileDialog::DontResolveSymlinks);

    if(DirName.isEmpty() || AllDirList.contains(DirName))
        return;

    QListWidgetItem *item = new QListWidgetItem();
    QCheckBox *checkBox = new QCheckBox;
    checkBox->setText(DirName);
    checkBox->setToolTip(DirName);
    mySideBar->myMusicListWid->musicInfoWidget->addItem(item);   //QListWidget
    mySideBar->myMusicListWid->musicInfoWidget->setItemWidget(item,checkBox);
    checkBox->setChecked(true);
    add_nullMusicWidgetAddFile();
}
void MainWid::add_nullMusicWidgetAddFile()
{
    int count = mySideBar->myMusicListWid->musicInfoWidget->count();
//    mySideBar->myMusicListWid->musicInfoWidget->clear();
    DirList.clear();
    MusicPathList.clear();
    AllDirList.clear();
    for(int i = 0;i < count;++i)
    {
         QListWidgetItem *item = mySideBar->myMusicListWid->musicInfoWidget->item(i);
         QWidget *widget = mySideBar->myMusicListWid->musicInfoWidget->itemWidget(item);
         QCheckBox *box = reinterpret_cast<QCheckBox*>(widget);

         MusicPath tmpPath;
         if(box->isChecked())
         {
            DirList.append(box->text());

         tmpPath.path = box->text();
         tmpPath.ischecked = true;
         MusicPathList.append(tmpPath);
         }
         else
         {
            tmpPath.path = box->text();
            tmpPath.ischecked = false;
            MusicPathList.append(tmpPath);
         }
         AllDirList.append(box->text());
    }
    SongDirPath = DirList;
    QSqlQuery query;

////        Dir = QDir::toNativeSeparators(songFiles.at(i));
//        matchDir.setPath(AllDirList.at(i));   // QDir
//        matchDir.setNameFilters(exts);
    mySideBar->myMusicListWid->musicInfoWidget->clear();
    qDebug()<<"SongDirPath : "<<SongDirPath.count();
    for(int i=0; i<SongDirPath.count(); i++)
    {
        matchDir.setPath(SongDirPath.at(i));
        QStringList exts;
        exts.append(QString("*.mp3"));
        exts.append(QString("*.ogg"));
        exts.append(QString("*.wav"));
        exts.append(QString("*.wma"));
        exts.append(QString("*.spx"));
        exts.append(QString("*.ape"));
        exts.append(QString("*.flac"));
        matchDir.setNameFilters(exts);
        matchMp3Files = matchDir.entryList();
        qDebug()<<"matchMp3Files : "<<matchMp3Files;
        qDebug()<<"matchMp3Files : "<<matchMp3Files.count();
        for(int j = 0;j < matchMp3Files.count();j++)
        {
            mp3Name = SongDirPath.at(i) + '/' + matchMp3Files.at(j);
            QFile file(mp3Name);
            file.open(QIODevice::ReadOnly);
            MD5Str = QCryptographicHash::hash(file.readAll(),QCryptographicHash::Md5).toHex().constData();
            file.close();

            if(MD5List.contains(MD5Str))
            {
                continue;
            }
            MD5List.append(MD5Str);

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

                titleStr = tags["TITLE"].toString().toCString(true);
                if(titleStr.isEmpty())
                    titleStr = fileInfo.baseName();

                artistStr = tags["ARTIST"].toString().toCString(true);
                if(artistStr.isEmpty())
                    artistStr = "未知歌手";

                albumStr = tags["ALBUM"].toString().toCString(true);
                if(albumStr.isEmpty())
                    albumStr = "未知专辑";

                TagLib::AudioProperties *properties = f.audioProperties();

                int seconds = properties->length() % 60;
                int minutes = (properties->length() - seconds) / 60;
                timeStr = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
            }
            QListWidgetItem *listItem = new QListWidgetItem(mySideBar->myMusicListWid->musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            mySideBar->myMusicListWid->musicInfoWidget->setItemWidget(listItem,songitem1);

            songitem1->song_singer_albumText(titleStr,artistStr,albumStr); //歌曲名称 歌手 专辑
            songitem1->songTimeLabel->setText(timeStr); //时长

            mySideBar->myMusicListWid->localModel->setTable("LocalMusic");
            mySideBar->myMusicListWid->localModel->select();

            int id = 0;
            if (mySideBar->myMusicListWid->localModel->rowCount() != 0)
            {
                int num = mySideBar->myMusicListWid->localModel->data(mySideBar->myMusicListWid->
                        localModel->index(mySideBar->myMusicListWid->localModel->rowCount()-1,0)).toInt();
                id = num + 1;
            }

            mySideBar->myMusicListWid->allmusic.append(mp3Name);
            mySideBar->myMusicListWid->localAllMusicid.append(QString::number(id));
            mySideBar->myMusicListWid->PlayList->addMedia(QUrl::fromLocalFile(mp3Name));

            query.exec(QString("insert into LocalMusic (id,musicname,filepath,singer,album,type,size,time) values (%1,'%2','%3','%4','%5','%6','%7','%8')")
                       .arg(id).arg(titleStr).arg(mp3Name).arg(artistStr).arg(albumStr).arg(type).arg(mp3Size).arg(timeStr));

            mySideBar->myMusicListWid->songNumberLabel->setText(tr("A total of")+QString::number(mySideBar->myMusicListWid->musicInfoWidget->count())+tr("The first"));
            qDebug()<<"musicInfoWidget count : "<<mySideBar->myMusicListWid->musicInfoWidget->count();
        }
    }
    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
}

// 清除历史记录
void MainWid::clear_HistoryPlayList()
{
//    int values=QMessageBox::information(this,tr("清除本地音乐列表"),tr("确定清除本地音乐列表?."),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
//    if(values == QMessageBox::Yes)
//    {
    QSqlQuery query;
    query.exec("select * from HistoryPlayList");
    query.exec("delete from HistoryPlayList");
    qDebug()<<query.exec("delete from HistoryPlayList");
    myPlaySongArea->mybeforeList->beforePlayList->clear();
//        playlist->clear();

    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
}

void MainWid::showSearchResultWidget()
{
    myTitleBar->searchWidget->hide();
    myTitleBar->searchResultWidget->musicInfoWidget->clear();
    QString enterStr = myTitleBar->searchEdit->text().trimmed();
    if(enterStr != "")
    {
        QSqlQuery searchQuery;
        QString searchStr = QString("select * from LocalMusic where musicname LIKE '%%1%'").arg(enterStr);
        searchQuery.exec(searchStr);
        while(searchQuery.next())
        {
            QString songName = searchQuery.value(1).toString();
            QString Path = searchQuery.value(2).toString();
            QString Title = searchQuery.value(3).toString();
            QString Album = searchQuery.value(4).toString();
            QString Time = searchQuery.value(7).toString();

            QListWidgetItem *resultitem=new QListWidgetItem(myTitleBar->searchResultWidget->musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            myTitleBar->searchResultWidget->musicInfoWidget->setItemWidget(resultitem,songitem1);

//            songitem1->songNameLabel->setText(songName); //歌曲名称
//            songitem1->albumLabel->setText(Album);  //专辑
//            songitem1->songTimeLabel->setText(Time); //时长
//            songitem1->singerLabel->setText(Title); //歌手

            songitem1->song_singer_albumText(songName,Title,Album);
            songitem1->songTimeLabel->setText(Time); //时长

            myTitleBar->searchResultWidget->PlayList->addMedia(QUrl::fromLocalFile(Path));

    //        searchResultWidget->musicInfoWidget->show();
        }
        myTitleBar->searchResultWidget->songNumberLabel->setText(tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));

        myTitleBar->searchResultWidget->show();
        myTitleBar->searchResultWidget->raise();
        rightlayout->replaceWidget(mySideBar->rightChangeWid,myTitleBar->searchResultWidget);
        mySideBar->rightChangeWid->hide();
    }
}

void MainWid::hideSearchResultWidget()
{
    if (!(myTitleBar->searchResultWidget->isHidden()))
    {
        myTitleBar->searchResultWidget->hide();
        rightlayout->replaceWidget(myTitleBar->searchResultWidget, mySideBar->rightChangeWid);
        mySideBar->rightChangeWid->show();
    }
}

void MainWid::currentPlayHighlight()
{

//    currentPlayIndex = mySideBar->myMusicListWid->PlayList->currentIndex();

//    QWidget* itemWid = mySideBar->myMusicListWid->musicInfoWidget->itemWidget(mySideBar->myMusicListWid->musicInfoWidget->item(currentPlayIndex));
//    QList<QLabel*> songItemList = itemWid->findChildren<QLabel*>();
//    songItemList[0]->setStyleSheet("color:red;");

    //    qDebug() << "songItemList[0]->songNameLabel" << songItemList[0]->songNameLabel->text();
}

void MainWid::local_Music()
{
    int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
    int music = mySideBar->myMusicListWid->localAllMusicid[currentIndex].toInt();
//    mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
    model->select();

    QString mp3Path = model->data(model->index(music, 2)).toString();
    qDebug()<<"mp3Path : "<<mp3Path;
    QFileInfo file(mp3Path);
    if(file.exists())
    {
        mySideBar->myMusicListWid->Music->play();
    }
    else
    {
        qDebug()<<"mp3Path is not exists !";
        if (mySideBar->myMusicListWid->PlayList->playbackMode() == QMediaPlaylist::Sequential)
        {
            if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
            {
                if(currentIndex == mySideBar->myMusicListWid->musicInfoWidget->count() - 1)
                {
//                    promptMessage();
                    mySideBar->myMusicListWid->Music->stop();
                    mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
                    QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
                    mySideBar->myMusicListWid->Music->play();
                    qDebug()<< "mp3Name :  " <<mp3Name;
                    myPlaySongArea->songText(mp3Name); // 正在播放

                    QPixmap default_photo = QPixmap(":/img/fengmian.png");
                    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
                    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
                    m_MiniWidget->m_coverLabel->setPixmap(default_photo);


                    TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
                    TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
                    if (!frameList.isEmpty())
                    {
                        TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
                        QPixmap photo;
                        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

                        photo = photo.scaled(40,40,Qt::KeepAspectRatio);
                        myPlaySongArea->coverPhotoLabel->setPixmap(photo);
                        m_MiniWidget->m_coverLabel->setPixmap(photo);

                        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
                    }
                }
                else
                {
//                    QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("警告"),tr("暂无法打开文件，请检查路径和文件名称"),QMessageBox::Yes);
//                    warn->button(QMessageBox::Yes)->setText("确定");
//                    warn->exec();
//                    promptMessage();
                    mySideBar->myMusicListWid->Music->stop();
                    mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
                    QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
                    mySideBar->myMusicListWid->Music->play();
                    myPlaySongArea->songText(mp3Name); // 正在播放
//                    QPixmap default_photo = QPixmap(":/img/fengmian.png");
//                    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
//                    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
//                    m_MiniWidget->m_coverLabel->setPixmap(default_photo);
//                    TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//                    qDebug()<<"123";
//                    TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//                    qDebug()<<"123";
//                    if (!frameList.isEmpty())
//                    {
//                        TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//                        QPixmap photo;
//                        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//                        photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//                        myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//                        m_MiniWidget->m_coverLabel->setPixmap(photo);

//                        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());
//                        qDebug()<<"123";
//                    }
                }
            }
        }
        else
        {
//            QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("警告"),tr("暂无法打开文件，请检查路径和文件名称"),QMessageBox::Yes);
//            warn->button(QMessageBox::Yes)->setText("确定");
//            warn->exec();
//            promptMessage();
            mySideBar->myMusicListWid->Music->stop();
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(currentIndex);
            QString mp3Name = model->data(model->index(currentIndex, 1)).toString();
            mySideBar->myMusicListWid->Music->play();
            myPlaySongArea->songText(mp3Name); // 正在播放
            QPixmap default_photo = QPixmap(":/img/fengmian.png");
            default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
            myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
            m_MiniWidget->m_coverLabel->setPixmap(default_photo);
//            TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//            TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//            if (!frameList.isEmpty())
//            {
//                TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//                QPixmap photo;
//                photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//                photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//                myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//                m_MiniWidget->m_coverLabel->setPixmap(photo);

//                photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//            }
        }
        return ;
    }
}

void MainWid::new_PlayList()
{
    qDebug()<<"count : "<<mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count();
    int thisrow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();
    model_1->select();
    qDebug()<<"currentSelectList : "<<mySideBar->currentSelectList;
    qDebug()<<"thisrow : "<<thisrow;
    qDebug()<<"thisrow : "<<thisrow;
    if(thisrow != -1)
    {
        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(thisrow);
        mySideBar->sidemodel->setTable("NewPlayList");
        mySideBar->sidemodel->select();
        QString allmusicid = mySideBar->sidemodel->data(mySideBar->sidemodel->index(mySideBar->currentSelectList,1)).toString();
        int thisid = allmusicid.split(",")[thisrow].toInt();
        QStringList thismusicinfo = mySideBar->myMusicListWid->get_info_from_db(thisid);
        QString mp3Path   = thismusicinfo[1];
        qDebug()<<"mp3Path : "<<mp3Path;
        QFileInfo file(mp3Path);
        if(file.exists())
        {
            mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
        }
        else
        {
            qDebug()<<"mp3Path is not exists !";
            if (mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->playbackMode() == QMediaPlaylist::Sequential)
            {
                if(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count() > 0)
                {
                    if(thisrow == mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->count() - 1)
                    {
                        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
                        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(0);
                        QString mp3Name = model->data(model->index(thisrow, 1)).toString();
                        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
                        myPlaySongArea->songText(mp3Name); // 正在播放

                        QPixmap default_photo = QPixmap(":/img/fengmian.png");
                        default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
                        myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
                        m_MiniWidget->m_coverLabel->setPixmap(default_photo);


//                        TagLib::MPEG::File fx(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//                        TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//                        if (!frameList.isEmpty())
//                        {
//                            TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//                            QPixmap photo;
//                            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//                            photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//                            myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//                            m_MiniWidget->m_coverLabel->setPixmap(photo);

//                            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//                        }
                    }
                    else
                    {
                        QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("警告"),tr("暂无法打开文件，请检查路径和文件名称"),QMessageBox::Yes);
                        warn->button(QMessageBox::Yes)->setText("确定");
                        warn->exec();
                        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
                        QString mp3Name = model->data(model->index(thisrow, 1)).toString();
                        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
                        myPlaySongArea->songText(mp3Name); // 正在播放
                        QPixmap default_photo = QPixmap(":/img/fengmian.png");
                        default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
                        myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
                        m_MiniWidget->m_coverLabel->setPixmap(default_photo);

                        TagLib::MPEG::File fx(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
                        TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
                        if (!frameList.isEmpty())
                        {
                            TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
                            QPixmap photo;
                            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

                            photo = photo.scaled(40,40,Qt::KeepAspectRatio);
                            myPlaySongArea->coverPhotoLabel->setPixmap(photo);
                            m_MiniWidget->m_coverLabel->setPixmap(photo);

                            photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

                        }
                    }
                }
            }
            else
            {
                QMessageBox *warn = new QMessageBox(QMessageBox::Warning,tr("警告"),tr("暂无法打开文件，请检查路径和文件名称"),QMessageBox::Yes);
                warn->button(QMessageBox::Yes)->setText("确定");
                warn->exec();
                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->stop();
                QString mp3Name = model->data(model->index(thisrow, 1)).toString();
                mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
                myPlaySongArea->songText(mp3Name); // 正在播放
                QPixmap default_photo = QPixmap(":/img/fengmian.png");
                default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
                myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
                m_MiniWidget->m_coverLabel->setPixmap(default_photo);


//                TagLib::MPEG::File fx(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
//                TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
//                if (!frameList.isEmpty())
//                {
//                    TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
//                    QPixmap photo;
//                    photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//                    photo = photo.scaled(40,40,Qt::KeepAspectRatio);
//                    myPlaySongArea->coverPhotoLabel->setPixmap(photo);
//                    m_MiniWidget->m_coverLabel->setPixmap(photo);

//                    photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

//                }

            }
    //        mySideBar->sidemodel->setTable("NewPlayList");
    //        mySideBar->sidemodel->select();

    //        QStringList thismusicinfo = mySideBar->myMusicListWid->get_info_from_db(thisrow);
    //        QString mp3Name   = thismusicinfo[0];
    //        QString mp3Name1   = thismusicinfo[0];
    //        QString artistStr   = thismusicinfo[2];
    //        QString timeStr   = thismusicinfo[6];
    //        //迷你模式正在播放
    //        m_MiniWidget->m_songNameLab->setText(mp3Name);

    //        QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
    //        HistoryListItem *besongitem1 = new HistoryListItem;
    //        myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
    //        myPlaySongArea->songText(mp3Name); // 正在播放
    //        besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
    //        besongitem1->songTimeLabel->setText(timeStr);

    //        QSqlQuery query;
    //        query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
    //                   .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
    //        qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
    //                  .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
    //        myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
    //                    tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
            return ;
        }
    }
}

void MainWid::albumCover_local_playlist()
{
    QPixmap default_photo = QPixmap(":/img/fengmian.png");
    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
    m_MiniWidget->m_coverLabel->setPixmap(default_photo);
}

void MainWid::local_playInformation()
{
    model->setTable("LocalMusic");
    model->select();
    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
    int musicid = mySideBar->myMusicListWid->localAllMusicid[row].toInt();
    QString mp3Name = model->data(model->index(musicid, 1)).toString();
//    myPlaySongArea->songNameofNowPlaying->setText(mp3Name);  //正在播放
    QString mp3Name1 = model->data(model->index(musicid, 1)).toString();
    //迷你模式正在播放
    m_MiniWidget->m_songNameLab->setText(mp3Name);

    QString artistStr = model->data(model->index(musicid, 3)).toString();
    QString timeStr   = model->data(model->index(musicid, 7)).toString();
    QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
    HistoryListItem *besongitem1 = new HistoryListItem;
    myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
    myPlaySongArea->songText(mp3Name); // 正在播放
    besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
    besongitem1->songTimeLabel->setText(timeStr);

    QSqlQuery query;
    query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
               .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
    qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
              .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
}

void MainWid::playlist_playInformation()
{
    int thisrow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();
    qDebug()<<thisrow;
    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(thisrow);
    mySideBar->sidemodel->setTable("NewPlayList");
    mySideBar->sidemodel->select();
    QString allmusicid = mySideBar->sidemodel->data(mySideBar->sidemodel->index(mySideBar->currentSelectList,1)).toString();
    int thisid = allmusicid.split(",")[thisrow].toInt();
    qDebug()<<thisid;
    QStringList thismusicinfo = mySideBar->myMusicListWid->get_info_from_db(thisid);
    QString mp3Name   = thismusicinfo[0];
    QString mp3Name1   = thismusicinfo[0];
    QString artistStr   = thismusicinfo[2];
    QString timeStr   = thismusicinfo[6];

    QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
    HistoryListItem *besongitem1 = new HistoryListItem;
    myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
    myPlaySongArea->songText(mp3Name); // 正在播放
    besongitem1->song_singerText(mp3Name1,artistStr); //历史列表
    besongitem1->songTimeLabel->setText(timeStr);
    //迷你模式正在播放
    m_MiniWidget->m_songNameLab->setText(mp3Name);

    QSqlQuery query;
    query.exec(QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
               .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr));
    qDebug()<<QString("insert into HistoryPlayList (id,mp3Name,artistStr,timeStr) values (%1,'%2','%3','%4')")
              .arg(qrand()%1000000).arg(mp3Name1).arg(artistStr).arg(timeStr);
    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
}

void MainWid::albumCover_local()
{
    TagLib::MPEG::File fx(mySideBar->myMusicListWid->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
    TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
    if (!frameList.isEmpty())
    {
        TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
        QPixmap photo;
        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

        photo = photo.scaled(40,40,Qt::KeepAspectRatio);
        myPlaySongArea->coverPhotoLabel->setPixmap(photo);
        m_MiniWidget->m_coverLabel->setPixmap(photo);

        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

    }
}

void MainWid::albumCover_playlist()
{
    TagLib::MPEG::File fx(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentMedia().canonicalUrl().path().toStdString().c_str());
    TagLib::ID3v2::FrameList frameList = fx.ID3v2Tag()->frameListMap()["APIC"];
    if (!frameList.isEmpty())
    {
        TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
        QPixmap photo;
        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

        photo = photo.scaled(40,40,Qt::KeepAspectRatio);
        myPlaySongArea->coverPhotoLabel->setPixmap(photo);
        m_MiniWidget->m_coverLabel->setPixmap(photo);

        photo.loadFromData(reinterpret_cast<const uchar*>(picFrame->picture().data()), picFrame->picture().size());

    }
}

void MainWid::closePromptMessage()
{
    qDebug()<<"开始收回了";
    promptMessageLabel->hide();
    if(promptMessageTimer->isActive())
    {
        promptMessageTimer->stop();
    }
    qDebug()<<"收回了";
}

void MainWid::showPromptMessage()
{
    qDebug()<<"开始弹窗了";
    QPixmap photo;
    photo.load(":/img/promptwhile.png");
    photo = photo.scaled(628,128,Qt::KeepAspectRatio);
    promptMessageLabel = new QLabel(this);
    promptMessageLabel->setFixedSize(628,128);
    QRect rect = geometry();
    promptMessageLabel->setGeometry(int((rect.width() - promptMessageLabel->width())/2),
                                    int((rect.height() - promptMessageLabel->height())/2),
                                    promptMessageLabel->width(), promptMessageLabel->height());
    promptMessageLabel->setPixmap(photo);
//    wid->setStyleSheet("background-color:white;");
    promptMessageLabel->show();
}

void MainWid::dragEnterEvent(QDragEnterEvent *event)   //拖进事件
{
    qDebug()<<"dragEnterEvent";
    // 判断拖拽文件类型，文件名 接收该动作
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
//        event->setDropAction(Qt::CopyAction);
        event->acceptProposedAction();
    }
    QWidget::dragEnterEvent(event);
}

void MainWid::dragMoveEvent(QDragMoveEvent *event)
{
//    event->setDropAction(Qt::MoveAction);
}

void MainWid::dragLeaveEvent(QDragLeaveEvent *event)
{
//    QWidget::dragLeaveEvent(event);
}

void MainWid::dropEvent(QDropEvent *event)    //放下事件
{
    qDebug()<<"dropEvent";
    auto urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        return;
    }
    QStringList localpath;
    for(auto &url : urls)
    {
        localpath << url.toLocalFile();
    }

    if(!localpath.isEmpty())
    {
//        emit fromFilemanager(localpath);
        emit addFile(localpath);
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
            rightlayout->replaceWidget(nullMusicWidget,mySideBar->rightChangeWid);
            nullMusicWidget->hide();
            mySideBar->rightChangeWid->show();
        }
    }
}

void MainWid::processArgs(QStringList args)
{
    qDebug()<<args;
    QSqlQuery query;
    for(int i = 0; i < args.count(); i++)
    {
        Dir = QDir::toNativeSeparators(args.at(i));
        musicPath = args[i];
        if(!musicPath.isEmpty())
        {
            bool songExists = false;
            for(QString &songPlaylist:mySideBar->myMusicListWid->allmusic)
            {
                if(musicPath == songPlaylist)    //通过路径判断歌曲是否存在
                {
                    qDebug()<<"歌曲已存在 (通过路径判断歌曲是否存在)";
                    songExists = true;
                    break;
                }
            }
            if(songExists)
            {
                continue;
            }

            fileInfo.setFile(musicPath);

            musicType = fileInfo.suffix();        //文件类型

            qint64 musicFileSize = fileInfo.size();
            if(musicFileSize/1024)
            {
                if(musicFileSize/1024/1024)
                {
                    if(musicFileSize/1024/1024/1024)
                    {
                        musicSize = QString::number(musicFileSize/1024/1024/1024,10)+"GB";
                    }
                    else
                        musicSize = QString::number(musicFileSize/1024/1024,10)+"MB";
                }
                else
                    musicSize = QString::number(musicFileSize/1024,10)+"KB";
            }
            else
                musicSize = QString::number(musicFileSize,10)+"B";
            byteArray = musicPath.toLocal8Bit();

            if(musicPath.split(".").last() != "amr" && musicPath.split(".").last() != "mmf")
            {
                TagLib::FileRef f(byteArray.data());
                if(f.isNull())
                {
                    //can't read this music;
                    continue;
                }
                TagLib::PropertyMap propertyMap = f.file() ->properties();

                musicName = propertyMap["TITLE"].toString().toCString(true);
                if(musicName.isEmpty())
                    musicName = fileInfo.baseName();
                musicSinger = propertyMap["ARTIST"].toString().toCString(true);
                if(musicSinger.isEmpty())
                    musicSinger = "未知歌手";
                musicAlbum = propertyMap["ALBUM"].toString().toCString(true);
                if(musicAlbum.isEmpty())
                    musicAlbum = "未知专辑";
                TagLib::AudioProperties *properties = f.audioProperties();

                int seconds = properties->length() % 60;
                int minutes = (properties->length() - seconds) / 60;
                musicTime = QString::number(minutes)+":"+QString("%1").arg(seconds, 2, 10, QChar('0'));
            }
            QListWidgetItem *listItem = new QListWidgetItem(mySideBar->myMusicListWid->musicInfoWidget);
            SongItem *songitem1 = new SongItem;
            mySideBar->myMusicListWid->musicInfoWidget->setItemWidget(listItem,songitem1);
            songitem1->song_singer_albumText(musicName,musicSinger,musicAlbum); //歌曲名称 歌手 专辑
            songitem1->songTimeLabel->setText(musicTime); //时长

            mySideBar->myMusicListWid->localModel->setTable("LocalMusic");
            mySideBar->myMusicListWid->localModel->select();

            int id = 0;
            if (mySideBar->myMusicListWid->localModel->rowCount() != 0)
            {
                int num = mySideBar->myMusicListWid->localModel->data(mySideBar->myMusicListWid->localModel->index(mySideBar->myMusicListWid->localModel->rowCount()-1,0)).toInt();
                id = num + 1;
            }

            mySideBar->myMusicListWid->allmusic.append(musicPath);
            mySideBar->myMusicListWid->localAllMusicid.append(QString::number(id));
            mySideBar->myMusicListWid->PlayList->addMedia(QUrl::fromLocalFile(musicPath));

            query.exec(QString("insert into LocalMusic (id,musicname,filepath,singer,album,type,size,time) values (%1,'%2','%3','%4','%5','%6','%7','%8')")
                       .arg(id).arg(musicName).arg(musicPath).arg(musicSinger).arg(musicAlbum).arg(musicType).arg(musicSize).arg(musicTime));

            mySideBar->myMusicListWid->songNumberLabel->setText(tr("A total of")+QString::number(mySideBar->myMusicListWid->musicInfoWidget->count())+tr("The first"));
        }
    }
    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
    for(int i = 0;i < mySideBar->myMusicListWid->allmusic.count();i++)
    {
        if(mySideBar->myMusicListWid->allmusic[i] == musicPath)
        {
            qDebug()<<"musicPath : "<<musicPath;
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(i);
            int music = mySideBar->myMusicListWid->localAllMusicid[i].toInt();
            qDebug()<<"i : "<<i;
            model->setTable("LocalMusic");
            model->select();
            QString Name = model->data(model->index(music, 1)).toString();
            mySideBar->myMusicListWid->Music->play();
            myPlaySongArea->songText(Name); // 正在播放
            m_MiniWidget->m_songNameLab->setText(Name);

        }
    }
    if(mySideBar->myMusicListWid->allmusic.indexOf(musicPath) == -1)
    {
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
            qDebug()<<"musicPath : "<<musicPath;
            int addMusicId = mySideBar->myMusicListWid->musicInfoWidget->count() - 1;
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(addMusicId);
            int music = mySideBar->myMusicListWid->localAllMusicid[addMusicId].toInt();
            model->setTable("LocalMusic");
            model->select();
            QString Name = model->data(model->index(music, 1)).toString();
            mySideBar->myMusicListWid->Music->play();
            myPlaySongArea->songText(Name); // 正在播放
            m_MiniWidget->m_songNameLab->setText(Name);
        }
    }
    for (int n=0; n < mySideBar->myMusicListWid->allmusic.count(); n++)
    {
        qDebug("MainWid::processArgs: allmusic[%d]: '%s'", n, mySideBar->myMusicListWid->allmusic[n].toUtf8().data());
    }
}
