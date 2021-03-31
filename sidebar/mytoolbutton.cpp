#include "mytoolbutton.h"
#include "base/widgetstyle.h"
#include <QDebug>

MyToolButton::MyToolButton()
{
    defaultStyle();
    this->setFixedSize(162,32);
    //文字在图标旁边
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->setIconSize(QSize(16,16));
    connect(this,&MyToolButton::clicked,this,&MyToolButton::selectChanged);

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this,&MyToolButton::customContextMenuRequested,this,&MyToolButton::selectChanged);
    connect(this,&MyToolButton::customContextMenuRequested,this,&MyToolButton::requestCustomContextMenu);
}

MyToolButton::~MyToolButton()
{

}

void MyToolButton::selectChanged()
{
    if(this->statusTip() == IS_SELECT)
    {
        return;
    }
    emit selectButtonChanged(this->text());
    qDebug() << this->text() << "歌单名";
    QList<MyToolButton *> list = this->parent()->parent()->parent()->findChildren<MyToolButton *>();
    for(MyToolButton *tmp : list)
    {
        if(tmp->statusTip() == IS_SELECT)
        {
            tmp->setStatusTip("");
            tmp->defaultStyle();
        }
    }
    this->setStatusTip(IS_SELECT);
    this->defaultStyle();
}

void MyToolButton::defaultStyle()
{
    if(this->statusTip() == IS_SELECT)
    {
        this->setIcon(QIcon(":/img/default/songlist_w .png"));
        this->setStyleSheet("QToolButton{background-color:qlineargradient(x1:0, y1:0, x2:1, y2:0,stop:0 #40A9FB,stop:1 #3790FA);padding-left:14px;color:#FFFFFF;border-radius: 4px;}");
        return;
    }
    else if(this->statusTip() != IS_SELECT)
    {
        if(WidgetStyle::themeColor == 0)
        {
            //padding-left:15px;  左内边距
            this->setIcon(QIcon(":/img/default/songlist.png"));
            this->setStyleSheet("QToolButton{padding-left:14px;\
                                background-color:#FAFAFA;color:#303133;border-radius:4px;}"
                                "QToolButton::hover{background-color:#EEEEEE;border-radius:4px;}");
        }

        else if(WidgetStyle::themeColor == 1)
        {
            this->setIcon(QIcon(":/img/default/songlist_w .png"));
            this->setStyleSheet("QToolButton{padding-left:14px;background-color:#1F2022;color:#F9F9F9;border-radius:4px;}"
                                "QToolButton::hover{background-color:#303032;border-radius:4px;}");
        }

    }
}

void MyToolButton::requestCustomContextMenu(const QPoint &pos)
{
    QMenu *menu = new QMenu(this);
    QAction *playAct = new QAction(this);
    QAction *pauseAct = new QAction(this);
    renameAct = new QAction(this);
    deleteAct = new QAction(this);
    playAct->setText(tr("Play"));
    pauseAct->setText(tr("Pause"));
    renameAct->setText(tr("Rename"));
    deleteAct->setText(tr("Delete"));
    menu->addAction(playAct);
//    menu->addAction(pauseAct);

    QString text = this->text();
    QString btnText = text;

//    qDebug()<< btnText <<__FILE__<< "," <<__FUNCTION__<< "," <<__LINE__;
    connect(menu,&QMenu::triggered,this,[ = ](QAction * action)
    {

        if(action->text() == tr("Play"))
        {
            emit playall(text);
        }
        else if(action->text() == tr("Pause"))
        {
//            emit
        }
        else if(action->text() == tr("Rename"))
        {
            emit renamePlayList(text);
        }
        else if(action->text() == tr("Delete"))
        {
            emit removePlayList(text);
        }
    });

    if(btnText != tr("Song List") && btnText != tr("I Love"))
    {
        menu->addAction(renameAct);
        menu->addAction(deleteAct);
    }
    menu->exec(mapToGlobal(pos));
    qDebug() << "btnText" << btnText;
    delete menu;
    delete playAct;
    delete renameAct;
    delete deleteAct;
}

