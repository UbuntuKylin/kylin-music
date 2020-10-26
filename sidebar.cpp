#include "sidebar.h"
#include <QDebug>

SideBar::SideBar(QWidget *parent) : QFrame(parent)
{
    setFixedWidth(210);

    this->setObjectName("SideBar");

    setStyleSheet("#SideBar{width: 210px;\
                  background: #FAFAFA;\
                  border-top-left-radius: 12px;border-bottom-left-radius:12px;}");

    initTopWidget();

    }


void SideBar::initTopWidget()
{

    QVBoxLayout *sideBarLayout = new QVBoxLayout(this);


    logoLabel = new QLabel(this);
//    logoLabel->setGeometry(40,20,100,30);
    logoLabel->setFixedSize(140,34);
    logoLabel->setStyleSheet("border-image:url(:/img/default/kylin.png);margin-left:30px;");

    recommendLabel = new QLabel(this);
    recommendLabel->setText("音乐库");
    recommendLabel->setGeometry(38,73,45,20);
    recommendLabel->setStyleSheet("font-size: 14px;margin-left:26px;\
                                  font-family: PingFangSC-Regular, PingFang SC;\
                                  font-weight: 400;\
                                  border:none;\
                                  color: #8F9399;\
                                  line-height: 14px;");

    PlayListBtn = new QToolButton(this);
//    PlayListBtn->setGeometry(16,99,180,32);
    PlayListBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    PlayListBtn->setIcon(QIcon(":/img/default/songlist_w .png"));
    PlayListBtn->setIconSize(QSize(16,16));
    PlayListBtn->setText("  歌曲列表");
    PlayListBtn->setFixedSize(180,32);
    PlayListBtn->setStyleSheet("QToolButton{padding-left:15px;margin-left:14px;\
                                font-size: 14px;\
                                font-family: PingFangSC-Regular;\
                                background:#FF7171;\
                                color:#FFFFFF;border-radius:16px;}"
                                "QToolButton::hover{background:#FF4848;}"
                                "QToolButton::pressed{background:#DD3C3C;}");

    QHBoxLayout *addSongListLayout = new QHBoxLayout(this);
    MySongListLabel = new QLabel(this);

    addSongListBtn = new QPushButton(this);
    MySongListLabel->setGeometry(38,161,56,16);
    MySongListLabel->setText("我的歌单");
    MySongListLabel->setStyleSheet("font-size: 14px;margin-left:26px;\
                                   font-family: PingFangSC-Regular, PingFang SC;\
                                   color: #8F9399;\
                                   line-height: 14px;");

//    addSongListBtn->setGeometry(100,161,16,16);
    addSongListBtn->setFixedSize(16,16);
//    addSongListBtn->setIcon(QIcon(":/img/default/add.png"));
    addSongListBtn->setStyleSheet("QPushButton{border-image:url(:/img/default/add.png);}\
                                  QPushButton:pressed{border-image:url(:/img/clicked/add.png);}");
    addSongListLayout->addWidget(MySongListLabel);
    addSongListLayout->addWidget(addSongListBtn);
    addSongListLayout->addSpacing(20);

    addSongListBtn->hide();   //添加歌单按钮隐藏

    favoriteMusicBtn = new QToolButton(this);
//    favoriteMusicBtn->setGeometry(24,187,162,32);
    favoriteMusicBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    favoriteMusicBtn->setIcon(QIcon(":/img/default/loveblack.png"));
    favoriteMusicBtn->setIconSize(QSize(16,16));
    favoriteMusicBtn->setText("  我喜欢");
    favoriteMusicBtn->setFixedSize(180,32);
//    favoriteMusicBtn->setStyleSheet("color:#303133;background:#FAFAFA;");
    favoriteMusicBtn->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                    font-size: 14px;\
                                    font-family: PingFangSC-Regular;\
                                    background-color:#FAFAFA;\
                                    color:#303133;border-radius:16px;}"
                                    "QToolButton::hover{background-color:#EEEEEE;border-radius:16px;}"
                                    "QToolButton::pressed{background-color:#EEEEEE;border-radius:16px;}");


    songListWidget = new QListWidget(this);
