#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    int res;
    res = g_db->initDataBase();
    if(res != DB_OP_SUCC)
    {
        qDebug() << "数据库加载失败" <<__FILE__<< ","<<__FUNCTION__<<","<<__LINE__;
    }
    qDebug() << "数据库加载成功";
    initAllComponent();
    allConnect();
    initGSettings();
}

Widget::~Widget()
{
}



void Widget::initAllComponent()
{

    mainVBoxLayout = new QVBoxLayout();

//    musicListTable = new TableBaseView();
    musicListTable = new TableOne(tr("Song List"),this);
    playSongArea = new PlaySongArea;
    QWidget *rightVWidget = new QWidget(this);
    rightVWidget->setLayout(mainVBoxLayout);
    mainVBoxLayout->addWidget(musicListTable);
    mainVBoxLayout->addWidget(playSongArea,0,Qt::AlignBottom);
//    QStringList listNameList;
//    int ret = g_db->getPlayList(listNameList);
//    for(QString name : listNameList)
//    {
//        TableOne *table = new TableOne(name,this);
//        mainVBoxLayout->addWidget(table);
//        table->hide();
//    }
//    mainVBoxLayout->setSpacing(0);
//    mainVBoxLayout->setMargin(0);
    mainHBoxLayout = new QHBoxLayout();
    sideBarWid = new SideBarWidget(this);
    sideBarWid->setFixedWidth(210);
    mainHBoxLayout->addWidget(sideBarWid);
    mainHBoxLayout->addWidget(rightVWidget);
    mainHBoxLayout->setSpacing(0);
    mainHBoxLayout->setMargin(0);

    this->resize(960,640);
    this->setLayout(mainHBoxLayout);
    this->setAutoFillBackground(true);
    this->setBackgroundRole(QPalette::Base);
}

void Widget::allConnect()
{

    connect(sideBarWid,&SideBarWidget::playListBtnCliced,this,&Widget::selectListChanged);
    connect(sideBarWid,&SideBarWidget::playListAdded,this,&Widget::playListAdded);
    connect(sideBarWid,&SideBarWidget::playListRemoved,this,&Widget::playListRemoved);
}


void Widget::showHistroryPlayList()
{
//    historyListTable->show();
}
void Widget::initGSettings()//初始化GSettings
{
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW))
    {
        themeData = new QGSettings(FITTHEMEWINDOW);
        if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
            WidgetStyle::themeColor = 1;
//            changeDarkTheme();
        }
        else
        {
            WidgetStyle::themeColor = 0;
//            changeLightTheme();
        }

        connect(themeData,&QGSettings::changed,this,[=]()
        {
            qDebug() << "主题颜色" << themeData->get("style-name").toString();
            if(themeData->get("style-name").toString() == "ukui-dark" || themeData->get("style-name").toString() == "ukui-black"){
                WidgetStyle::themeColor = 1;
//                changeDarkTheme();
            }
            else
            {
                WidgetStyle::themeColor = 0;
//                changeLight Theme();
            }
        });
    }
    qDebug()<<"初始化GSettings成功";
}
void Widget::selectListChanged(QString listname)
{

    qDebug() << "歌单名" <<listname;
    musicListTable->m_model->clear();
    musicListTable->nowListName = listname;
    musicListTable->getMusicList();
    musicListTable->changeNumber();
    musicListTable->listTitleLabel->setText(listname);
//    QList<TableOne *> list = this->findChildren<TableOne *>();
//    for(TableOne *tmp : list)
//    {
//        tmp->hide();
//        if(tmp->listTitleLabel->text() == listname)
//        {
//            tmp->show();
//        }
//    }

}
void Widget::playListAdded(QString listname)
{
//    TableOne *table = new TableOne(listname,this);
//    mainVBoxLayout->addWidget(table);
//    table->hide();
}

void Widget::playListRemoved(QString listname)
{
//    QList<TableOne *> list = this->findChildren<TableOne *>();
//    for(int i = 0; i < list.size();i++)
//    {
//        list[i]->hide();
//        if(list[i]->listTitleLabel->text() == listname)
//        {
//            if(i = list.size()-1)
//            {
//                list[i-1]->show();
//                list[i]->deleteLater();
//            }
//            else
//            {
//                list[i+1]->show();
//                list[i]->deleteLater();
//            }
//        }
//    }
}
void Widget::playListRenamed(QString oldName,QString newName)
{
//    QList<TableOne *> list = this->findChildren<TableOne *>();
//    for(TableOne *tmp : list)
//    {
//        tmp->hide();
//        if(tmp->listTitleLabel->text() == oldName)
//        {
//            tmp->listTitleLabel->setText(newName);
//        }
//    }
}
