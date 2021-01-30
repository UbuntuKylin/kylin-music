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

#include <QDateTime>

#include "mainwid.h"
#include "songitem.h"
#include "miniwidget.h"
#include "widgetstyle.h"
#include "myapplication.h"
#include "xatom-helper.h"
#include "musicDataBase.h"

MainWid *MainWid::mutual = nullptr;  //！！！！初始化，非常重要

MainWid::MainWid(QString str, QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "Mainwindow intialed at " << QDateTime::currentDateTime().toString("    yyyy-MM-dd hh:mm:ss.zzz"); //设置显示格式
    mutual = this;//！！！赋值，非常重要
    Single(str);//单例
    initDaemonIpcDbus();//用户手册
    initDataBase();//数据库
    initDbus();//初始化dbus
    initGSettings();//初始化GSettings
    initControlse();//初始化控件
    initAction();//初始化事件
    initSystemTray();//初始化托盘

    if(argName != "")
    {
        kylin_music_play_request(argName);
    }
    initStyle();//初始化样式
    qDebug() << "Mainwindow displayed at " << QDateTime::currentDateTime().toString("    yyyy-MM-dd hh:mm:ss.zzz"); //设置显示格式
    qDebug()<<"--------------------程序初始化完成--------------------";
}

MainWid::~MainWid()
{
    //原来是直接调用析构函数
    m_MiniWidget->deleteLater();
}

void MainWid::Single(QString path)   //单例
{
    QStringList homePath = QStandardPaths::standardLocations(QStandardPaths::HomeLocation);
    QString lockPath = homePath.at(0) + "/.config/kylin-music-lock";
    int fd = open(lockPath.toUtf8().data(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    if (fd < 0) { exit(1); }
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
        qDebug()<<"麒麟音乐已经在运行";
        exit(0);
    }
    argName = path;
}

void MainWid::initStyle()//初始化样式
{
    //设置初始样式
    myTitleBar->menumodule->themeUpdate();

    //不接受焦点高亮。解决点击后有蓝框的问题
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    for(QPushButton *btn :list)
        btn->setFocusPolicy(Qt::NoFocus);

    qDebug()<<"初始化样式成功";
}

void MainWid::initControlse()//初始化控件
{
    setAcceptDrops(true);
    //    promptMessage();  //提示信息
        setMinimumSize(960,640);
    //    setFocus();
    //    this->setWindowFlags(Qt::FramelessWindowHint);            //
        this->setAttribute(Qt::WA_TranslucentBackground, true);     //窗体透明

        //窗体显示在中间
        QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
        this->move((availableGeometry.width() - this->width())/2, (availableGeometry.height() - this->height())/2);

    //    this->setWindowTitle(tr("麒麟音乐"));
        this->setWindowTitle(tr("Kylin music"));
        this->setWindowIcon(QIcon(":/img/kylin-music.png"));

        myTitleBar = new TitleBar(this);
        mySideBar = new SideBar(this);

//        connect(mySideBar,SIGNAL(createNewList()),this,SLOT(connectHandle()));

        nullMusicWidget = new ChangeListWid(this);   //空页面

        //    myMusicListWid = new MusicListWid(this);

        myPlaySongArea = new PlaySongArea(this);

        QHBoxLayout *mainlayout = new QHBoxLayout(this);

        QVBoxLayout *leftlayout = new QVBoxLayout(this);
        rightlayout = new QVBoxLayout(this);

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

        //实例化进度条
        hSlider = new MusicSlider(this);


        rightlayout->addWidget(hSlider);
        rightlayout->addWidget(myPlaySongArea,0,Qt::AlignBottom);

        vSlider = new Slider();
        vSlider->installEventFilter(this);
        vSlider->setOrientation(Qt::Vertical);
    //    vSlider->setFixedSize(100,12);
    //    vSlider->setToolTip("音量调节");
        vSlider->setToolTip(tr("volume"));
        vSlider->setMinimum(0);
        vSlider->setMaximum(100);
        vSliderWid = new QWidget();
//        vSliderWid->setFixedSize(30,90);
        vSliderWid->setWindowFlags(Qt::FramelessWindowHint);

        HLayout = new QHBoxLayout;

        HLayout->addWidget(vSlider);
        vSliderWid->setLayout(HLayout);
        vSliderWid->setStyleSheet("background-color:red;");
        vSliderWid->setGeometry(811,498,30,90);
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
        qDebug()<<"初始化控件成功";
}

void MainWid::initDbus()//初始化dbus
{
    QDBusConnection sessionBus = QDBusConnection::sessionBus();
    if(sessionBus.registerService("org.ukui.kylin_music"))
    {
        sessionBus.registerObject("/org/ukui/kylin_music",this,
                                  QDBusConnection::ExportAllContents);
        qDebug()<<"初始化DBUS成功";
    }
    else
        qDebug()<<"初始化DBUS失败";
}
void MainWid::onPlaylistChanged(int index)
{
    if(mySideBar->musicListChangeWid[index]->m_musicInitialed == false){
        qDebug()<< __FILE__ << " " <<__LINE__ <<"QMediaPlayer Not initialed";
        return ;
    }
    qDebug()<< __FILE__ << " " <<__LINE__ <<"onPlaylistChanged";
    connect(mySideBar->musicListChangeWid[index]->Music,SIGNAL(positionChanged(qint64)),
        this,SLOT(playlist_positionChange(qint64)));  //滑块进度条位置改变
    connect(mySideBar->musicListChangeWid[index]->Music,SIGNAL(durationChanged(qint64)),
        this,SLOT(playlist_durationChange(qint64)));
}
void MainWid::initAction()//初始化事件
{
    connect(vSlider,&QSlider::valueChanged,this,&MainWid::changeVolume);

    connect(myTitleBar->miniBtn,&QPushButton::clicked,this,&MainWid::slot_showMiniWidget);
    connect(m_MiniWidget->m_recoveryWinBtn,&QPushButton::clicked,this,&MainWid::slot_recoverNormalWidget);

    this->setCentralWidget(mainWidget);


    connect(myPlaySongArea->playBtn,SIGNAL(clicked(bool)),this,SLOT(play_Song()));   //播放歌曲

    connect(mySideBar->myMusicListWid->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_listWidget_doubleClicked(QListWidgetItem*)));
    connect(mySideBar,SIGNAL(changePlaylist(int)),this,SLOT(onPlaylistChanged(int)));
    connect(myPlaySongArea->mybeforeList->beforePlayList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(on_historyWidget_doubleClicked(QListWidgetItem*)));

    //播放列表右键菜单
    mySideBar->myMusicListWid->musicInfoWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mySideBar->myMusicListWid->musicInfoWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(on_musicInfoWidget_customContextMenuRequested(const QPoint&)));

    connect(myPlaySongArea->lastBtn,SIGNAL(clicked()),this,SLOT(on_lastBtn_clicked())); //上一首
//    connect(myPlaySongArea->lastBtn,SIGNAL(clicked()),this,SLOT(on_playlist_lastBtn_clicked())); //上一首
    connect(myPlaySongArea->nextBtn,SIGNAL(clicked()),this,SLOT(on_nextBtn_clicked())); //下一首
//    connect(myPlaySongArea->nextBtn,SIGNAL(clicked()),this,SLOT(on_playlist_nextBtn_clicked())); //下一首
    connect(mySideBar->myMusicListWid->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::updatalistwidget);
    connect(myPlaySongArea->mybeforeList->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::updatehistorywidget);

//    connect(mySideBar->myMusicListWid->Music,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(currentMediaChanged(QMediaContent)));

    /* 歌曲列表播放活动条关联的槽函数 */
    connect(mySideBar->myMusicListWid->Music,SIGNAL(positionChanged(qint64)),this,SLOT(positionChange(qint64)));  //滑块进度条位置改变
    connect(mySideBar->myMusicListWid->Music,SIGNAL(durationChanged(qint64)),this,SLOT(durationChange(qint64)));
    connect(hSlider,SIGNAL(sliderPressed()),this,SLOT(slidePress()));
    connect(hSlider,SIGNAL(sliderReleased()),this,SLOT(slideRelease()));
    connect(hSlider,&QSlider::valueChanged,this,&MainWid::setPosition);
    //    connect(hSlider,SIGNAL(sliderMoved(int)),this,SLOT(slideMove(int)));
//    connect(hSlider,&MusicListWid::on_top_addSongBtn_slot,this,&MainWid::setHsliderPosition);

    /* 历史列表播放活动条关联的槽函数 */
//    connect(myPlaySongArea->mybeforeList->Music,SIGNAL(positionChanged(qint64)),this,SLOT(historyPositionChange(qint64)));  //滑块进度条位置改变
//    connect(myPlaySongArea->mybeforeList->Music,SIGNAL(durationChanged(qint64)),this,SLOT(historyDurationChange(qint64)));

//    connect(mySideBar->myMusicListWid->Music,&QMediaPlayer::stateChanged,this,&MainWid::Music_stateChang);
    connect(myPlaySongArea->playModeBtn,SIGNAL(clicked(bool)),this,SLOT(PlayModeChanged()));
    connect(myPlaySongArea->likeMusicBtn,&QPushButton::clicked,this,&MainWid::addLike);

    /* 最大化 最小化 关闭 */
    connect(myTitleBar->maximumBtn, &QToolButton::clicked, this, &MainWid::slot_showMaximized);
    connect(myTitleBar->minimumBtn, &QToolButton::clicked, this, &MainWid::slot_showMinimized);
    connect(myTitleBar->closeBtn, &QToolButton::clicked, this, &MainWid::close_MainWid);

    //迷你模式槽函数绑定
    connect(m_MiniWidget->m_nextBtn,&QPushButton::clicked,this,&MainWid::on_nextBtn_clicked);
    connect(m_MiniWidget->m_playStateBtn,&QPushButton::clicked,this,&MainWid::play_Song);
    connect(m_MiniWidget->m_preBtn,&QPushButton::clicked,this,&MainWid::on_lastBtn_clicked);
    connect(m_MiniWidget->m_orderBtn,&QPushButton::clicked,this,&MainWid::PlayModeChanged);
    connect(m_MiniWidget->m_closeBtn,&QPushButton::clicked,this,&MainWid::slot_closeMiniWidget);
    connect(m_MiniWidget->m_loveBtn,&QPushButton::clicked,this,&MainWid::addLike);

    //历史播放列表相关
    connect(myPlaySongArea->listBtn,&QPushButton::clicked,this,&MainWid::showBeforeList);

    myPlaySongArea->mybeforeList->raise();

    //sidebar右键菜单
    mySideBar->songListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(mySideBar->songListWidget,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(on_sidebarWidget_customContextMenuRequested(const QPoint&)));

    //提示信息弹窗
    connect(mySideBar->renameSongListPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(renameThisSongList()));
    connect(mySideBar->promptSongListPup->confirmBtn,&QPushButton::clicked,mySideBar,&SideBar::deleteSongList); //删除歌单提示信息
    connect(mySideBar->promptExistListPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(renameSongListCon()));
    connect(mySideBar->promptRenamePlayList->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(promptRenamePlayList()));
    connect(mySideBar->promptRemovePlayList->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(promptRemovePlayList()));
    connect(mySideBar->renameSongListPup->enterLineEdit,SIGNAL(returnPressed()),this,SLOT(renameThisSongList()));
    connect(myTitleBar->aboutAct,SIGNAL(triggered(bool)),this,SLOT(showAboutWidget()));

