#include "sidebarwidget.h"
#include "base/musicDataBase.h"
#include "base/widgetstyle.h"

SideBarWidget::SideBarWidget(QWidget *parent) : QWidget(parent)
{
//    setFixedSize(210, 640);
    resize(210, 640);
    this->setObjectName("SideBarWidget");
    initWidget(); //初始化侧边栏界面
    initConnect(); //建立connect
    sidecolor();   //初始化样式

    getPlayListName();
}

void SideBarWidget::initWidget()
{
    //侧边栏界面样式
    QVBoxLayout *mainLayout = new QVBoxLayout();

    //logo
    QHBoxLayout *logoLayout = new QHBoxLayout();
    QLabel *logoNameLabel = new QLabel(this);
    logoNameLabel->setFixedSize(120,30);
    logoNameLabel->setText(tr("Kylin Music"));
    logoPushButton = new QPushButton(this);
    logoPushButton->setFixedSize(24,24);
    logoPushButton->setIconSize(QSize(24,24));
    logoLayout->addWidget(logoPushButton,Qt::AlignLeft);
    logoLayout->setSpacing(8);
    logoLayout->addWidget(logoNameLabel,0,Qt::AlignLeft);
    logoLayout->setContentsMargins(8,8,0,0);

    //音乐库
    QHBoxLayout *libraryLayout = new QHBoxLayout();
    libraryLabel = new QLabel(this);
    libraryLabel->setText(tr("Library"));
//    libraryLabel->setFixedSize(80,30);
    libraryLayout->addWidget(libraryLabel);
    libraryLayout->setContentsMargins(38,0,38,0);

    //歌曲列表
    QHBoxLayout *playListBtnLayout = new QHBoxLayout();
    playListBtn = new MyToolButton;
    playListBtn->setText(tr("Song List"));
    playListBtn->setStatusTip(IS_SELECT);
    playListBtnLayout->addWidget(playListBtn,Qt::AlignCenter);
//    playListBtnLayout->setContentsMargins(24,0,24,0);
    connect(playListBtn,&MyToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnCliced);

    //我的歌单
    QHBoxLayout *myPlayListLayout = new QHBoxLayout();
    myPlayListLabel = new QLabel(this);
    myPlayListBtn = new QPushButton(this);
    myPlayListLabel->setText(tr("My PlayList"));
    myPlayListBtn->setFixedSize(20,20);
    myPlayListBtn->setIcon(QIcon::fromTheme("list-add-symbolic"));
    myPlayListBtn->setProperty("isWindowButton", 0x1);
    myPlayListBtn->setProperty("useIconHighlightEffect", 0x2);
    myPlayListBtn->setFlat(true);

    myPlayListLayout->addWidget(myPlayListLabel);
    myPlayListLayout->addStretch();
    myPlayListLayout->addWidget(myPlayListBtn);
    myPlayListLayout->setContentsMargins(38,0,38,0);

    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setFrameStyle(0);

    //歌单界面
    newPlayListWidget = new QWidget();
    newPlayListLayout = new QVBoxLayout();

    scrollArea->setWidgetResizable(true);
//    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    newPlayListLayout->setAlignment(Qt::AlignTop);
    newPlayListWidget->setLayout(newPlayListLayout);
    newPlayListLayout->setContentsMargins(24,0,24,0);

    mainLayout->addLayout(logoLayout);
    mainLayout->addSpacing(16);
    mainLayout->addLayout(libraryLayout);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(playListBtnLayout);
    mainLayout->addSpacing(16);
    mainLayout->addLayout(myPlayListLayout);
    mainLayout->addSpacing(6);
    mainLayout->addWidget(newPlayListWidget);
    mainLayout->addStretch();
    mainLayout->setMargin(0);

    //新建歌单弹窗
    newSonglistPup = new AllPupWindow(this);
    newSonglistPup->titleLab->setText(tr("New Playlist"));
    newSonglistPup->pupDialog->hide();

    renameSongListPup = new AllPupWindow(this);
    renameSongListPup->titleLab->setText(tr("Rename"));
    renameSongListPup->pupDialog->hide();

    //侧边栏界面 嵌套 widget
    mainWid = new QWidget(this);
    mainWid->setLayout(mainLayout);
    mainWid->setObjectName("mainWid");
    scrollArea->setWidget(mainWid);

    //放置活动条
    QVBoxLayout *scrollLayout = new QVBoxLayout(this);
    scrollLayout->addWidget(scrollArea);
    scrollLayout->setMargin(0);
    this->setLayout(scrollLayout);

//    this->setAutoFillBackground(true);
//    this->setBackgroundRole(QPalette::Window);
}

