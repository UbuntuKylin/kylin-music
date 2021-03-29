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
#include "musicDataBase.h"
#include "beforeplaylist.h"
#include "songitem.h"
#include <QGraphicsDropShadowEffect>

BeforePlayList::BeforePlayList(QWidget *parent):QWidget(parent)
{

    setGeometry(640,0,320,562);
//    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background:#FFFFFF;");

    initAction(); //初始化连接
    initUi();   //初始化样式
    historyItemColor();
}

void BeforePlayList::initUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *titleLayout = new QHBoxLayout(this);

    beforeListTitleLabel = new QLabel(this);
//    beforeListTitleLabel->setText("播放列表");
    beforeListTitleLabel->setText(tr("The playlist"));
    beforeListTitleLabel->setFixedHeight(30);
    beforeListTitleLabel->setStyleSheet("width: 72px;margin-top:2px;\
                                        height: 18px;\
                                        font-size: 18px;\
                                        font-weight: 600;\
                                        color: #303133;\
                                        line-height: 18px;\  ");


    beforeListNumberLabel = new QLabel(this);
    beforeListNumberLabel->setStyleSheet("width: 37px;margin-top:10px;\
                                         height: 14px;\
                                         font-size: 14px;\
                                         font-weight: 400;\
                                         color: #8F9399;\
                                         line-height: 14px;\
                                         ");

    beforeListNumberLabel->setText(
        tr("A total of")+QString::number(beforePlayList->count())+tr("The first"));
    emptyBtn = new QToolButton(this);
    emptyBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    emptyBtn->setIcon(QIcon(":/img/default/delete.png"));
    emptyBtn->setIconSize(QSize(16,16));
//    emptyBtn->setText("清空");
    emptyBtn->setText(tr("empty"));
    emptyBtn->setStyleSheet("QToolButton{font-size: 14px;margin-top:6px;\
                            font-weight: 400;\
                            color: #8F9399;\
                            line-height: 14px;}"
                            "QToolButton::hover{background-color:#FFFFFF;border-radius:16px;}"
                            "QToolButton::pressed{background-color:#FFFFFF;border-radius:16px;}"
                            );

    QWidget *titleWidget = new QWidget(this);
    titleWidget->setFixedHeight(45);
    titleWidget->setStyleSheet("background-color:#FFFFFF;");

    titleLayout->addWidget(beforeListTitleLabel,0,Qt::AlignLeft);
    titleLayout->addWidget(beforeListNumberLabel,Qt::AlignLeft);
    titleLayout->addStretch();
    titleLayout->addWidget(emptyBtn,0,Qt::AlignRight);
    titleWidget->setLayout(titleLayout);


    mainLayout->addWidget(titleWidget,0,Qt::AlignTop);
    mainLayout->addWidget(beforePlayList);
    mainLayout->setSpacing(0);

    this->setLayout(mainLayout);
//    mainWidget->raise();
//    this->raise();
}

void BeforePlayList::initAction()
{
    PlayList = new QMediaPlaylist(this);
    Music = new QMediaPlayer(this) ;
    /* 初始化历史列表 */
    beforePlayList = new QListWidget(this);
    beforePlayList->setContentsMargins(16,0,16,0);
//    beforePlayList->setStyleSheet("QListWidget{background-color:#FFFFFF;border:0px;}"
//                                  "QListWidget::item{height:40px;}"
//                                  "QListWidget::item:selected{background-color:#FFFFFF;color:#FF4848;}"
//                                  "QListWidget::item:hover{background-color:#FFFFFF;}"
//                                  );
    QList<musicDataStruct> resList;
    int ret;
    historyMusicid.clear();

    ret = g_db->getSongInfoListFromHistoryMusic(resList);
    if(ret == DB_OP_SUCC)
    {
        for (int i = 0; i < resList.size(); i++)
        {
            QListWidgetItem *belistItem = new QListWidgetItem(beforePlayList);
            HistoryListItem *besongitem1 = new HistoryListItem;
            beforePlayList->setItemWidget(belistItem,besongitem1);
            historyMusicid.append(resList.at(i).filepath);
            besongitem1->song_singerText(resList.at(i).title, resList.at(i).singer); //历史列表
            besongitem1->songTimeLabel->setText(resList.at(i).time); //时长
            PlayList->addMedia(QUrl::fromLocalFile(resList.at(i).filepath));
        }
    }
    //历史列表右键菜单
    beforePlayList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(beforePlayList,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(on_historyWidget_customContextMenuRequested(const QPoint&)));
}

void BeforePlayList::on_historyWidget_customContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *curItem1 = beforePlayList->itemAt(pos);

    if(curItem1 == NULL)
    {
        return;
    }
    historyMenu = new QMenu(beforePlayList);
    playAction = new QAction(this);
    nextAction = new QAction(this);
    delAction = new QAction(this);

    playAction->setText(tr("play"));
    nextAction->setText(tr("next"));
    delAction->setText(tr("delete"));

//    historyMenu->addAction(playAction);
//    historyMenu->addAction(nextAction);
//    historyMenu->addAction(delAction);
    connect(playAction,&QAction::triggered,this,&BeforePlayList::historyPlay);
    connect(nextAction,&QAction::triggered,this,&BeforePlayList::historyNext);
    connect(delAction,&QAction::triggered,this,&BeforePlayList::historyDel);
    historyMenu->exec(QCursor::pos());
    delete historyMenu;
    delete playAction;
    delete nextAction;
    delete delAction;
}

void BeforePlayList::historyPlay()
{

}

void BeforePlayList::historyNext()
{

}

void BeforePlayList::historyDel()
{

}

void BeforePlayList::historyItemColor()
{
    if(WidgetStyle::themeColor == 1 )
    {
        this->setObjectName("beforeplaylist");
        this->setStyleSheet("#beforeplaylist{background-color:#252526;}");
        beforePlayList->setStyleSheet("QListWidget{background-color:#252526;border:0px;}"
                                      "QListWidget::item{height:40px;}"
                                      "QListWidget::item:selected{background-color:#303032;}"
                                      "QListWidget::item:hover{background-color:#303032;}");

    }
    else if (WidgetStyle::themeColor == 0)
    {
        this->setObjectName("beforeplaylist");
        this->setStyleSheet("#beforeplaylist{background-color:#FFFFFF;}");

        beforePlayList->setStyleSheet("QListWidget{background-color:#FFFFFF;border:0px;}"
                                      "QListWidget::item{height:40px;}"
                                      "QListWidget::item:pressed{background-color:#F0F0F0;}"
                                      "QListWidget::item:selected{background-color:#F0F0F0;}"
                                      "QListWidget::item:hover{background-color:#F0F0F0;}");
    }
}