//    connect(mySideBar->myMusicListWid,&MusicListWid::fromFilemanager,mySideBar->myMusicListWid,&MusicListWid::addFile);

//    //历史列表右键菜单
//    myPlaySongArea->mybeforeList->beforePlayList->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(myPlaySongArea->mybeforeList->beforePlayList,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(on_historyWidget_customContextMenuRequested(const QPoint&)));

    QShortcut *addvSlider=new QShortcut(Qt::Key_W,this);
    connect(addvSlider,&QShortcut::activated,this,&MainWid::addvSlider_slot);
    QShortcut *subvSlider=new QShortcut(Qt::Key_S,this);
    connect(subvSlider,&QShortcut::activated,this,&MainWid::subvSlider_slot);
    connect(myPlaySongArea->volumeBtn,&QPushButton::clicked,this,&MainWid::show_volumeBtn);
    connect(myPlaySongArea->mybeforeList->emptyBtn,SIGNAL(clicked(bool)),this,SLOT(clear_HistoryPlayList()));

    int ret;

    mySideBar->playListName.clear();
    ret = g_db->getPlayList(mySideBar->playListName);
    if(ret == DB_OP_SUCC)
    {
        for(int i = 0;i < mySideBar->playListName.size();i++)
        {
            connect(mySideBar->musicListChangeWid[i]->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
                this,SLOT(on_musicListChangeWid_doubleClicked(QListWidgetItem*)));
            connect(mySideBar->musicListChangeWid[i]->PlayList,&QMediaPlaylist::currentIndexChanged,
                this,&MainWid::updataplaylistwidget);
            connect(mySideBar->musicListChangeWid[i]->PlayList,&QMediaPlaylist::currentIndexChanged,
                this,&MainWid::playlist_currentIndexChanged);
//            connect(mySideBar->musicListChangeWid[i]->Music,&QMediaPlayer::stateChanged,
//                this,&MainWid::Music_playlist_stateChang);
            connect(mySideBar->musicListChangeWid[i]->Music,SIGNAL(positionChanged(qint64)),
                this,SLOT(playlist_positionChange(qint64)));  //滑块进度条位置改变
            connect(mySideBar->musicListChangeWid[i]->Music,SIGNAL(durationChanged(qint64)),
                this,SLOT(playlist_durationChange(qint64)));
        }
    }

    //切换主题
    connect(myTitleBar->menumodule,&menuModule::menuModuleSetThemeStyle,this,&MainWid::menuModuleSetThemeStyle);
//    connect(myTitleBar->darkThemeAct,&QAction::triggered,this,&MainWid::changeDarkTheme);
//    connect(myTitleBar->lightThemeAct,&QAction::triggered,this,&MainWid::changeLightTheme);

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

    connect(mySideBar->myMusicListWid->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::songListOutHightStyle);
    qDebug()<<"初始化事件成功";

    connect(mySideBar->myMusicListWid->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::local_currentIndexChanged);
//    int ref;
//    qDebug()<<mySideBar->playListName.size();
//    ref = g_db->getPlayList(mySideBar->playListName);
//    if(ret == DB_OP_SUCC)
//    {
//        for(int i = 0;i < mySideBar->playListName.size();i++)
//        {

//        }
//    }
//    connect(mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::playlist_currentIndexChanged);
    connect(myPlaySongArea->mybeforeList->PlayList,&QMediaPlaylist::currentIndexChanged,this,&MainWid::history_currentIndexChanged);
}

void MainWid::songListOutHightStyle(int cur)
{
    QWidget *wid = mySideBar->myMusicListWid->musicInfoWidget->itemWidget(mySideBar->myMusicListWid->musicInfoWidget->currentItem());
    SongItem* item = qobject_cast<SongItem *>(wid);
    item->itemType = SongItem::highlightType;//打上高亮标签
    changeItemColour();
    item->itemType = SongItem::defaultType;;//去掉高亮标签
}

void MainWid::initAddPlayList(int num)//初始化播放列表
{
    connect(mySideBar->musicListChangeWid[num]->musicInfoWidget,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this,SLOT(on_musicListChangeWid_doubleClicked(QListWidgetItem*)));
    connect(mySideBar->musicListChangeWid[num]->PlayList,&QMediaPlaylist::currentIndexChanged,
        this,&MainWid::updataplaylistwidget);
    connect(mySideBar->musicListChangeWid[num]->PlayList,&QMediaPlaylist::currentIndexChanged,
        this,&MainWid::playlist_currentIndexChanged);
    connect(mySideBar->musicListChangeWid[num]->Music,SIGNAL(positionChanged(qint64)),
        this,SLOT(playlist_positionChange(qint64)));  //滑块进度条位置改变
    connect(mySideBar->musicListChangeWid[num]->Music,SIGNAL(durationChanged(qint64)),
        this,SLOT(playlist_durationChange(qint64)));
    qDebug()<<"初始化播放列表功";
}

void MainWid::initGSettings()//初始化GSettings
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
    qDebug()<<"初始化GSettings成功";
}

void MainWid::initSystemTray()//初始化托盘
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
    qDebug()<<"初始化托盘成功";
}

void MainWid::initDaemonIpcDbus()//用户手册
{
    mDaemonIpcDbus = new DaemonIpcDbus();
    qDebug()<<"包含用户手册成功";
}

void MainWid::initDataBase()//数据库
{
    int res;
    res = g_db->initDataBase();
    if(res != DB_OP_SUCC)
    {
        qDebug() << "数据库加载失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
    }
    qDebug() << "数据库加载成功";
}

int MainWid::kylin_music_play_request(QString path)
{
    QStringList qStringListPath ;
    qStringListPath << path;
    processArgs(qStringListPath);
    if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
    {
        rightlayout->replaceWidget(nullMusicWidget,mySideBar->rightChangeWid);
        nullMusicWidget->hide();
        mySideBar->rightChangeWid->show();
    }
    return 0;
}

// 实现键盘响应
void MainWid::keyPressEvent(QKeyEvent *event)
{
    // F1快捷键打开用户手册
    if (event->key() == Qt::Key_F1) {
        if (!mDaemonIpcDbus->daemonIsNotRunning()){
            // F1快捷键打开用户手册，如kylin-music
            //由于是小工具类，下面的showGuide参数要填写"tools/kylin-recorder"
            mDaemonIpcDbus->showGuide("kylin-music");
        }
    }
    else if(event->key() == Qt::Key_Right)
    {
        return;
    }
    else if(event->key() == Qt::Key_Left)
    {
        return;
    }
    QMainWindow::keyPressEvent(event);
}

void MainWid::resizeEvent(QResizeEvent *event)
{
    myPlaySongArea->mybeforeList->setGeometry(width() - 310,-10,320,height() - 68);
}

