#include <QMenu>
#include <QFile>

#include "titlebar.h"


TitleBar::TitleBar(QWidget *parent) : QFrame(parent)
{
    setMouseTracking(true);
    initTitle();


}


//控制最大化、最小化、关闭按钮

void TitleBar::onClicked()
{
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());
    QWidget *pWindow = this->window();
    if (pWindow->isTopLevel())
    {
        if (pButton == minimumBtn)
        {
            pWindow->showMinimized();
        }
        else if (pButton == maximumBtn)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();

        }

        else if (pButton == closeBtn)
        {
            pWindow->close();
        }

    }
}

void TitleBar::initTitle()
{
    setStyleSheet(::main_style);

//    setMinimumSize(1000,50);
//    setMaximumSize(1920,50);
    resize(750,56);

    titleLayout = new QHBoxLayout(this);

    titleIconLabel = new QLabel;
    titleIconLabel->setFixedWidth(114);
    titleIconLabel->setCursor(Qt::PointingHandCursor);
    titleIconLabel->setStyleSheet("border-image:url(:/img/default/kylin.png);");



    LayoutLeft = new QHBoxLayout;

    leftBtn = new QPushButton;
    leftBtn->setStyleSheet("QPushButton{background:transparent;\
                           border-image:url(:/img/default/backoff.png);}");
    leftBtn->setFixedSize(16,16);
    leftBtn->setToolTip("后退");
    leftBtn->setDisabled(true);

    rightBtn = new QPushButton;
    rightBtn->setStyleSheet("QPushButton{background:transparent;\
                            border-image:url(:/img/default/forward.png);}");
    rightBtn->setFixedSize(16,16);
    rightBtn->setToolTip("前进");
    rightBtn->setDisabled(true);

    searchEdit = new QLineEdit;
    searchEdit->setStyleSheet("QLineEdit{border-radius:16px;background-color:#F6F6F6;color:#BBBDCA;}");
    searchEdit->setFixedSize(200,32);
    searchEdit->setPlaceholderText("搜索音乐，歌手");

    searchBtn = new QPushButton(searchEdit);
    searchBtn->setStyleSheet("QPushButton{background:transparent;\
                             border-image:url(:/img/default/search.png);}\
                             QPushButton::hover{border-image:url(:/img/hover/search.png);}");
    searchBtn->setFixedSize(16,16);
    searchBtn->setCursor(Qt::PointingHandCursor);


    QMargins margins = searchEdit->textMargins();
    searchEdit->setTextMargins(margins.left()+10, margins.top(), searchBtn->width()+15, margins.bottom());

    searchLayout = new QHBoxLayout(searchEdit);
    searchLayout->addStretch();
    searchLayout->addWidget(searchBtn);
    searchLayout->setSpacing(0);
    //btn at right
    searchLayout->setContentsMargins(0,0,10,0);



    LayoutLeft->addWidget(leftBtn);
    LayoutLeft->addSpacing(10);
    LayoutLeft->addWidget(rightBtn);
    LayoutLeft->addSpacing(10);
    LayoutLeft->addWidget(searchEdit);
    LayoutLeft->setSpacing(0);

    LayoutRight = new QHBoxLayout;

    userIconBtn = new QPushButton;
    userIconBtn->setCursor(Qt::PointingHandCursor);
    userIconBtn->setFixedSize(20,20);
    userIconBtn->setStyleSheet("QPushButton{background:transparent;\
                               border-image:url(:/img/default/headportrait.png);}");

    loginBtn = new QPushButton;
    loginBtn->setText("未登录");
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
    setBtn->setFixedSize(32,32);
    setBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                          border-image:url(:/img/default/menu.png);}"
                          "QPushButton::hover{border-image:url(:/img/hover/menu.png);background:#D4DBE9;opacity:0.04;}"
                          "QPushButton::pressed{border-image:url(:/img/clicked/menu.png);background:#BEBEBE;opacity:0.04;}"
                          );

    nullLabel = new QLabel;
    nullLabel->setPixmap(QPixmap(":/images/TitleBar/line.png"));

    miniBtn = new QPushButton;
    miniBtn->setCursor(Qt::PointingHandCursor);
    miniBtn->setFixedSize(32,32);
    miniBtn->setToolTip("mini模式");
    miniBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                           border-image:url(:/img/default/mini.png);}"
                           "QPushButton::hover{border-image:url(:/img/hover/mini.png);background:#D4DBE9;opacity:0.04;}"
                           "QPushButton::pressed{border-image:url(:/img/clicked/mini.png);background:#BEBEBE;opacity:0.04;}");

    minimumBtn = new QPushButton;
    minimumBtn->setCursor(Qt::PointingHandCursor);
    minimumBtn->setToolTip("最小化");
    minimumBtn->setFixedSize(32,32);
    minimumBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                              border-image:url(:/img/default/minimize.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/minimize.png);background:#D4DBE9;opacity:0.04;}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/minimize.png);background:#BEBEBE;opacity:0.04;}");

    maximumBtn = new QPushButton;
    maximumBtn->setCursor(Qt::PointingHandCursor);
    maximumBtn->setFixedSize(32,32);
    maximumBtn->setObjectName("maximumBtn");
    maximumBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;\
                              border-image:url(:/img/default/maximize.png);}"
                              "QPushButton::hover{border-image:url(:/img/hover/maximize.png);background:#D4DBE9;opacity:0.04;}"
                              "QPushButton::pressed{border-image:url(:/img/clicked/maximize.png);background:#BEBEBE;opacity:0.04;}"
                               );


    closeBtn = new QPushButton;
    closeBtn->setCursor(Qt::PointingHandCursor);
    closeBtn->setFixedSize(32,32);
    closeBtn->setToolTip("关闭");
    closeBtn->setStyleSheet("QPushButton{background:transparent;border-radius:4px;;\
                            border-image:url(:/img/default/close.png);}\
                            QPushButton::hover{border-image:url(:/img/hover/close.png);\
                                               background:#FD9595;opacity:0.04}\
                            QPushButton:pressed{border-image:url(:/img/clicked/close.png);background:#ED6464;}");



    connect(maximumBtn, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(minimumBtn, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(closeBtn, SIGNAL(clicked(bool)), this, SLOT(onClicked()));


    LayoutRight->setSpacing(0);
//    LayoutRight->addWidget(userIconBtn);
//    LayoutRight->addWidget(loginBtn);
//    LayoutRight->addSpacing(20);
    LayoutRight->addWidget(setBtn);
    LayoutRight->addSpacing(15);
    LayoutRight->addWidget(miniBtn);
    LayoutRight->addSpacing(10);
    LayoutRight->addWidget(minimumBtn);
    LayoutRight->addSpacing(10);
    LayoutRight->addWidget(maximumBtn);
    LayoutRight->addSpacing(10);
    LayoutRight->addWidget(closeBtn);

//    titleLayout->addWidget(titleIconLabel);
//    titleLayout->addSpacing(60);
    titleLayout->addLayout(LayoutLeft);
    titleLayout->addStretch();
    titleLayout->addLayout(LayoutRight);


}

