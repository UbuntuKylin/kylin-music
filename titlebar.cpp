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

#include <QMenu>
#include <QFile>

#include "titlebar.h"


TitleBar::TitleBar(QWidget *parent) : QFrame(parent)
{
    setMouseTracking(true);
    initTitle();

    searchWidget = new QListWidget(parent);
    searchResultWidget = new MusicListWid(parent);
//    searchResultWidget->songListLabel->setText("搜索结果");
    searchResultWidget->songListLabel->setText(tr("The search results"));

    searchResultWidget->hide();

    searchWidget->setGeometry(270,42,200,180);

    searchWidget->hide();

    titlecolor();
}

void TitleBar::initTitle()
{
    setStyleSheet(::main_style);

    menumodule = new menuModule(this);
//    menumodule->menuButton
    resize(750,56);

    titleLayout = new QHBoxLayout(this);

    LayoutLeft = new QHBoxLayout;

    leftBtn = new QPushButton;
//    leftBtn->setIcon(QIcon::fromTheme("pan-start-symbolic"));

//    leftBtn->setStyleSheet("QPushButton{background:transparent;}");
    leftBtn->setStyleSheet("QPushButton{background:transparent;\
                           border-image:url(:/img/default/backoff.png);}");
    leftBtn->setFixedSize(16,16);
//    leftBtn->setStyleSheet()
//    leftBtn->setToolTip("后退");
    leftBtn->setCursor(Qt::PointingHandCursor);
    leftBtn->setToolTip(tr("back"));
    leftBtn->setDisabled(true);
    leftBtn->setContentsMargins(8,0,0,0);
    rightBtn = new QPushButton;
//    rightBtn->setIcon(QIcon::fromTheme("pan-end-symbolic"));
    rightBtn->setStyleSheet("QPushButton{background:transparent;}");
    rightBtn->setStyleSheet("QPushButton{background:transparent;\
                            border-image:url(:/img/default/forward.png);}");
    rightBtn->setFixedSize(16,16);
//    rightBtn->setToolTip("前进");
    rightBtn->setCursor(Qt::PointingHandCursor);
    rightBtn->setToolTip(tr("forward"));
    rightBtn->setDisabled(true);

    searchEdit = new QLineEdit;
    searchEdit->setFixedSize(200,32);
//    searchEdit->setPlaceholderText("搜索音乐，歌手");
    searchEdit->setPlaceholderText(tr("Search for music, singers"));

//    searchEdit->setContentsMargins(0,4,0,0);
    searchEdit->hide();
    searchBtn = new QPushButton(searchEdit);

    searchBtn->setFixedSize(16,16);
    searchBtn->setCursor(Qt::PointingHandCursor);
//    searchBtn->setIcon(QIcon::fromTheme("search-symbolic"));

    QMargins margins = searchEdit->textMargins();
    searchEdit->setTextMargins(margins.left()+10, margins.top(), searchBtn->width()+15, margins.bottom());

    searchLayout = new QHBoxLayout(searchEdit);
    searchLayout->addStretch();
    searchLayout->addWidget(searchBtn);
    searchLayout->setSpacing(0);
    //btn at right
    searchLayout->setContentsMargins(0,0,10,0);

    LayoutLeft->addWidget(leftBtn);
    LayoutLeft->addSpacing(16);
    LayoutLeft->addWidget(rightBtn);
    LayoutLeft->addSpacing(30);
    LayoutLeft->addWidget(searchEdit);
    LayoutLeft->setSpacing(8);
    LayoutLeft->setContentsMargins(30,0,0,0);

    LayoutRight = new QHBoxLayout;

    userIconBtn = new QPushButton;
    userIconBtn->setCursor(Qt::PointingHandCursor);
    userIconBtn->setFixedSize(20,20);
    userIconBtn->setStyleSheet("QPushButton{background:transparent;\
                               border-image:url(:/img/default/headportrait.png);}");

    loginBtn = new QPushButton;
//    loginBtn->setText("未登录");
    loginBtn->setText(tr("Not logged in"));
    loginBtn->setFlat(true);
    loginBtn->adjustSize();
    loginBtn->setCursor(Qt::PointingHandCursor);
    loginBtn->setStyleSheet("QPushButton{background:transparent;\
                            color:rgb(238,238,238);\
                            border:none;}");

    //for trigon list
    QMenu *loginMenu = new QMenu;
    loginBtn->setMenu(loginMenu);

    setBtn = new QPushButton;
    setBtn->setCursor(Qt::PointingHandCursor);
    setBtn->setFixedSize(30,30);
    setBtn->setIcon(QIcon::fromTheme("application-menu"));
    settingMenu = new QMenu(this);

    changeThemeColorMenu = new QMenu(this);
    darkThemeAct = new QAction(this);
    lightThemeAct = new QAction(this);
    aboutAct = new QAction(this);
    if(WidgetStyle::themeColor == 1)
    {
        settingMenu->setStyleSheet("QMenu{background-color:#303032;color:#F9F9F9;}"
                                   "QMenu::item:selected{background-color:#48484C;}");
        changeThemeColorMenu->setStyleSheet("QMenu{background-color:#303032;color:#F9F9F9;}"
                                            "QMenu::item:selected{background-color:#48484C;}");
    }
    else if(WidgetStyle::themeColor == 0)
    {
        settingMenu->setStyleSheet("QMenu{background-color:#FFFFFF;color:#303133;}"
                                   "QMenu::item:selected{background-color:#F7F7F7;}");
        changeThemeColorMenu->setStyleSheet("QMenu{background-color:#FFFFFF;color:#303133;}"
                                            "QMenu::item:selected{background-color:#F7F7F7;}");
    }

//    changeThemeColorMenu->setTitle("更换主题颜色");
    changeThemeColorMenu->setTitle(tr("Change the theme color"));
//    darkThemeAct->setText("深色模式");
    darkThemeAct->setText(tr("Dark mode"));
//    lightThemeAct->setText("浅色模式");
    lightThemeAct->setText(tr("Light color pattern"));
//    aboutAct->setText("关于");
    aboutAct->setText(tr("about"));

    settingMenu->addMenu(changeThemeColorMenu);
    changeThemeColorMenu->addAction(lightThemeAct);
    changeThemeColorMenu->addAction(darkThemeAct);
//    settingMenu->addAction(aboutAct);

    setBtn->setMenu(settingMenu);   //下拉三角图标

    connect(aboutAct,SIGNAL(triggered()),this,SLOT(showAboutWidget()));


    nullLabel = new QLabel;
    nullLabel->setPixmap(QPixmap(":/images/TitleBar/line.png"));

    miniBtn = new QPushButton;
    miniBtn->setCursor(Qt::PointingHandCursor);
    miniBtn->setFixedSize(30,30);
//    miniBtn->setToolTip("mini模式");
    miniBtn->setToolTip(tr("mini model"));
    miniBtn->setIcon(QIcon::fromTheme("ukui-mini"));

    minimumBtn = new QToolButton;
    minimumBtn->setCursor(Qt::PointingHandCursor);
//    minimumBtn->setToolTip(tr("最小化"));
    minimumBtn->setToolTip(tr("To minimize the"));
    minimumBtn->setFixedSize(30,30);
    minimumBtn->setIcon(QIcon::fromTheme("window-minimize-symbolic"));

    maximumBtn = new QToolButton;
    maximumBtn->setCursor(Qt::PointingHandCursor);
    maximumBtn->setFixedSize(30,30);
//    maximumBtn->setToolTip(tr("最大化"));
    maximumBtn->setToolTip(tr("maximize"));
    maximumBtn->setObjectName("maximumBtn");
    maximumBtn->setIcon(QIcon::fromTheme("window-maximize-symbolic"));


    closeBtn = new QToolButton;
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setFixedSize(30,30);
//    closeBtn->setToolTip("关闭");
    closeBtn->setToolTip(tr("close"));
    closeBtn->setIcon(QIcon::fromTheme("window-close-symbolic"));
    closeBtn->setProperty("isWindowButton", 0x2);
    closeBtn->setProperty("useIconHighlightEffect", 0x8);
    closeBtn->setAutoRaise(true);


//    connect(searchBtn,SIGNAL(clicked(bool)),this,SLOT(searchMusic()));
    connect(searchEdit,&QLineEdit::textChanged,this,&TitleBar::searchMusic);


    LayoutRight->setSpacing(0);
//    LayoutRight->addWidget(userIconBtn);
//    LayoutRight->addWidget(loginBtn);
//    LayoutRight->addSpacing(20);
    LayoutRight->addWidget(menumodule->menuButton);
    LayoutRight->addSpacing(4);
    LayoutRight->addWidget(miniBtn);
    LayoutRight->addSpacing(4);
    LayoutRight->addWidget(minimumBtn);
    LayoutRight->addSpacing(4);
    LayoutRight->addWidget(maximumBtn);
    LayoutRight->addSpacing(4);
    LayoutRight->addWidget(closeBtn);


    titleLayout->addLayout(LayoutLeft);
    titleLayout->addStretch();
    titleLayout->addLayout(LayoutRight);
//    titleLayout->setSpacing(0);
    titleLayout->setContentsMargins(0,4,4,0);

}