#include <QPropertyAnimation>
void MainWid::slot_showMiniWidget()
{
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

void MainWid::slot_showMaximized()
{
    if(Minimize)
    {
        showNormal();
        Minimize = false;
        myTitleBar->maximumBtn->setIcon(QIcon::fromTheme("window-maximize-symbolic"));
        myTitleBar->maximumBtn->setToolTip(tr("maximize"));
        myTitleBar->maximumBtn->setProperty("isWindowButton", 0x1);
        myTitleBar->maximumBtn->setProperty("useIconHighlightEffect", 0x2);
        myTitleBar->maximumBtn->setFlat(true);
    }
    else
    {
//        showMaximized();
        showFullScreen();
        Minimize = true;
        myTitleBar->maximumBtn->setIcon(QIcon::fromTheme("window-restore-symbolic"));
//                maximumBtn->setToolTip(tr("还原"));
        myTitleBar->maximumBtn->setToolTip(tr("reduction"));
        myTitleBar->maximumBtn->setProperty("isWindowButton", 0x1);
        myTitleBar->maximumBtn->setProperty("useIconHighlightEffect", 0x2);
        myTitleBar->maximumBtn->setFlat(true);
    }
}

void MainWid::slot_showMinimized()
{
//    this->hide();

    this->showMinimized();
    this->showNormal();
}

void MainWid::close_MainWid()
{
    this->close();
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
    if(mySideBar->currentMusicPlaylist == -2) {
        QMessageBox::about(this,"提示信息","请选择歌曲后再点击播放");
        return;
    }

    if(mySideBar->currentMusicPlaylist == -1)
    {
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
            int ret;
            musicDataStruct fileData;
            if(mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PlayingState)
            {
                mySideBar->myMusicListWid->isStartPlay = false;

                myPlaySongArea->playBtn->setFixedSize(36,36);
                myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);

                myPlaySongArea->playBtn->setIconSize(QSize(36,36));

                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

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
                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");


                //迷你模式样式
    //                m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/pause2.png"));
                m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

                albumCover_local_playlist();  //本地和歌单默认封面


                int currentIndex = mySideBar->myMusicListWid->PlayList->currentIndex();
                if(currentIndex == -1)
                {
                    return;
                }

                QString filePath = mySideBar->myMusicListWid->localAllMusicid[currentIndex];
                ret = g_db->getSongInfoFromLocalMusic(filePath, fileData);
                if(ret == DB_OP_SUCC)
                {
                    myPlaySongArea->songText(fileData.title); // 正在播放
                    m_MiniWidget->songText(fileData.title);   //mini正在播放
                }
                mySideBar->myMusicListWid->Music->play();
            }
        }
    }
    else if(mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        if(mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->musicInfoWidget->count() > 0)
        {
            int ret;
            musicDataStruct fileData;
            if(mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->state() == QMediaPlayer::PlayingState)
            {
                mySideBar->myMusicListWid->isStartPlay = false;
                myPlaySongArea->playBtn->setFixedSize(36,36);
                myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);
                myPlaySongArea->playBtn->setIconSize(QSize(36,36));
                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

                //迷你模式样式
                m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                // 根据当前播放的列表进行暂停
                mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->pause();
                return ;
            }

            // 没有歌曲在播放
            else
            {
                mySideBar->myMusicListWid->isStartPlay = true;
                myPlaySongArea->playBtn->setFixedSize(36,36);
                myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);

                myPlaySongArea->playBtn->setIconSize(QSize(36,36));
                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

                //迷你模式样式
                m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

                // 播放歌曲，并设置当前播放列表索引
                int currentIndex = mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->currentIndex();
                if(currentIndex == -1)
                {
                    return;
                }

                QString filePath = mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->localAllMusicid[currentIndex];
                ret = g_db->getSongInfoFromPlayList(fileData, filePath, mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->tableName);
                if(ret == DB_OP_SUCC)
                {
                    myPlaySongArea->songText(fileData.title); // 正在播放
                    m_MiniWidget->songText(fileData.title);   //mini正在播放
                }
                mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->play();
            }
        }
    }
    else
    {
        if(myPlaySongArea->mybeforeList->beforePlayList->count() > 0)
        {
            int ret;
            musicDataStruct fileData;
            if(myPlaySongArea->mybeforeList->Music->state() == QMediaPlayer::PlayingState)
            {
                myPlaySongArea->mybeforeList->isStartPlay = false;

                myPlaySongArea->playBtn->setFixedSize(36,36);
                myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);

                myPlaySongArea->playBtn->setIconSize(QSize(36,36));

                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");

                //迷你模式样式
                m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                myPlaySongArea->mybeforeList->Music->pause();
            }
            //
            else
            {
                myPlaySongArea->mybeforeList->isStartPlay = true;
                myPlaySongArea->playBtn->setFixedSize(36,36);
                myPlaySongArea->playBtn->setCursor(Qt::PointingHandCursor);

                myPlaySongArea->playBtn->setIconSize(QSize(36,36));
                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");


                //迷你模式样式
                //                m_MiniWidget->m_playStateBtn->setIcon(QIcon(":/img/default/pause2.png"));
                m_MiniWidget->m_playStateBtn->setIconSize(QSize(36,36));
                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

                albumCover_local_playlist();  //本地和歌单默认封面

                int currentIndex = myPlaySongArea->mybeforeList->PlayList->currentIndex();

                QString filePath = myPlaySongArea->mybeforeList->historyMusicid[currentIndex];
                ret = g_db->getSongInfoFromHistoryMusic(filePath, fileData);
                if(ret == DB_OP_SUCC)
                {
                    myPlaySongArea->songText(fileData.title); // 正在播放
                    m_MiniWidget->songText(fileData.title);   //mini正在播放
                }
                myPlaySongArea->mybeforeList->Music->play();
            }
        }
    }
}

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
//    sideMenu->addAction(actionRename);
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
    int ret;
    QListWidgetItem *curItem1 = mySideBar->myMusicListWid->musicInfoWidget->itemAt(pos);

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

    playAct->setText(tr("play"));   //播放
    nextAct->setText(tr("The following piece"));   //下一首
    deleAct->setText(tr("Remove from the song list"));   //从歌曲列表中删除
    songAct->setText(tr("Song information"));    //歌曲信息

    QMenu *menu5 = new QMenu(this);
    menu5->setTitle(tr("Add to playlist"));    //添加到歌单
    QStringList playListNameList;

    ret = g_db->getPlayList(playListNameList);
    if(ret == DB_OP_SUCC)
    {
        for(int i = 0;i< playListNameList.size();i++)
        {

            QString listName = playListNameList.at(i);
            QAction *listaction = new QAction(this);
            listaction->setData(i);
            listaction->setText(listName);
            menu5->addAction(listaction);
        }
    }
    Menu->addMenu(menu5);
    Menu->addAction(deleAct);
    Menu->addAction(songAct);
    connect(menu5, SIGNAL(triggered(QAction*)), this, SLOT(add_music_to_songlist(QAction*)));
    connect(deleAct,&QAction::triggered,this,&MainWid::deleteMusicFromLocalList);
    connect(songAct,&QAction::triggered,this,&MainWid::getSongInfoAct);
    Menu->exec(QCursor::pos());
    delete Menu;
    delete playAct;
    delete nextAct;
    delete deleAct;
    delete songAct;
}

//void MainWid::on_historyWidget_customContextMenuRequested(const QPoint &pos)
//{
//    QListWidgetItem *curItem1 = myPlaySongArea->mybeforeList->beforePlayList->itemAt(pos);

//    if(curItem1 == NULL)
//    {
//        return;
//    }
//    historyMenu = new QMenu(myPlaySongArea->mybeforeList->beforePlayList);
//    playAction = new QAction(this);
//    nextAction = new QAction(this);
//    delAction = new QAction(this);

//    playAction->setText(tr("paly"));
//    nextAction->setText(tr("next"));
//    delAction->setText(tr("delete"));

//    historyMenu->addAction(playAction);
//    historyMenu->addAction(nextAction);
//    historyMenu->addAction(delAction);
//    connect(playAction,&QAction::triggered,this,&MainWid::historyPlay);
//    connect(nextAction,&QAction::triggered,this,&MainWid::historyNext);
//    connect(delAction,&QAction::triggered,this,&MainWid::historyDel);
//    historyMenu->exec(QCursor::pos());
//    delete historyMenu;
//    delete playAction;
//    delete nextAction;
//    delete delAction;
//}

