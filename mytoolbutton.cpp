#include "mytoolbutton.h"
#include "widgetstyle.h"
#include <QDebug>

MyToolButton::MyToolButton()
{
    defaultStyle();
    this->setFixedSize(180,32);
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    this->setIconSize(QSize(16,16));
    connect(this,&MyToolButton::clicked,this,&MyToolButton::selectChanged);
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
        this->setStyleSheet("background-color:qlineargradient(x1:0, y1:0, x2:1, y2:0,stop:0 #40A9FB,stop:1 #3790FA);border-radius:16px;color:#FFFFFF;padding-left:15px;margin-left:10px;font-size: 14px");
        return;
    }
    else if(this->statusTip() != IS_SELECT)
    {
        if(WidgetStyle::themeColor == 0)
        {
            this->setIcon(QIcon(":/img/default/songlist.png"));
            this->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;\
                                font-size: 14px;background-color:#FAFAFA;color:#303133;border-radius:16px;}"
                                "QToolButton::hover{background-color:#EEEEEE;border-radius:16px;}");
        }

        else if(WidgetStyle::themeColor == 1)
        {
            this->setIcon(QIcon(":/img/default/songlist_w .png"));
            this->setStyleSheet("QToolButton{padding-left:15px;margin-left:10px;font-size: 14px;background-color:#1F2022;;color:#F9F9F9;border-radius:16px;}"
                                "QToolButton::hover{background-color:#303032;border-radius:16px;}");
        }

    }
}

