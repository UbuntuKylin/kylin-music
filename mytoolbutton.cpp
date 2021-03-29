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


MyLabel::MyLabel(QWidget *parent, Qt::WindowFlags f)
    : QLabel(parent, f)
{
    this->setMinimumWidth(minSize);
    setTextFormat(Qt::PlainText);
}

MyLabel::MyLabel(const QString &text, QWidget *parent, Qt::WindowFlags f)
    : QLabel(text, parent, f), m_fullText(text)
{
    this->setMinimumWidth(minSize);
    setTextFormat(Qt::PlainText);
}

void MyLabel::setText(const QString &text)
{
    setFullText(text);
}

void MyLabel::setFullText(const QString &text)
{
    m_fullText = text;
    update();
}

void MyLabel::setTextLimitShrink(const QString &text, int width)
{
    this->setMinimumWidth(qMin(this->fontMetrics().width(text), width));
    setFullText(text);
}

void MyLabel::setTextLimitExpand(const QString &text)
{
    int textWidth = this->fontMetrics().width(text);
    this->setMaximumWidth(textWidth);
    setFullText(text);
}

QString MyLabel::fullText() const
{
    return m_fullText;
}

void MyLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    elideText();
}

void MyLabel::elideText()
{
    QFontMetrics fm = this->fontMetrics();
    int dif = fm.width(m_fullText) - this->width();
    if (dif > 0) {
        QString showText = fm.elidedText(m_fullText, Qt::ElideRight, this->width());
        QLabel::setText(showText);
        if (showText != m_fullText) {
            QString str = dealMessage(m_fullText);
            this->setToolTip(str);
        } else {
            this->setToolTip("");
        }
    } else {
        QLabel::setText(m_fullText);
        this->setToolTip("");
    }
}

QString MyLabel::dealMessage(QString msg)
{
    if(msg.size() > fontSize)
    {
        QString str;
        int time = msg.size()/fontSize;
        for(int i = 0; i <= time-1; i++)
        {
            str = QString(str + msg.mid(i*fontSize,fontSize)+"\r\n");
        }
        str = QString(str+msg.mid(time*fontSize,fontSize));
        return str;
    }
    else
        return msg;
}