// 右键播放暂停功能
void MainWid::playOrPauseAct()
{
    if(mySideBar->myMusicListWid->Music->playlist()==mySideBar->myMusicListWid->PlayList)
    {
        if(mySideBar->myMusicListWid->musicInfoWidget->item(mySideBar->myMusicListWid->PlayList->currentIndex())->isSelected())
        {

            if(mySideBar->myMusicListWid->Music->state() == QMediaPlayer::PlayingState)
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
    int ret;
    musicDataStruct musicInfo;
    int listindex = listact->data().toInt();

    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();

    QString filePath = mySideBar->myMusicListWid->localAllMusicid[row];
    ret = g_db->getSongInfoFromLocalMusic(filePath, musicInfo);
    if(ret != DB_OP_SUCC)
    {
        qDebug() << "从歌曲列表中获取歌曲信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return;
    }
    ret = g_db->addMusicToPlayList(filePath, listact->text());
    if(ret == DB_OP_SUCC)
    {
        QListWidgetItem *item1=new QListWidgetItem(mySideBar->musicListChangeWid[listindex]->musicInfoWidget);
        SongItem *songitem1 = new SongItem;
        mySideBar->musicListChangeWid[listindex]->musicInfoWidget->setItemWidget(item1,songitem1);
        songitem1->song_singer_albumText(musicInfo.title,musicInfo.singer,musicInfo.album); //歌曲名称 歌手 专辑
        songitem1->songTimeLabel->setText(musicInfo.time); //时长
        mySideBar->musicListChangeWid[listindex]->localAllMusicid.append(musicInfo.filepath);
        mySideBar->musicListChangeWid[listindex]->PlayList->addMedia(QUrl::fromLocalFile(musicInfo.filepath));
        mySideBar->musicListChangeWid[listindex]->musicInfoWidget->show();
        mySideBar->musicListChangeWid[listindex]->songNumberLabel->setText(
                    tr("A total of")+QString::number(mySideBar->musicListChangeWid[listindex]->musicInfoWidget->count())+tr("The first"));
    }

    else
    {
        QMessageBox::about(this,"提示信息","歌曲已存在");
    }
}

void MainWid::local_currentIndexChanged(int currentIndex)
{
    int ret;
    musicDataStruct musicInfo;

    QString filePath = mySideBar->myMusicListWid->localAllMusicid[currentIndex];
    ret = g_db->getSongInfoFromLocalMusic(filePath, musicInfo);
    myPlaySongArea->fileData = musicInfo;
    myPlaySongArea->index = currentIndex;
    if(ret == DB_OP_SUCC)
    {
        myPlaySongArea->songText(myPlaySongArea->fileData.title); // 正在播放
        m_MiniWidget->songText(myPlaySongArea->fileData.title);   //mini正在播放
        if(mySideBar->musicListChangeWid[0]->localAllMusicid.indexOf(myPlaySongArea->fileData.filepath) != -1)
        {
            myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
            m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
        }
        else
        {
            myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");
            m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");
        }
    }
}

void MainWid::playlist_currentIndexChanged(int currentIndex)
{
    int ret;
    musicDataStruct musicInfo;
    QString filePath = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->localAllMusicid[currentIndex];
    ret = g_db->getSongInfoFromPlayList(musicInfo, filePath, mySideBar->playListName[mySideBar->currentSelectList]);
    myPlaySongArea->fileData = musicInfo;
    myPlaySongArea->index = currentIndex;
    if(ret == DB_OP_SUCC)
    {
        myPlaySongArea->songText(myPlaySongArea->fileData.title); // 正在播放
        m_MiniWidget->songText(myPlaySongArea->fileData.title);   //mini正在播放
        for(int i = 0;i < mySideBar->playListName.size();i++)
        {
            if(mySideBar->musicListChangeWid[0]->localAllMusicid.indexOf(myPlaySongArea->fileData.filepath) != -1)
            {
                myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
            }
            else
            {
                myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
                m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");
            }
        }
    }
}

void MainWid::history_currentIndexChanged(int currentIndex)
{
    int ret;
    musicDataStruct musicInfo;

    QString filePath = myPlaySongArea->mybeforeList->historyMusicid[currentIndex];
    ret = g_db->getSongInfoFromHistoryMusic(filePath, musicInfo);
    myPlaySongArea->fileData = musicInfo;
    myPlaySongArea->index = currentIndex;
    if(ret == DB_OP_SUCC)
    {
        myPlaySongArea->songText(musicInfo.title); // 正在播放
        m_MiniWidget->songText(musicInfo.title);   //mini正在播放
        if(mySideBar->musicListChangeWid[0]->localAllMusicid.indexOf(myPlaySongArea->fileData.filepath) != -1)
        {
            myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
            m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
        }
        else
        {
            myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
            m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");
        }
    }
}

void MainWid::addLike()
{
    int ret;
    int ref;
    if(mySideBar->currentMusicPlaylist == -1)
    {
        ret = g_db->getSongInfoFromLocalMusic(myPlaySongArea->fileData.filepath, myPlaySongArea->fileData);
        if(ret == DB_OP_SUCC)
        {
            if(mySideBar->musicListChangeWid[0]->localAllMusicid.indexOf(myPlaySongArea->fileData.filepath) == -1)
            {
                ref = g_db->addMusicToPlayList(myPlaySongArea->fileData.filepath, mySideBar->playListName[0]);
                if(ref == DB_OP_SUCC)
                {
                    QListWidgetItem * item = new QListWidgetItem(mySideBar->musicListChangeWid[0]->musicInfoWidget);
                    SongItem *songitem = new SongItem;
                    mySideBar->musicListChangeWid[0]->musicInfoWidget->setItemWidget(item,songitem);
                    songitem->song_singer_albumText(myPlaySongArea->fileData.title,myPlaySongArea->fileData.singer,myPlaySongArea->fileData.album); //歌曲名称 歌手 专辑
                    songitem->songTimeLabel->setText(myPlaySongArea->fileData.time); //时长
                    mySideBar->musicListChangeWid[0]->localAllMusicid.append(myPlaySongArea->fileData.filepath);
                    mySideBar->musicListChangeWid[0]->PlayList->addMedia(QUrl::fromLocalFile(myPlaySongArea->fileData.filepath));
                    mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                    myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                    m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                }
            }
            else
            {
                ret = g_db->delMusicFromPlayList(myPlaySongArea->fileData.filepath, mySideBar->playListName[0]);
                if(ret == DB_OP_SUCC)
                {
                    for(int i = 0;i < mySideBar->musicListChangeWid[0]->localAllMusicid.count();i++)
                    {
                        if(mySideBar->musicListChangeWid[0]->localAllMusicid[i] == myPlaySongArea->fileData.filepath)
                        {
                            mySideBar->musicListChangeWid[0]->localAllMusicid.removeOne(myPlaySongArea->fileData.filepath);
                            mySideBar->musicListChangeWid[0]->musicInfoWidget->removeItemWidget(mySideBar->musicListChangeWid[0]->musicInfoWidget->item(i));
                            delete mySideBar->musicListChangeWid[0]->musicInfoWidget->item(i);
                            mySideBar->musicListChangeWid[0]->PlayList->removeMedia(i, i);
                        }
                    }
                    mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                    myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
                    m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                           "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                           "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");

                }
            }
        }
    }
    else if(mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        int currPlay;
        ret = g_db->getSongInfoFromPlayList(myPlaySongArea->fileData, myPlaySongArea->fileData.filepath, mySideBar->playListName[mySideBar->currentMusicPlaylist]);

        if(ret == DB_OP_SUCC)
        {
//            for(int i = 0;i < mySideBar->playListName.size();i++)
//            {
                if(mySideBar->musicListChangeWid[0]->localAllMusicid.indexOf(myPlaySongArea->fileData.filepath) == -1)
                {
                    ref = g_db->addMusicToPlayList(myPlaySongArea->fileData.filepath, mySideBar->playListName[0]);
                    if(ref == DB_OP_SUCC)
                    {
                        QListWidgetItem * item = new QListWidgetItem(mySideBar->musicListChangeWid[0]->musicInfoWidget);
                        SongItem *songitem = new SongItem;
                        mySideBar->musicListChangeWid[0]->musicInfoWidget->setItemWidget(item,songitem);
                        songitem->song_singer_albumText(myPlaySongArea->fileData.title,myPlaySongArea->fileData.singer,myPlaySongArea->fileData.album); //歌曲名称 歌手 专辑
                        songitem->songTimeLabel->setText(myPlaySongArea->fileData.time); //时长
                        mySideBar->musicListChangeWid[0]->localAllMusicid.append(myPlaySongArea->fileData.filepath);
                        mySideBar->musicListChangeWid[0]->PlayList->addMedia(QUrl::fromLocalFile(myPlaySongArea->fileData.filepath));
                        mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                    tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                        myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                        m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                    }
                }
                else
                {
                    ret = g_db->delMusicFromPlayList(myPlaySongArea->fileData.filepath, mySideBar->playListName[0]);
                    if(ret == DB_OP_SUCC)
                    {
                        for(int i = 0;i < mySideBar->musicListChangeWid[0]->localAllMusicid.count();i++)
                        {
                            if(mySideBar->musicListChangeWid[0]->localAllMusicid[i] == myPlaySongArea->fileData.filepath)
                            {
                                mySideBar->musicListChangeWid[0]->localAllMusicid.removeOne(myPlaySongArea->fileData.filepath);
                                mySideBar->musicListChangeWid[0]->musicInfoWidget->removeItemWidget(mySideBar->musicListChangeWid[0]->musicInfoWidget->item(i));
                                delete mySideBar->musicListChangeWid[0]->musicInfoWidget->item(i);

                                if(mySideBar->currentMusicPlaylist == 0)
                                {
                                    currPlay = mySideBar->musicListChangeWid[0]->PlayList->currentIndex();
                                    if (currPlay == i)
                                    {
                                        mySideBar->musicListChangeWid[0]->Music->stop();
                                        mySideBar->musicListChangeWid[0]->PlayList->removeMedia(i);
                                        mySideBar->musicListChangeWid[0]->Music->setPlaylist(mySideBar->musicListChangeWid[0]->PlayList);
                                        mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                                    tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                                        if(mySideBar->musicListChangeWid[0]->musicInfoWidget->count() > 0)
                                        {
                                            if(i == mySideBar->musicListChangeWid[0]->musicInfoWidget->count())
                                            {
                                                mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(0);
                                                playlist_currentIndexChanged(0);
                                            }
                                            else
                                            {
                                                mySideBar->musicListChangeWid[0]->PlayList->setCurrentIndex(i);
                                                playlist_currentIndexChanged(i);
                                            }
                                        }
                                        mySideBar->musicListChangeWid[0]->Music->play();
                                        if (mySideBar->musicListChangeWid[0]->musicInfoWidget->count() == 0)
                                        {
                                            mySideBar->musicListChangeWid[0]->Music->stop();
                                            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                                   "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                                   "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                                            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                                        "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                                        "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                                            myPlaySongArea->mybeforeList->isStartPlay = false;
                                            myPlaySongArea->songText("");
                                            myPlaySongArea->bottomLeftLabel->setText("");
                                            mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                                        tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                                        }
                                    }
                                    if(mySideBar->currentSelectList == 0)
                                    {
                                        myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                                        m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                                        if(mySideBar->musicListChangeWid[0]->musicInfoWidget->count() == 0)
                                        {
                                            myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                                        "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                                        "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
                                            m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                                   "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                                   "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");
                                        }
                                    }
                                    else
                                    {
                                        myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                                    "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                                    "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
                                        m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                               "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                               "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");
                                    }

                                }
                                else
                                {
                                    mySideBar->musicListChangeWid[0]->PlayList->removeMedia(i, i);
                                    mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                                tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                                    myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                                "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                                "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
                                    m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                           "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                           "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");
                                }
                            }
                        }
                    }
//                }
            }
        }
    }
    else
    {
        ret = g_db->getSongInfoFromHistoryMusic(myPlaySongArea->fileData.filepath, myPlaySongArea->fileData);
        if(ret == DB_OP_SUCC)
        {
            if(mySideBar->musicListChangeWid[0]->localAllMusicid.indexOf(myPlaySongArea->fileData.filepath) == -1)
            {
                ref = g_db->addMusicToPlayList(myPlaySongArea->fileData.filepath, mySideBar->playListName[0]);
                if(ref == DB_OP_SUCC)
                {
                    QListWidgetItem * item = new QListWidgetItem(mySideBar->musicListChangeWid[0]->musicInfoWidget);
                    SongItem *songitem = new SongItem;
                    mySideBar->musicListChangeWid[0]->musicInfoWidget->setItemWidget(item,songitem);
                    songitem->song_singer_albumText(myPlaySongArea->fileData.title,myPlaySongArea->fileData.singer,myPlaySongArea->fileData.album); //歌曲名称 歌手 专辑
                    songitem->songTimeLabel->setText(myPlaySongArea->fileData.time); //时长
                    mySideBar->musicListChangeWid[0]->localAllMusicid.append(myPlaySongArea->fileData.filepath);
                    mySideBar->musicListChangeWid[0]->PlayList->addMedia(QUrl::fromLocalFile(myPlaySongArea->fileData.filepath));
                    mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                    myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                    m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/clicked/love1.png);}");
                }
            }
            else
            {
                ret = g_db->delMusicFromPlayList(myPlaySongArea->fileData.filepath, mySideBar->playListName[0]);
                if(ret == DB_OP_SUCC)
                {
                    for(int i = 0;i < mySideBar->musicListChangeWid[0]->localAllMusicid.count();i++)
                    {
                        if(mySideBar->musicListChangeWid[0]->localAllMusicid[i] == myPlaySongArea->fileData.filepath)
                        {
                            mySideBar->musicListChangeWid[0]->localAllMusicid.removeOne(myPlaySongArea->fileData.filepath);
                            mySideBar->musicListChangeWid[0]->musicInfoWidget->removeItemWidget(mySideBar->musicListChangeWid[0]->musicInfoWidget->item(i));
                            delete mySideBar->musicListChangeWid[0]->musicInfoWidget->item(i);
                            mySideBar->musicListChangeWid[0]->PlayList->removeMedia(i, i);
                        }
                    }
                    mySideBar->musicListChangeWid[0]->songNumberLabel->setText(
                                tr("A total of")+QString::number(mySideBar->musicListChangeWid[0]->musicInfoWidget->count())+tr("The first"));
                    myPlaySongArea->likeMusicBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                                "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                                "QPushButton::pressed{border-image:url(:/img/clicked/love1h.png);}");
                    m_MiniWidget->m_loveBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/loveblack2.png);}"
                                                           "QPushButton::hover{border-image:url(:/img/clicked/love2.png);}"
                                                           "QPushButton::pressed{border-image:url(:/img/clicked/love1.png);}");

                }
            }
        }
    }
}