//    songListWidget->setFrameShape(QListWidget::NoFrame);
//    songListWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    songListWidget->setFocusPolicy(Qt::NoFocus);
    songListWidget->setStyleSheet("QListWidget{background-color:#FAFAFA;border:0px;}"
                                  "QListWidget::item:selected{background-color:#FAFAFA;}"
                                  "QListWidget::item:hover{background-color:#FAFAFA;}"
                                  "QListWidget::item:pressed{background-color:#FAFAFA;}");

    myLoveItem = new QListWidgetItem(songListWidget);
    myLoveItem->setSizeHint(QSize(180,32));

    songListWidget->setItemWidget(myLoveItem,favoriteMusicBtn);
//    for(int i = 0;i<10;i++)
//    {
//        newSongList[i] = new QListWidgetItem(songListWidget);
//        newSongListBtn[i] = new QToolButton(this);
//        newSongListBtn[i]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
//        newSongListBtn[i]->setIcon(QIcon(":/img/default/loveblack.png"));
//        newSongListBtn[i]->setIconSize(QSize(16,16));
//        newSongListBtn[i]->setText("  新建歌单");
//        newSongListBtn[i]->setFixedSize(180,32);
//        newSongList[i]->setSizeHint(QSize(180,32));

//    //    favoriteMusicBtn->setStyleSheet("color:#303133;background:#FAFAFA;");
//        newSongListBtn[i]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
//                                        font-size: 14px;\
//                                        font-family: PingFangSC-Regular;\
//                                        background-color:#FAFAFA;\
//                                        color:#303133;border-radius:16px;}"
//                                        "QToolButton::hover{background-color:#EEEEEE;border-radius:16px;}"
//                                        "QToolButton::pressed{background-color:#EEEEEE;border-radius:16px;}");


//        songListWidget->setItemWidget(newSongList[i],newSongListBtn[i]);
//        newSongList[i]->setHidden(true);
//    }



    qDebug() << "songListWidget->count()" << songListWidget->count();


    sideBarLayout->addWidget(logoLabel);
    sideBarLayout->addSpacing(15);
    sideBarLayout->addWidget(recommendLabel);
    sideBarLayout->addSpacing(5);
    sideBarLayout->addWidget(PlayListBtn);
    sideBarLayout->addSpacing(15);
    sideBarLayout->addLayout(addSongListLayout);
    sideBarLayout->addWidget(songListWidget);

    this->setLayout(sideBarLayout);



    addSongListDialog = new QDialog(this);
    addSongListDialog->setFixedSize(424,172);
    addSongListDialog->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    addSongListDialog->setStyleSheet("background-color:#FFFFFF;"
                                     "border-radius:6px;");
    QVBoxLayout *testLayout = new QVBoxLayout(addSongListDialog);
    QHBoxLayout *titleLayout = new QHBoxLayout(addSongListDialog);
    QLabel *titleAddList = new QLabel(addSongListDialog);
    QPushButton *closeDialog = new QPushButton(addSongListDialog);


    titleAddList->setText("新建歌单");
    titleAddList->setStyleSheet("width:80px;height:14px;font-size: 14px;\
                                font-family: PingFangSC-Regular, PingFang SC;\
                                font-weight: 600;\
                                border:none;\
                                color: #1B1B1B;\
                                line-height:14px;");
    closeDialog->setStyleSheet("QPushButton{width:30px;height:30px;background:transparent;border-radius:4px;;\
                               border-image:url(:/img/default/close.png);}\
                               QPushButton::hover{border-image:url(:/img/hover/close.png);\
                                                  background:#FD9595;opacity:0.04}\
                               QPushButton::pressed{border-image:url(:/img/clicked/close.png);background:#ED6464;}");

    titleLayout->addWidget(titleAddList,Qt::AlignLeft);
    titleLayout->addWidget(closeDialog,0,Qt::AlignRight);



    titleLayout->setMargin(0);
    titleLayout->setSpacing(0);

    QLineEdit *songListName = new QLineEdit(addSongListDialog);


    QPushButton *confirm = new QPushButton(addSongListDialog);

    QPushButton *cancel = new QPushButton(addSongListDialog);

    QHBoxLayout *btnLayout = new QHBoxLayout(addSongListDialog);
    confirm->setText("确认");
    confirm->setFixedSize(64,30);
    confirm->setStyleSheet("background-color:#FF4848;border-radius:4px;color:#FFFFFF;");
    cancel->setText("取消");
    cancel->setStyleSheet("border-radius:4px;border:1px solid #DDDFE7;color:#8F9399;");
    cancel->setFixedSize(64,30);

    btnLayout->addWidget(cancel, 0, Qt::AlignRight);
    btnLayout->addWidget(confirm, Qt::AlignRight);
    btnLayout->setSpacing(20);
    btnLayout->setMargin(0);

    songListName->setPlaceholderText("请输入歌单标题");
    songListName->setStyleSheet("width:392px;height:32px;border-radius:4px;border:1px solid #DDDFE7;");


    testLayout->addLayout(titleLayout);
    testLayout->addWidget(songListName);
    testLayout->addLayout(btnLayout);

    testLayout->setMargin(0);
    testLayout->setSpacing(20);

    addSongListDialog->setLayout(testLayout);
    addSongListDialog->setContentsMargins(10, 0, 10, 0);

    // 设置边框阴影
    QGraphicsDropShadowEffect *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setOffset(0, 0);
    shadowEffect->setColor(Qt::gray);
    shadowEffect->setBlurRadius(6);
    cancel->setGraphicsEffect(shadowEffect);


    rightChangeWid = new QStackedWidget(this);
    myMusicListWid = new MusicListWid(this);
    musicListChangeWid[0] = new MusicListWid(this);

    rightChangeWid->addWidget(myMusicListWid);