void TitleBar::searchMusic()
{

    QString enterStr = searchEdit->text().trimmed();
    qDebug() << "搜索" <<"输入栏" << enterStr;
    //    searchEdit->clear();
    if(enterStr == "")
    {
        searchWidget->close();
        return;
    }
    searchWidget->show();
    searchWidget->raise();

    searchWidget->clear();

    QSqlQuery searchQuery;
    QString searchStr = QString("select * from LocalMusic where musicname LIKE '%%1%'").arg(enterStr);
    qDebug() << "搜索" <<"数据库" << searchStr;
    searchQuery.exec(searchStr);
    while(searchQuery.next())
    {
        QString songName = searchQuery.value(1).toString();
        QString Path = searchQuery.value(2).toString();
        QString Title = searchQuery.value(3).toString();
        QString Album = searchQuery.value(4).toString();
        QString Time = searchQuery.value(7).toString();

        qDebug() << "搜索" << songName << Path ;

        QListWidgetItem *item1 = new QListWidgetItem(searchWidget);
        item1->setText(songName+" "+Title+" "+Album);
    }
}


void TitleBar::titlecolor()
{


    if (WidgetStyle::themeColor == 1)
    {
        settingMenu->setStyleSheet("QMenu{background-color:#303032;color:#F9F9F9;}"
                                   "QMenu::item:selected{background-color:#3D6BE5;}");
        changeThemeColorMenu->setStyleSheet("QMenu{background-color:#303032;color:#F9F9F9;}"
                                            "QMenu::item:selected{background-color:#3D6BE5;}");

        this->setStyleSheet("background-color:#252526;");
        searchWidget->setStyleSheet("QListWidget{border:5px;background:#3D3D41;color:#F9F9F9; }"
                                    "QListWidget::Item{height:30px;color:#F9F9F9;}"
                                    "QListWidget::Item:hover{background:#303032;color:#F9F9F9; }"
                                    "QListWidget::item:selected{background:#303032;color:#F9F9F9;}"
                                    );

        searchEdit->setStyleSheet("QLineEdit{border-radius:16px;background-color:#303032;color:#F9F9F9;}");

        searchBtn->setStyleSheet("QPushButton{background:transparent;\
                                 border-image:url(:/img/default/search.png);}\
                                 QPushButton::hover{border-image:url(:/img/hover/search.png);}");

        menumodule->menuButton->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                              }"
                              "QPushButton::hover{background:#D4DBE9;opacity:0.04;}"
                              "QPushButton::pressed{background:#BEBEBE;opacity:0.04;}"
                              "QPushButton::menu-indicator{image:None;}"
                              );


        miniBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                               }"
                               "QPushButton::hover{background:#D4DBE9;opacity:0.04;}"
                               "QPushButton::pressed{background:#BEBEBE;opacity:0.04;}");

        minimumBtn->setStyleSheet("QToolButton{background:transparent;border-radius:4px;\
                                  }"
                                  "QToolButton::hover{background:#D4DBE9;opacity:0.04;}"
                                  "QToolButton::pressed{background:#BEBEBE;opacity:0.04;}");

        maximumBtn->setStyleSheet("QToolButton{background:transparent;border-radius:4px;\
                                  }"
                                  "QToolButton::hover{background:#D4DBE9;opacity:0.04;}"
                                  "QToolButton::pressed{background:#D4DBE9;opacity:0.04;}"
                                   );

        closeBtn->setStyleSheet("QToolButton{background:transparent;border-radius:4px;}"
                                "QToolButton::hover{background:#FD9595;opacity:0.04}"
                                "QToolButton::pressed{background:#ED6464;}");

    }
    else if(WidgetStyle::themeColor == 0)
    {
        settingMenu->setStyleSheet("QMenu{background-color:#FFFFFF;color:#303133;}"
                                   "QMenu::item:selected{background-color:#3D6BE5;color:#FFFFFF;}");
        changeThemeColorMenu->setStyleSheet("QMenu{background-color:#FFFFFF;color:#303133;}"
                                            "QMenu::item:selected{background-color:#3D6BE5;color:#FFFFFF;}");


        this->setStyleSheet("background-color:#FFFFFF;");

        searchWidget->setStyleSheet("QListWidget{border:5px;color:#303133; }"
                                    "QListWidget::Item{height:30px;color:#303133;}"
                                    "QListWidget::Item:hover{background:#F7F7F7;color:#303133; }"
                                    "QListWidget::item:selected{background:#F7F7F7;color:#303133;}"
                                    );

        searchEdit->setStyleSheet("QLineEdit{border-radius:16px;background-color:#F6F6F6;color:#303133;}");

        searchBtn->setStyleSheet("QPushButton{background:transparent;\
                                 border-image:url(:/img/default/search.png);}\
                                 QPushButton::hover{border-image:url(:/img/hover/search.png);}");

        menumodule->menuButton->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                              }"
                              "QPushButton::hover{background:rgba(0,0,0,0.15);}"
                              "QPushButton::pressed{background:rgba(0, 0, 0, 0.2);}"
                              "QPushButton::menu-indicator{image:None;}"
                              );


        miniBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                               }"
                               "QPushButton::hover{background:rgba(0,0,0,0.15);}"
                               "QPushButton::pressed{background:rgba(0, 0, 0, 0.2);}");

        minimumBtn->setStyleSheet("QToolButton{background:transparent;border-radius:4px;}"
                                  "QToolButton::hover{background:rgba(0,0,0,0.15);}"
                                  "QToolButton::pressed{background:rgba(0, 0, 0, 0.2);}");

        maximumBtn->setStyleSheet("QToolButton{background:transparent;border-radius:4px;}"
                                  "QToolButton::hover{background-color:rgba(0,0,0,0.15);}"
                                  "QToolButton::pressed{background:rgba(0, 0, 0, 0.2);}"
                                   );
        //QPushButton:pressed{border-image:url(:/img/clicked/close.png);background:rgba(228, 76, 80, 1);}");
        closeBtn->setStyleSheet("QToolButton{border-radius:4px;}"
                                "QToolButton::hover{background-color:#F86457;}"
                                "QToolButton::pressed{background-color:#E44C50;}");
    }
}