void MainWid::deleteThisSongList()
{
    mySideBar->promptSongListPup->stackWid->setCurrentIndex(1);
    mySideBar->promptSongListPup->pupDialog->show();
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

void MainWid::getPlayListStop()
{
    myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                           "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                           "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
    m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
    myPlaySongArea->mybeforeList->isStartPlay = false;
    myPlaySongArea->songText("");
    myPlaySongArea->bottomLeftLabel->setText("");
}

// 从歌曲列表里删除
void MainWid::deleteMusicFromLocalList()
{
    int ret;
    int currPlay;
    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
    QString filePath = mySideBar->myMusicListWid->localAllMusicid[row];

    ret = g_db->delMusicFromLocalMusic(filePath);
    if(ret == DB_OP_SUCC)
    {
        mySideBar->myMusicListWid->localAllMusicid.removeOne(filePath);
        mySideBar->myMusicListWid->musicInfoWidget->removeItemWidget(mySideBar->myMusicListWid->musicInfoWidget->item(row));
        delete mySideBar->myMusicListWid->musicInfoWidget->item(row);

        if (mySideBar->currentMusicPlaylist == mySideBar->currentSelectList) {
            currPlay = mySideBar->myMusicListWid->PlayList->currentIndex();
            if (currPlay == row)
            {
                mySideBar->myMusicListWid->Music->stop();
                mySideBar->myMusicListWid->PlayList->removeMedia(row);
                mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
                if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
                {
                    if(row == mySideBar->myMusicListWid->musicInfoWidget->count())
                    {
                        mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
                        local_currentIndexChanged(0);
                    }
                    else
                    {
                        mySideBar->myMusicListWid->PlayList->setCurrentIndex(row);
                        local_currentIndexChanged(row);
                    }
                }
                mySideBar->myMusicListWid->Music->play();
                if (mySideBar->myMusicListWid->musicInfoWidget->count() == 0)
                {
                    mySideBar->myMusicListWid->Music->stop();
                    myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                           "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                           "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                    m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                    myPlaySongArea->mybeforeList->isStartPlay = false;
                    myPlaySongArea->songText("");
                    myPlaySongArea->bottomLeftLabel->setText("");
                }
            }
            else if (currPlay > row)
            {
                int position = 0;
                if(mySideBar->myMusicListWid->Music->state()==QMediaPlayer::PlayingState)
                {
                    position=mySideBar->myMusicListWid->Music->position();
                }
                mySideBar->myMusicListWid->Music->stop();
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(0);
                mySideBar->myMusicListWid->PlayList->removeMedia(row, row);
                mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(currPlay - 1);
                mySideBar->myMusicListWid->Music->setPosition(position);
                hSlider->setValue(position);
                mySideBar->myMusicListWid->Music->play();
            }
        }
        else
        {
            mySideBar->myMusicListWid->PlayList->removeMedia(row, row);
        }
        mySideBar->myMusicListWid->songNumberLabel->setText(tr("A total of")+QString::number(mySideBar->myMusicListWid->musicInfoWidget->count())+tr("The first"));
    }
}

//歌曲信息
void MainWid::getSongInfoAct()
{
    int ret;
    musicDataStruct fileData;
    mySongInfoWidget = new SongInfoWidget;
    mySongInfoWidget->songInfoDlg->show();

    int row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();

    QString musicPath = mySideBar->myMusicListWid->localAllMusicid[row];

    ret = g_db->getSongInfoFromLocalMusic(musicPath, fileData);
    if(ret == DB_OP_SUCC)
    {
        mySongInfoWidget->titleLab ->setText(tr("The song name:"));  //歌曲名称
        mySongInfoWidget->artistLab->setText(tr("singer:"));         //歌曲歌手
        mySongInfoWidget->albumLab ->setText(tr("album:"));          //歌曲专辑
        mySongInfoWidget->typeLab  ->setText(tr("The file type:"));  //文件类型
        mySongInfoWidget->sizeLab  ->setText(tr("The file size:"));  //文件大小
        mySongInfoWidget->timeLab  ->setText(tr("File length:"));    //文件时长
        mySongInfoWidget->pathLab  ->setText(tr("File location:"));  //文件位置


        mySongInfoWidget->musicNameLab->setText(fileData.title);
        mySongInfoWidget->singerNameLab->setText(fileData.singer);
        mySongInfoWidget->albumNameLab->setText(fileData.album);
        mySongInfoWidget->fileTypeLab->setText(fileData.filetype);
        mySongInfoWidget->fileSizeLab->setText(fileData.size);
        mySongInfoWidget->fileTimeLab->setText(fileData.time);

        QString showpathStr   = "";
        if(fileData.filepath.length() > 30)
        {
            showpathStr = fileData.filepath.mid(0,29);
            showpathStr.append("...");
            mySongInfoWidget->filePathLab->setText(" "+showpathStr);
            mySongInfoWidget->filePathLab->setToolTip(" "+fileData.filepath);
        }
        else
        {
            mySongInfoWidget->filePathLab->setText(" "+fileData.filepath);
        }
    }
}

//void MainWid::historyPlay()
//{

//}

//void MainWid::historyNext()
//{

//}

//void MainWid::historyDel()
//{

//}

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

void MainWid::updatehistorywidget(int value)
{
    if (skipPlayHighlight)
    {
        return ;
    }

    myPlaySongArea->mybeforeList->beforePlayList->item(value)->setSelected(true);
    if(myPlaySongArea->mybeforeList->currentPlayIndex != -1)
    {
//        lastPlayIndex = myPlaySongArea->mybeforeList->PlayList->currentIndex();
        QWidget* lastitemWid = myPlaySongArea->mybeforeList->beforePlayList->itemWidget
                (myPlaySongArea->mybeforeList->beforePlayList->item
                (myPlaySongArea->mybeforeList->currentPlayIndex));
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
    myPlaySongArea->mybeforeList->currentPlayIndex = myPlaySongArea->mybeforeList->PlayList->currentIndex();
    QWidget* itemWid = myPlaySongArea->mybeforeList->beforePlayList->itemWidget
            (myPlaySongArea->mybeforeList->beforePlayList->item
            (myPlaySongArea->mybeforeList->currentPlayIndex));
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

void MainWid::on_listWidget_doubleClicked(QListWidgetItem *item)//双击本地音乐播放playlist
{
    int row;
    int ret;
    musicDataStruct fileData;
    QString musicPath;

    /* default cover */
    QPixmap default_photo = QPixmap(":/img/fengmian.png");
    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
    m_MiniWidget->m_coverLabel->setPixmap(default_photo);

    if (mySideBar->currentMusicPlaylist == -2) {
        mySideBar->currentMusicPlaylist = mySideBar->currentSelectList;
    }
    /* 检查是否切换了歌单 */
    if (mySideBar->currentMusicPlaylist != mySideBar->currentSelectList || myPlaySongArea->mybeforeList->currentMusicPlaylist == 20) {
        if(mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
        {
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->stop();
        }
        else if(myPlaySongArea->mybeforeList->currentMusicPlaylist == 20)
        {
            if(myPlaySongArea->mybeforeList->m_isMusicInitialed == true)
                myPlaySongArea->mybeforeList->Music->stop();
        }
        else
        {
            mySideBar->myMusicListWid->Music->stop();
        }
    }

    /* get music info */
    row = mySideBar->myMusicListWid->musicInfoWidget->currentIndex().row();
    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
    mySideBar->myMusicListWid->PlayList->setCurrentIndex(row);
    mySideBar->currentMusicPlaylist = -1;
    connect(mySideBar->myMusicListWid->PlayList, &QMediaPlaylist::currentIndexChanged, this,&MainWid::currentPlayHighlight,Qt::UniqueConnection);//因为在事件里，所以加一个参数，防止重复绑定

    //迷你模式正在播放
    /* to do */

    /* play area info */

    musicPath = mySideBar->myMusicListWid->localAllMusicid[row];
    ret = g_db->getSongInfoFromLocalMusic(musicPath, fileData);
    if(ret == DB_OP_SUCC)
    {
        mySideBar->myMusicListWid->Music->play();
        myPlaySongArea->songText(fileData.title); // 正在播放
        m_MiniWidget->songText(fileData.title);   //mini正在播放
    }
    else
    {
        qDebug()<<"-------从本地歌单中没有获取指定歌曲信息-------";
        return;
    }
    /* ===to do: add to history table */
    ret = g_db->addMusicToHistoryMusic(fileData.filepath);
    if (ret == DB_OP_SUCC) {
        QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
        HistoryListItem *besongitem1 = new HistoryListItem;
        myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
        myPlaySongArea->mybeforeList->historyMusicid.append(fileData.filepath);
        myPlaySongArea->mybeforeList->PlayList->addMedia(QUrl::fromLocalFile(fileData.filepath));
        besongitem1->song_singerText(fileData.title, fileData.singer); //历史列表
        besongitem1->songTimeLabel->setText(fileData.time); //时长
    }
    myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                           "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                           "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

    m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
    mySideBar->myMusicListWid->isStartPlay = true;
    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
}

// 双击歌单播放
void MainWid::on_musicListChangeWid_doubleClicked(QListWidgetItem *item)
{
    int row;
    int ret;
    musicDataStruct fileData;
    QString musicPath;
    /* default cover */
    QPixmap default_photo = QPixmap(":/img/fengmian.png");
    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
    m_MiniWidget->m_coverLabel->setPixmap(default_photo);

    if (mySideBar->currentMusicPlaylist == -2) {
        mySideBar->currentMusicPlaylist = mySideBar->currentSelectList;
    }

    /* 检查是否切换了歌单 */
    if(mySideBar->currentMusicPlaylist != mySideBar->currentSelectList)
    {
        if (mySideBar->currentMusicPlaylist == -1)
        {
            mySideBar->myMusicListWid->Music->stop();
        }
        else
        {
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->stop();
        }
    }
    if(myPlaySongArea->mybeforeList->currentMusicPlaylist == 20)
    {
        if(myPlaySongArea->mybeforeList->m_isMusicInitialed == true)
            myPlaySongArea->mybeforeList->Music->stop();
    }
    /* get music info */
    row = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->musicInfoWidget->currentIndex().row();
    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->setPlaylist(mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList);



    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(row);
    mySideBar->currentMusicPlaylist = mySideBar->currentSelectList;
//    connect(mySideBar->myMusicListWid->PlayList, &QMediaPlaylist::currentIndexChanged, this,&MainWid::currentPlayHighlight);

    //迷你模式正在播放
    /* to do */

    /* play area info */
    musicPath = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->localAllMusicid[row];
    ret = g_db->getSongInfoFromPlayList(fileData, musicPath, mySideBar->playListName[mySideBar->currentMusicPlaylist]);
    if(ret == DB_OP_SUCC)
    {
        mySideBar->musicListChangeWid[mySideBar->currentSelectList]->Music->play();
        myPlaySongArea->songText(fileData.title); // 正在播放
        m_MiniWidget->songText(fileData.title);   //mini正在播放
    }
    else
    {
        qDebug()<<"从本地歌单中没有获取指定歌曲信息";
        return;
    }

    /* ===to do: add to history table */
    ret = g_db->addMusicToHistoryMusic(fileData.filepath);
    if (ret == DB_OP_SUCC) {
        QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
        HistoryListItem *besongitem1 = new HistoryListItem;
        myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
        myPlaySongArea->mybeforeList->historyMusicid.append(fileData.filepath);
        myPlaySongArea->mybeforeList->PlayList->addMedia(QUrl::fromLocalFile(fileData.filepath));
        besongitem1->song_singerText(fileData.title, fileData.singer); //历史列表
        besongitem1->songTimeLabel->setText(fileData.time); //时长
    }
    myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                           "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                           "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

    m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
    mySideBar->myMusicListWid->isStartPlay = true;
    myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
}

void MainWid::on_historyWidget_doubleClicked(QListWidgetItem *item)
{
    int row;
    int ret;
    musicDataStruct fileData;
    QString musicPath;

    QPixmap default_photo = QPixmap(":/img/fengmian.png");
    default_photo = default_photo.scaled(40,40,Qt::KeepAspectRatio);
    myPlaySongArea->coverPhotoLabel->setPixmap(default_photo);
    m_MiniWidget->m_coverLabel->setPixmap(default_photo);

    if(mySideBar->currentMusicPlaylist == -2)
    {
        mySideBar->currentMusicPlaylist = myPlaySongArea->mybeforeList->currentMusicPlaylist;
        if(mySideBar->currentMusicPlaylist == -2)
        {
            myPlaySongArea->mybeforeList->currentMusicPlaylist = 20;
        }
    }
    if ( myPlaySongArea->mybeforeList->currentMusicPlaylist != 20 ||
         mySideBar->currentMusicPlaylist == -1 ||
         mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        if(mySideBar->currentMusicPlaylist == -1)
        {
            mySideBar->myMusicListWid->Music->stop();
        }
        else if(mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
        {
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->stop();
        }
        else
        {
            myPlaySongArea->mybeforeList->Music->stop();
        }
    }

    row =  myPlaySongArea->mybeforeList->beforePlayList->currentIndex().row();
    myPlaySongArea->mybeforeList->Music->setPlaylist(myPlaySongArea->mybeforeList->PlayList);

    myPlaySongArea->mybeforeList->PlayList->setCurrentIndex(row);
    myPlaySongArea->mybeforeList->currentMusicPlaylist = 20;
    mySideBar->currentMusicPlaylist = 20;

    musicPath = myPlaySongArea->mybeforeList->historyMusicid[row];
    ret = g_db->getSongInfoFromHistoryMusic(musicPath, fileData);
    if(ret == DB_OP_SUCC)
    {
        myPlaySongArea->mybeforeList->Music->play();
        myPlaySongArea->songText(fileData.title); // 正在播放
        m_MiniWidget->songText(fileData.title);   //mini正在播放
    }

    myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                           "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                           "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

    m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
    myPlaySongArea->mybeforeList->isStartPlay = true;
}

void MainWid::historyPositionChange(qint64 position)
{
    if (myPlaySongArea->mybeforeList->Music->state() == QMediaPlayer::PlayingState)
    {
//        hSlider->setMaximum(mySideBar->myMusicListWid->Music->duration() / 1000);//设置滑块的长度范围为音乐长.00度
        hSlider->setValue(static_cast<int>(position));
//        hSlider->setValue(position / 1000);//如果音乐进度改变，则改变滑块显示位置
//        moved = position;//让播放时间等于音乐进度（毫秒）
        QTime duration(0, static_cast<int>(position) / 60000, static_cast<int>((position % 60000) / 1000.0));
//        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
        QString str_time = duration.toString("mm:ss");
        QTime totalTime(0,(myPlaySongArea->mybeforeList->Music->duration()/60000) % 60,
                       (myPlaySongArea->mybeforeList->Music->duration() / 1000) % 60);
        myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

        //迷你模式时间输出
        m_MiniWidget->m_timeLab->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

        // 判断顺序播放模式下最后一首歌，播放完停止播放
        if (myPlaySongArea->mybeforeList->PlayList->playbackMode() == QMediaPlaylist::Sequential)
        {

            if (myPlaySongArea->mybeforeList->PlayList->currentIndex() == myPlaySongArea->mybeforeList->PlayList->mediaCount() - 1)
            {

                if (myPlaySongArea->mybeforeList->Music->state() == QMediaPlayer::PlayingState)
                {
                    if (myPlaySongArea->mybeforeList->Music->duration() > 0)
                    {
                        if (str_time == totalTime.toString("mm:ss")&&str_time != "00:00")
                        {
                            myPlaySongArea->mybeforeList->Music->stop();
                            position = 0;
                            hSlider->setValue(position);
                            myPlaySongArea->bottomLeftLabel->setText(tr("00:00") + "/" + totalTime.toString("mm:ss"));
                            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                   "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                   "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                        "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                        "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                            myPlaySongArea->mybeforeList->isStartPlay = false;
                        }
                    }
                }
            }
        }
    }
}

void MainWid::historyDurationChange(qint64 duration)
{
    hSlider->setRange(0,static_cast<int>(duration));
    hSlider->setEnabled(duration>0);
    hSlider->setPageStep(static_cast<int>(duration)/10);
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
        m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
//        myPlaySongArea->playBtn->setToolTip(tr("play"));
    }
}


void MainWid::on_lastBtn_clicked()             //上一首
{
    int preIndex = 0;
    int ret;
    QString musicPath;
    musicDataStruct fileData;
    if (mySideBar->currentMusicPlaylist == -2) {
        QMessageBox::about(this,"提示信息","请选择歌曲后再点击播放");
        return;
    }
    if (mySideBar->currentMusicPlaylist == -1) {
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
            preIndex = (mySideBar->myMusicListWid->PlayList->currentIndex() - 1 + mySideBar->myMusicListWid->PlayList->mediaCount()) % \
                    mySideBar->myMusicListWid->PlayList->mediaCount();
            mySideBar->myMusicListWid->PlayList->setCurrentIndex(preIndex);
            mySideBar->myMusicListWid->Music->play();

            /* 添加到历史列表 */
            musicPath = mySideBar->myMusicListWid->localAllMusicid[preIndex];
            ret = g_db->getSongInfoFromLocalMusic(musicPath, fileData);
            if(ret != DB_OP_SUCC)
            {
                qDebug() << "从本地歌单中获取歌曲信息失败";
                return;
            }
            ret = g_db->addMusicToHistoryMusic(fileData.filepath);
            if (ret == DB_OP_SUCC) {
                QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
                HistoryListItem *besongitem1 = new HistoryListItem;
                myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
                myPlaySongArea->mybeforeList->PlayList->addMedia(QUrl::fromLocalFile(fileData.filepath));
                besongitem1->song_singerText(fileData.title, fileData.singer); //历史列表
                besongitem1->songTimeLabel->setText(fileData.time); //时长
            }
            else
            {
                qDebug() << "从本地歌单中获取歌曲信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
            }
            myPlaySongArea->songText(fileData.title); // 正在播放
            m_MiniWidget->songText(fileData.title);   //mini正在播放
            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
            mySideBar->myMusicListWid->isStartPlay = true;
            myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                        tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
        }
    }
    else if(mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        if(mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->musicInfoWidget->count() > 0)
        {
            preIndex = (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->currentIndex() - 1 + \
                    mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->mediaCount()) % \
                    mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->mediaCount();
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->setCurrentIndex(preIndex);
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->play();

            /* 添加到历史列表 */
            musicPath = mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->localAllMusicid[preIndex];
            ret = g_db->getSongInfoFromPlayList(fileData, musicPath, mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->tableName);
            if(ret != DB_OP_SUCC)
            {
                qDebug() << "从歌单中获取歌曲信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                return;
            }

            ret = g_db->addMusicToHistoryMusic(fileData.filepath);
            if (ret == DB_OP_SUCC) {
                QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
                HistoryListItem *besongitem1 = new HistoryListItem;
                myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
                myPlaySongArea->mybeforeList->PlayList->addMedia(QUrl::fromLocalFile(fileData.filepath));
                besongitem1->song_singerText(fileData.title, fileData.singer); //历史列表
                besongitem1->songTimeLabel->setText(fileData.time); //时长
            }
            else
            {
                qDebug() << "从本地歌单中获取歌曲信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
            }
            myPlaySongArea->songText(fileData.title); // 正在播放
            m_MiniWidget->songText(fileData.title);   //mini正在播放
            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
            mySideBar->myMusicListWid->isStartPlay = true;
            myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                        tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
        }
    }
    else
    {
        if(myPlaySongArea->mybeforeList->beforePlayList->count() > 0)
        {
            preIndex = (myPlaySongArea->mybeforeList->PlayList->currentIndex() - 1 + myPlaySongArea->mybeforeList->PlayList->mediaCount()) % \
                    myPlaySongArea->mybeforeList->PlayList->mediaCount();
            myPlaySongArea->mybeforeList->PlayList->setCurrentIndex(preIndex);
            myPlaySongArea->mybeforeList->Music->play();

            /* 添加到历史列表 */
            musicPath = myPlaySongArea->mybeforeList->historyMusicid[preIndex];
            ret = g_db->getSongInfoFromHistoryMusic(musicPath, fileData);
            if(ret != DB_OP_SUCC)
            {
                qDebug() << "从历史歌单获取歌曲信息失败";
                return;
            }
            myPlaySongArea->songText(fileData.title); // 正在播放
            m_MiniWidget->songText(fileData.title);   //mini正在播放
            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
            myPlaySongArea->mybeforeList->isStartPlay = true;
        }
    }
}


void MainWid::on_nextBtn_clicked()      //下一首
{
    int nextIndex;
    int ret;
    QString musicPath;
    musicDataStruct fileData;
    if (mySideBar->currentMusicPlaylist == -2) {
        QMessageBox::about(this,"提示信息","请选择歌曲后再点击播放");
        return;
    }
    if (mySideBar->currentMusicPlaylist == -1) {
        if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
        {
        /* 歌曲列表下一首 */
        nextIndex = (mySideBar->myMusicListWid->PlayList->currentIndex() + 1)%mySideBar->myMusicListWid->PlayList->mediaCount();
        mySideBar->myMusicListWid->PlayList->setCurrentIndex(nextIndex);
        mySideBar->myMusicListWid->Music->play();

        /* 添加到历史列表 */
        musicPath = mySideBar->myMusicListWid->localAllMusicid[nextIndex];
        ret = g_db->getSongInfoFromLocalMusic(musicPath, fileData);
        if(ret != DB_OP_SUCC)
        {
            qDebug() << "从本地歌单中获取歌曲信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        }

        ret = g_db->addMusicToHistoryMusic(fileData.filepath);
        if (ret == DB_OP_SUCC) {
            QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
            HistoryListItem *besongitem1 = new HistoryListItem;
            myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
            myPlaySongArea->mybeforeList->historyMusicid.append(fileData.filepath);
            myPlaySongArea->mybeforeList->PlayList->addMedia(QUrl::fromLocalFile(fileData.filepath));
            besongitem1->song_singerText(fileData.title, fileData.singer); //历史列表
            besongitem1->songTimeLabel->setText(fileData.time); //时长
        }
        else
        {
            qDebug() << "添加歌曲信息到历史列表失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        }
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

        m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
        mySideBar->myMusicListWid->isStartPlay = true;
        myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                    tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
        }
    }
    else if(mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        if(mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->musicInfoWidget->count() > 0)
        {
            /* 歌单下一首 */
            nextIndex = (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->currentIndex() + 1) % mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->mediaCount();
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->setCurrentIndex(nextIndex);
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->play();

            /* 添加到历史列表 */
            musicPath = mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->localAllMusicid[nextIndex];
            ret = g_db->getSongInfoFromPlayList(fileData, musicPath, mySideBar->playListName[mySideBar->currentMusicPlaylist]);
            if(ret != DB_OP_SUCC)
            {
                qDebug() << "从本地歌单中获取歌曲信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
                return;
            }
            ret = g_db->addMusicToHistoryMusic(fileData.filepath);
            if (ret == DB_OP_SUCC) {
                QListWidgetItem *belistItem = new QListWidgetItem(myPlaySongArea->mybeforeList->beforePlayList);
                HistoryListItem *besongitem1 = new HistoryListItem;
                myPlaySongArea->mybeforeList->beforePlayList->setItemWidget(belistItem,besongitem1);
                myPlaySongArea->mybeforeList->historyMusicid.append(fileData.filepath);
                myPlaySongArea->mybeforeList->PlayList->addMedia(QUrl::fromLocalFile(fileData.filepath));
                besongitem1->song_singerText(fileData.title, fileData.singer); //历史列表
                besongitem1->songTimeLabel->setText(fileData.time); //时长
            }
            else
            {
                qDebug() << "添加歌曲信息到历史列表失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
            }
            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
            mySideBar->myMusicListWid->isStartPlay = true;
            myPlaySongArea->mybeforeList->beforeListNumberLabel->setText(
                        tr("A total of")+QString::number(myPlaySongArea->mybeforeList->beforePlayList->count())+tr("The first"));
        }
    }
    else
    {
        if(myPlaySongArea->mybeforeList->beforePlayList->count() > 0)
        {
            nextIndex = (myPlaySongArea->mybeforeList->PlayList->currentIndex() + 1) % myPlaySongArea->mybeforeList->PlayList->mediaCount();
            myPlaySongArea->mybeforeList->PlayList->setCurrentIndex(nextIndex);
            myPlaySongArea->mybeforeList->Music->play();

            /* 添加到历史列表 */
            musicPath = myPlaySongArea->mybeforeList->historyMusicid[nextIndex];
            ret = g_db->getSongInfoFromHistoryMusic(musicPath, fileData);
            if(ret != DB_OP_SUCC)
            {
                qDebug() << "从本地歌单中获取歌曲信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
            }
            myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                   "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                   "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

            m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                        "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                        "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
            myPlaySongArea->mybeforeList->isStartPlay = true;
        }
    }
}

void MainWid::positionChange(qint64 position)    //更新播放位置
{
    if(mySideBar->currentMusicPlaylist == -2)
    {
        return;
    }
    if(mySideBar->currentMusicPlaylist == -1)
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
                            if (str_time == totalTime.toString("mm:ss")&&str_time != "00:00")
                            {
                                mySideBar->myMusicListWid->Music->stop();
                                position = 0;
                                hSlider->setValue(position);
                                myPlaySongArea->bottomLeftLabel->setText(tr("00:00") + "/" + totalTime.toString("mm:ss"));

                                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                       "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                       "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                            "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                            "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                                mySideBar->myMusicListWid->isStartPlay = false;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWid::playlist_positionChange(qint64 position)
{
    if(mySideBar->currentMusicPlaylist == -2)
    {
        return;
    }
    if (mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        if (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->state() == QMediaPlayer::PlayingState)
        {
            //        hSlider->setMaximum(mySideBar->myMusicListWid->Music->duration() / 1000);//设置滑块的长度范围为音乐长.00度
            hSlider->setValue(static_cast<int>(position));
            //        hSlider->setValue(position / 1000);//如果音乐进度改变，则改变滑块显示位置
            //        moved = position;//让播放时间等于音乐进度（毫秒）
            QTime duration(0, static_cast<int>(position) / 60000, static_cast<int>((position % 60000) / 1000.0));
            //        QTime moveTime(0,(moved/60000) % 60,(moved / 1000) % 60);//设置时间
            QString str_time = duration.toString("mm:ss");
            QTime totalTime(0,(mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->duration()/60000) % 60,
                    (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->duration() / 1000) % 60);
            myPlaySongArea->bottomLeftLabel->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间
            //迷你模式时间输出
            m_MiniWidget->m_timeLab->setText(str_time + "/" + totalTime.toString("mm:ss"));//显示时间

            // 判断顺序播放模式下最后一首歌，播放完停止播放
            if (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->playbackMode() == QMediaPlaylist::Sequential)
            {
                if (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->currentIndex() ==
                        mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->mediaCount() - 1)
                {
                    if (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->state() == QMediaPlayer::PlayingState)
                    {
                        if (mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->duration() > 0)
                        {
                            if (str_time == totalTime.toString("mm:ss")&&str_time != "00:00")
                            {
                                mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->stop();
                                position = 0;
                                hSlider->setValue(position);
                                myPlaySongArea->bottomLeftLabel->setText(tr("00:00") + "/" + totalTime.toString("mm:ss"));
                                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                       "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                       "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                                            "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                                            "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
                                mySideBar->myMusicListWid->isStartPlay = false;
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWid::playlist_durationChange(qint64 duration)
{
    hSlider->setRange(0,static_cast<int>(duration));
    hSlider->setEnabled(duration>0);
    hSlider->setPageStep(static_cast<int>(duration)/10);
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
    if (mySideBar->currentMusicPlaylist == -1)
    {
        mySideBar->myMusicListWid->Music->pause();
    }
    else if (mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->pause();
    }
    else
    {
        myPlaySongArea->mybeforeList->Music->pause();
    }
}

void MainWid::slideRelease()   //滑动条鼠标弹起
{
    if(mySideBar->currentMusicPlaylist == -1)
    {
        mySideBar->myMusicListWid->Music->play();
    }
    else if (mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->play();
    }
    else
    {
        myPlaySongArea->mybeforeList->Music->play();
    }
}

void MainWid::setPosition(int position)
{
    if(mySideBar->currentMusicPlaylist == -1)
    {
        if (qAbs(mySideBar->myMusicListWid->Music->position() - position) > 99)
            mySideBar->myMusicListWid->Music->setPosition(position);
    }
    else if (mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        if (qAbs(mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->position() - position) > 99)
            mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->setPosition(position);
    }
    else
    {
        if (qAbs(myPlaySongArea->mybeforeList->Music->position() - position) > 99)
            myPlaySongArea->mybeforeList->Music->setPosition(position);
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

void MainWid::PlayModeChanged()
{
    if (mySideBar->currentMusicPlaylist == -1)
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
    else if (mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
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
                mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
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
                mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->setPlaybackMode(QMediaPlaylist::Loop);
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
                mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->setPlaybackMode(QMediaPlaylist::Random);
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
                mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
                break;
            default:
                break;
        }
    }
    else
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
                myPlaySongArea->mybeforeList->PlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
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
                myPlaySongArea->mybeforeList->PlayList->setPlaybackMode(QMediaPlaylist::Loop);
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
                myPlaySongArea->mybeforeList->PlayList->setPlaybackMode(QMediaPlaylist::Random);
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
                myPlaySongArea->mybeforeList->PlayList->setPlaybackMode(QMediaPlaylist::Sequential);
                break;
            default:
                break;
        }
    }
}



//显示历史播放列表
void MainWid::showBeforeList()
{
    if((myPlaySongArea->listBtn->isChecked()) == true)
    {
        //历史播放列表输出
        if(myPlaySongArea->mybeforeList->m_isMusicInitialed == false)
        {
            myPlaySongArea->mybeforeList->initialQMediaPlayer();
            connect(myPlaySongArea->mybeforeList->Music,SIGNAL(positionChanged(qint64)),this,SLOT(historyPositionChange(qint64)));  //滑块进度条位置改变
            connect(myPlaySongArea->mybeforeList->Music,SIGNAL(durationChanged(qint64)),this,SLOT(historyDurationChange(qint64)));
        }

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

    aboutWidget = new AllPupWindow(this);
    aboutWidget->resize(420,300);
    aboutWidget->cancelBtn->hide();

//    aboutWidget->hide();
    aboutWidget->pupDialog->show();
}
void MainWid::menuModuleSetThemeStyle(QString str)
{
    if("light-theme"==str)
    {
        changeItemColour();
        changeLightTheme();
    }
    else if("dark-theme"==str)
    {
        changeItemColour();
        changeDarkTheme();
    }
    else
        qDebug()<<"切换主题样式功能异常:"<<str;
}
void MainWid::changeItemColour()
{
    QList<SongItem *> list = this->findChildren<SongItem *>();
    //深色模式 || （跟随主题 && 深色主题）
    if(myTitleBar->menumodule->themeStatus == 1 || ( myTitleBar->menumodule->themeStatus == 0 && WidgetStyle::themeColor == 1 ))
    {
        songListItemColourType = 1 ;//浅色文字
    }
    else
        songListItemColourType = 0 ;
    //qDebug()<<"设置"<<myTitleBar->menumodule->themeStatus<<"主题"<<WidgetStyle::themeColor<<"字体"<<songListItemColourType;
    for(SongItem * item : list)
    {
        if(item->itemType==SongItem::highlightType)//跳过高亮
            continue;

        item->itemcolor(songListItemColourType);
    }
}

//切换深色主题
void MainWid::changeDarkTheme()
{
    int ret;
    QStringList playListNameList;
    WidgetStyle::themeColor = 1;
    mySideBar->newSonglistPup->dlgcolor();
    mySideBar->renameSongListPup->dlgcolor();
    mySideBar->sidecolor();
    myPlaySongArea->playcolor();
    m_MiniWidget->minicolor();
    myTitleBar->titlecolor();
//    mySideBar->myMusicListWid->musicInfoWidget->clear();     //主题切换清理musicInfoWidget引起闪退
//    mySideBar->myMusicListWid->get_localmusic_information();           jialin
    nullMusicWidget->nullWidgetColor();
    mySideBar->myMusicListWid->musiclistcolor();
//    mySideBar->songListBtncolor();

    ret = g_db->getPlayList(playListNameList);
    if(ret == DB_OP_SUCC)
    {
        for (int i = 0; i < playListNameList.size(); i++)
        {

            mySideBar->musicListChangeWid[i]->musiclistcolor();
//            mySideBar->musicListChangeWid[i]->musicInfoWidget->clear();
    //        mySideBar->get_listmusic_information(i, playListNameList.at(i));
            mySideBar->newSongListBtn[i]->setIcon(QIcon(":/img/default/songlist_w .png"));

            mySideBar->newSongListBtn[i]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                             font-size: 14px;\
                                            background-color:#1F2022;\
                                            color:#F9F9F9;border-radius:16px;}"
                                            "QToolButton::hover{background-color:#303032;border-radius:16px;}"
                                            "QToolButton::pressed{background-color:#303032;border-radius:16px;}");
        }
    }
    mainWidget->setStyleSheet("#mainWidget{background:#252526;}");
    rightWid->setStyleSheet("#rightWid{background:#252526;}");
    this->setStyleSheet("*MainWid{background:#252526;}");
}

//切换浅色主题
void MainWid::changeLightTheme()
{
    int ret;
    QStringList playListNameList;
    WidgetStyle::themeColor = 0;
    mySideBar->newSonglistPup->dlgcolor();
    mySideBar->renameSongListPup->dlgcolor();
    mySideBar->sidecolor();
    myPlaySongArea->playcolor();
    m_MiniWidget->minicolor();
    myTitleBar->titlecolor();
//    mySideBar->myMusicListWid->musicInfoWidget->clear();
//    mySideBar->myMusicListWid->get_localmusic_information();          jialin
    nullMusicWidget->nullWidgetColor();

    mySideBar->myMusicListWid->musiclistcolor();
    ret = g_db->getPlayList(playListNameList);
    if(ret == DB_OP_SUCC)
    {
        for (int i = 0; i < playListNameList.size(); i++)
        {
//            mySideBar->musicListChangeWid[i]->musicInfoWidget->clear();
    //        mySideBar->get_listmusic_information(i, playListNameList.at(i));
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
    }
    mainWidget->setStyleSheet("#mainWidget{background:#FFFFFF;}");
    rightWid->setStyleSheet("#rightWid{background:#FFFFFF;}");
    this->setStyleSheet("");
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

            qDebug()<<"musicInfoWidget count : "<<mySideBar->myMusicListWid->musicInfoWidget->count();
        }
    }
    mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
}

// 清除历史记录
void MainWid::clear_HistoryPlayList()
{
    int ret;
    QList<musicDataStruct> resList;

    ret = g_db->getSongInfoListFromHistoryMusic(resList);
//    myPlaySongArea->mybeforeList->beforePlayList->clear();
    if(myPlaySongArea->mybeforeList->Music->state() == QMediaPlayer::PlayingState)
    {
        myPlaySongArea->mybeforeList->Music->stop();
        myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                               "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                               "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/play2.png);}"
                                                    "QPushButton::hover{border-image:url(:/img/hover/play2.png);}"
                                                    "QPushButton::pressed{border-image:url(:/img/clicked/play2.png);}");
        myPlaySongArea->mybeforeList->isStartPlay = false;
        myPlaySongArea->songText("");
        myPlaySongArea->bottomLeftLabel->setText("");
    }
    if(ret == DB_OP_SUCC)
    {
        for (int i = 0; i < resList.size(); i++)
        {
            ret = g_db->delMusicFromHistoryMusic(resList.at(i).filepath);
            if(ret == DB_OP_SUCC)
            {
                myPlaySongArea->mybeforeList->historyMusicid.removeOne(resList.at(i).filepath);
                QListWidgetItem *item = myPlaySongArea->mybeforeList->beforePlayList->takeItem(0);
                delete item;
            }
        }
    }
    else
    {
        qDebug() << "清除历史记录失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return;
    }
    myPlaySongArea->mybeforeList->PlayList->clear();
    qDebug()<<myPlaySongArea->mybeforeList->PlayList->mediaCount();
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
    int thisrow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();
    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(thisrow);
    model_1->select();
//    int thisrow = mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->currentIndex();
//    mySideBar->musicListChangeWid[mySideBar->currentSelectList]->PlayList->setCurrentIndex(thisrow);
    qDebug()<<"currentSelectList : "<<mySideBar->currentSelectList;
//    qDebug() << thisrow << allmusicid.size();
//    if (thisrow  >= allmusicid.size() - 2) {
//        thisrow = 0;
//    }
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
            return ;
            }
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

    if(mySideBar->currentMusicPlaylist >= 0 && mySideBar->currentMusicPlaylist < 20)
    {
        mySideBar->musicListChangeWid[mySideBar->currentMusicPlaylist]->Music->stop();
    }
    if(myPlaySongArea->mybeforeList->currentMusicPlaylist == 20)
    {
        if(myPlaySongArea->mybeforeList->m_isMusicInitialed == true)
            myPlaySongArea->mybeforeList->Music->stop();
    }
    mySideBar->currentMusicPlaylist = -1;
    if(mySideBar->currentMusicPlaylist == -1)
    {
        int ret;
        if(!args.isEmpty())
        {
            qDebug()<<args;
            for(int i = 0; i < args.size(); i++)
            {
                if(args.at(i) == "")
                {
                    return ;
                }
                mySideBar->myMusicListWid->filepath(args.at(i));
                myPlaySongArea->fileData.filepath = args[i];
                mySideBar->myMusicListWid->fileInfo.setFile(mySideBar->myMusicListWid->musicdataStruct.filepath);
                mySideBar->myMusicListWid->fileType(mySideBar->myMusicListWid->fileInfo);          //文件类型
                mySideBar->myMusicListWid->fileSize(mySideBar->myMusicListWid->fileInfo);      //文件大小
                mySideBar->myMusicListWid->fileInformation(mySideBar->myMusicListWid->musicdataStruct.filepath);//获取歌曲文件信息
                ret = g_db->addMusicToLocalMusic(mySideBar->myMusicListWid->musicdataStruct);
                if (ret == DB_OP_SUCC) {
                    mySideBar->myMusicListWid->showFileInformation(mySideBar->myMusicListWid->musicdataStruct.title,mySideBar->myMusicListWid->musicdataStruct.singer,mySideBar->myMusicListWid->musicdataStruct.album,mySideBar->myMusicListWid->musicdataStruct.time);  //显示获取歌曲文件信息
                    mySideBar->myMusicListWid->localAllMusicid.append(mySideBar->myMusicListWid->musicdataStruct.filepath);
                    mySideBar->myMusicListWid->PlayList->addMedia(QUrl::fromLocalFile(mySideBar->myMusicListWid->musicdataStruct.filepath));
                    mySideBar->myMusicListWid->songNumberLabel->setText(tr("A total of")+QString::number(mySideBar->myMusicListWid->musicInfoWidget->count())+tr("The first"));
                } else {
                    qDebug()<<"歌曲已存在";
                }
            }
        }

        qDebug()<<"myPlaySongArea->fileData.filepath : "<<myPlaySongArea->fileData.filepath;
        for(int i = 0;i < mySideBar->myMusicListWid->localAllMusicid.count();i++)
        {
            if(mySideBar->myMusicListWid->localAllMusicid[i] == myPlaySongArea->fileData.filepath)
            {
                //mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(i);
                songListOutHightStyle(i);
                mySideBar->myMusicListWid->Music->play();
                myPlaySongArea->playBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                       "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                       "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");

                m_MiniWidget->m_playStateBtn->setStyleSheet("QPushButton{border-radius:17px;border-image:url(:/img/default/pause2.png);}"
                                                            "QPushButton::hover{border-image:url(:/img/hover/pause2.png);}"
                                                            "QPushButton::pressed{border-image:url(:/img/clicked/pause2.png);}");
                mySideBar->myMusicListWid->isStartPlay = true;
            }
        }
        if(mySideBar->myMusicListWid->localAllMusicid.indexOf(myPlaySongArea->fileData.filepath) == -1 && myPlaySongArea->fileData.filepath != "")
        {
            if(mySideBar->myMusicListWid->musicInfoWidget->count() > 0)
            {
                qDebug()<<"myPlaySongArea->fileData.filepath : "<<myPlaySongArea->fileData.filepath;
                int addMusicId = mySideBar->myMusicListWid->musicInfoWidget->count() - 1;
                mySideBar->myMusicListWid->Music->setPlaylist(mySideBar->myMusicListWid->PlayList);
                mySideBar->myMusicListWid->PlayList->setCurrentIndex(addMusicId);
                local_currentIndexChanged(addMusicId);
                mySideBar->myMusicListWid->Music->play();
            }
        }
    }
}