//    musicListChangeWid[0]->musicInfoWidget->clear();
    musicListChangeWid[0]->songListLabel->setText("我喜欢");
    musicListChangeWid[0]->count = 0;
    musicListChangeWid[0]->songNumberLabel->setText("共0首");
    musicListChangeWid[0]->musicInfoWidget->clear();
    musicListChangeWid[0]->PlayList->clear();
    musicListChangeWid[0]->Music->setPlaylist(musicListChangeWid[0]->PlayList);
//    musicListChangeWid[0]->sqlFilenameList.clear();

    QSqlQuery query;
    query.exec("select * from FavoriteMusic");  //本地音乐列表数据

    while(query.next())
    {
        QString Name = query.value(1).toString();
        QString FileName = query.value(2).toString();
//        QString mp3Name = musicListChangeWid[0]->getMp3FileName(Name);

//        musicListChangeWid[0]->sqlFilenameList.append(mp3Name);

        if (Name != "" && FileName != "")
        {
            QListWidgetItem *item1 = new QListWidgetItem;
//            item1->setIcon(QIcon(":/image/image/Music_32px_1144946_easyicon.net.png"));
//            item1->setSizeHint(QSize(700,40));
//            item1->setText(mp3Name);
            musicListChangeWid[0]->musicInfoWidget->addItem(item1);
            musicListChangeWid[0]->PlayList->addMedia(QUrl::fromLocalFile(FileName));
        }
    }
//    musicListChangeWid[0]->count = musicListChangeWid[0]->allmusic.size();
    musicListChangeWid[0]->songNumberLabel->setText("共"+QString::number(musicListChangeWid[0]->count)+"首");
//    qDebug()<<"sqlFilenameList.size() :"<<musicListChangeWid[0]->sqlFilenameList.size();
    musicListChangeWid[0]->Music->setPlaylist(musicListChangeWid[0]->PlayList);
    rightChangeWid->addWidget(musicListChangeWid[0]);

//    connect(songListWidget,SIGNAL(currentRowChanged(int)),this,SLOT(AlterPage(int)));
//    connect(songListWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(AlterPage(QListWidgetItem*)));
//    connect(favoriteMusicBtn,SIGNAL(clicked(bool)),this,SLOT(AlterthisPage()));

    connect(addSongListBtn, SIGNAL(clicked(bool)), this, SLOT(addSongList()));
    connect(closeDialog,SIGNAL(clicked(bool)),this,SLOT(dialogClosed()));
    connect(cancel,SIGNAL(clicked(bool)),this,SLOT(dialogClosed()));
    connect(confirm,SIGNAL(clicked(bool)),this,SLOT(createSongList()));

    connect(PlayListBtn,SIGNAL(clicked(bool)),this,SLOT(ChangePage()));









}
void SideBar::addSongList()
{
    addSongListDialog->show();

}