void SideBarWidget::initConnect()
{
    connect(myPlayListBtn,&QPushButton::clicked,this,&SideBarWidget::addPlayList);
    connect(newSonglistPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(addItemToSongList()));
    connect(newSonglistPup->enterLineEdit,SIGNAL(returnPressed()),this,SLOT(addItemToSongList()));

    connect(renameSongListPup->confirmBtn,SIGNAL(clicked(bool)),this,SLOT(renamePlayList()));
    connect(renameSongListPup->enterLineEdit,SIGNAL(returnPressed()),this,SLOT(renamePlayList()));
}

void SideBarWidget::sidecolor()
{
    if(WidgetStyle::themeColor == 1)
    {
        QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
        for(MyToolButton *tmp : list)
        {
            tmp->defaultStyle();
        }
        mainWid->setStyleSheet("#mainWid{width: 210px;\
                      background: #1F2022;\
                      border-radius: 4px;}");
        logoPushButton->setStyleSheet("border-image:url(:/img/kylin-music-logo.png);");
        myPlayListLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
        libraryLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
        for(MyToolButton *tmp : list)
        {
            tmp->defaultStyle();
        }
        mainWid->setStyleSheet("#mainWid{width: 210px;\
                      background: #FAFAFA;\
                      border-radius: 4px;}");
        logoPushButton->setStyleSheet("border-image:url(:/img/kylin-music-logo.png);");
        myPlayListLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
        libraryLabel->setStyleSheet("color: #8F9399;line-height: 14px;");
    }
}

void SideBarWidget::getPlayListName()
{
    int ret = g_db->getPlayList(playListName);
    if(ret != DB_OP_SUCC)
    {
        qDebug() << playListName;
        qDebug() << "获取歌单名信息失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
        return;
    }
    QString listName;
    for(int i = 0; i < playListName.size();i++)
    {
        MyToolButton *newBtn = new MyToolButton;
        newPlayListLayout->setAlignment(Qt::AlignTop);
        newPlayListLayout->addWidget(newBtn);
    //    newPlayListLayout->setContentsMargins(24,0,24,0);
        newPlayListLayout->setSpacing(6);
        listName = playListName.at(i);
        newBtn->setText(listName);
        connect(newBtn,SIGNAL(playall(QString)),this,SLOT(playAll(QString)));
        connect(newBtn,SIGNAL(renamePlayList(QString)),this,SLOT(rename(QString)));
        connect(newBtn,SIGNAL(removePlayList(QString)),this,SLOT(removePlayList(QString)));
        connect(newBtn,&MyToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnCliced);
    }
}

void SideBarWidget::addPlayList()
{
    newSonglistPup->enterLineEdit->clear();
    newSonglistPup->pupDialog->show();
    newSonglistPup->enterLineEdit->setFocus();
}

void SideBarWidget::addItemToSongList()
{
    MyToolButton *newBtn = new MyToolButton();
//    QIcon icon = QIcon(":/img/default/songlist.png");
//    newBtn->setIcon(QIcon(":/img/default/songlist.png"));
//    newBtn->setIcon(QIcon::fromTheme("stock_contact-list"));
    newPlayListLayout->setAlignment(Qt::AlignTop);
    newPlayListLayout->addWidget(newBtn);
//    newPlayListLayout->setContentsMargins(24,0,24,0);
    newPlayListLayout->setSpacing(6);
    QString text = newSonglistPup->enterLineEdit->text();
    if(text != "")
    {
        for(int i = 0;i< playListName.size();i++)
        {
            if(playListName.at(i) == text)
            {
                newSonglistPup->pupDialog->hide();
        //        QMessageBox::about(this,tr("提示信息"),tr("歌单名已存在！！！"));
                QMessageBox::about(this,tr("Prompt information"),tr("Single song name already exists!!!"));
                return ;
            }
        }

        newBtn->setText(text);
        playListName.append(text);
        newSonglistPup->pupDialog->hide();
        g_db->createNewPlayList(text);
        emit playListAdded(text);
    }
    else
    {
        QString listName = newPlayListName();
        newBtn->setText(listName);
        playListName.append(listName);
        newSonglistPup->pupDialog->hide();
        g_db->createNewPlayList(listName);
        emit playListAdded(listName);
    }
    connect(newBtn,SIGNAL(playall(QString)),this,SLOT(playAll(QString)));
    connect(newBtn,SIGNAL(renamePlayList(QString)),this,SLOT(rename(QString)));
    connect(newBtn,SIGNAL(removePlayList(QString)),this,SLOT(removePlayList(QString)));
    connect(newBtn,&MyToolButton::selectButtonChanged,this,&SideBarWidget::playListBtnCliced);

}

QString SideBarWidget::newPlayListName()
{
    QMap<QString, QString> exitsName;
    for (auto &name : playListName)
    {
        exitsName.insert(name, name);
    }

    QString name = tr("New Playlist");
    if(!exitsName.contains(name))
    {
        return name;
    }

    int i = 1;
    for(i = 1; i < exitsName.size() + 1; i++)
    {
        QString newName = QString("%1 %2").arg(name).arg(i);
        if(!exitsName.contains(newName))
        {
            return newName;
        }
    }
    return QString("%1 %2").arg(name).arg(i);
}

void SideBarWidget::playAll(QString btnText)
{

}

void SideBarWidget::rename(QString text)
{
    btnText = text;
    renameSongListPup->enterLineEdit->clear();
    renameSongListPup->pupDialog->show();
    renameSongListPup->enterLineEdit->setFocus();
}

void SideBarWidget::renamePlayList()
{
    QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
    for(MyToolButton *tmp : list)
    {
        if(tmp->text() ==  btnText)
        {
            QString text = renameSongListPup->enterLineEdit->text();
            if(text != "")
            {
                for(int i = 0;i< playListName.size();i++)
                {
                    if(playListName.at(i) == text)
                    {
                        newSonglistPup->pupDialog->hide();
                //        QMessageBox::about(this,tr("提示信息"),tr("歌单名已存在！！！"));
                        QMessageBox::about(this,tr("Prompt information"),tr("Single song name already exists!!!"));
                        return ;
                    }
                }

                tmp->setText(text);
                for(int i = 0; i < playListName.size();i++)
                {
                    if(btnText == playListName[i])
                    {
                        playListName[i] = text;
                    }
                }
                emit playListRenamed(btnText,text);   //fff
                renameSongListPup->pupDialog->hide();
            }
            else
            {
                QString listName = btnText;
                tmp->setText(listName);
                renameSongListPup->pupDialog->hide();
            }
        }
    }
}

void SideBarWidget::removePlayList(QString text)
{
    QList<MyToolButton *> list = this->findChildren<MyToolButton *>();
    MyToolButton *tmp = nullptr;

     for(auto i = list.begin();i < list.end();i++)
     {
         if((*i)->text() == text)
         {
             if(i+1 == list.end())
             {
                tmp = *(i-1);
                tmp->click();
             }
             else
             {
                 tmp = *(i+1);
                 tmp->click();
             }
             (*i)->deleteLater();
             g_db->delPlayList(text);
         }
     }
    emit playListRemoved(text);
//    promptSongListPup->pupDialog->hide();
}