void SideBar::dialogClosed()
{
    addSongListDialog->close();

}

void SideBar::createSongList()
{
    int num = songListWidget->count();   //num为songListWidget页面中当前item索引值

    newSongList[num-1] = new QListWidgetItem(songListWidget);  //我喜欢 这一item占有一个索引，所以num作为控制新建歌单数组的下标变量时应该减去1
    newSongListBtn[num-1] = new QToolButton(this);
    newSongListBtn[num-1]->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    newSongListBtn[num-1]->setIcon(QIcon(":/img/default/songlist.png"));
    newSongListBtn[num-1]->setIconSize(QSize(16,16));
    newSongListBtn[num-1]->setText("  新建歌单" + QString::number(num));
    newSongListBtn[num-1]->setFixedSize(180,32);
    newSongList[num-1]->setSizeHint(QSize(180,32));

    //    favoriteMusicBtn->setStyleSheet("color:#303133;background:#FAFAFA;");
    newSongListBtn[num-1]->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                         font-size: 14px;\
                                        font-family: PingFangSC-Regular;\
                                        background-color:#FAFAFA;\
                                        color:#303133;border-radius:16px;}"
                                        "QToolButton::hover{background-color:#EEEEEE;border-radius:16px;}"
                                        "QToolButton::pressed{background-color:#EEEEEE;border-radius:16px;}");


    songListWidget->setItemWidget(newSongList[num-1],newSongListBtn[num-1]);

    musicListChangeWid[num+1] = new MusicListWid(this);

    rightChangeWid->addWidget(musicListChangeWid[num+1]);

    qDebug() << songListWidget->count() << "121111" << rightChangeWid->count();
    connect(newSongListBtn[num-1],SIGNAL(clicked(bool)),this,SLOT(AlterPage()));


    addSongListDialog->close();


}

void SideBar::ChangePage()
{
    rightChangeWid->setCurrentIndex(0);
    currentSelectList = 0;

    if (currentPlayList != 0)
    {
        // 歌曲列表失去焦点
        for (int i = 0; i < myMusicListWid->musicInfoWidget->count(); i++) {
            myMusicListWid->musicInfoWidget->item(i)->setSelected(false);
        }
    }

}

void SideBar::AlterPage()
{


//    rightChangeWid->setCurrentIndex(index+1);

    QToolButton *pButton = qobject_cast<QToolButton *>(sender());

    int btnIndex = 0;

    for (; btnIndex < 10; btnIndex++) {

        if (newSongListBtn[btnIndex] == pButton) {
            break;
        }
    }

    if (btnIndex < 10) {
        rightChangeWid->setCurrentIndex(btnIndex + 1);
    }
    else {
        rightChangeWid->setCurrentIndex(1);
        currentSelectList = 1;
        if (currentPlayList != 1)
        {
            // 我喜欢列表失去焦点
            for (int i = 0; i < musicListChangeWid[0]->musicInfoWidget->count(); i++) {
                musicListChangeWid[0]->musicInfoWidget->item(i)->setSelected(false);
            }
        }
    }

    qDebug() << "rightChangeWid   " << pButton->text() << "  " << btnIndex+1;

}

void SideBar::AlterthisPage()
{

    rightChangeWid->setCurrentIndex(1);
    currentSelectList = 1;
    qDebug() << "rightChangeWid   " << rightChangeWid->currentIndex();
    musicListChangeWid[0]->top_addSongBtn->hide();

    if (currentPlayList != 1)
    {
        // 我喜欢列表失去焦点
        for (int i = 0; i < musicListChangeWid[0]->musicInfoWidget->count(); i++) {
            musicListChangeWid[0]->musicInfoWidget->item(i)->setSelected(false);
        }
    }
}





