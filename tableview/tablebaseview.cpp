#include "tablebaseview.h"

TableBaseView::TableBaseView(QTableView *parent)
{
    this->setMouseTracking(true);
//    this->installEventFilter(this);
//    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    this->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
//    this->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置按ctrl键选中多个
//    this->setContextMenuPolicy(Qt::CustomContextMenu);

//    this->setStyleSheet("QTableView{border:none;}"
//                             "QTableView::item:selected{color:black;background:#F0F0F0;border:none;}");
//    QList<musicDataStruct> resList;
//    int ret = g_db->getSongInfoListFromLocalMusic(resList);
//    qDebug() << resList[0].filepath;
//    m_model = new ScoreInfoModel;
//    m_model->add(resList);
//    qDebug() << "Aaaaaaaaaaaaaaaaaaaaa";
//    this->setModel(m_model->m_model);
//    this->horizontalHeader()->setVisible(false);// 水平不可见
//    this->verticalHeader()->setVisible(false);// 垂直不可见
//    this->setAutoScroll(true);
//    this->verticalScrollBarPolicy();
//    this->setAutoFillBackground(true);
//    this->hideColumn(4);
//    this->hideColumn(5);
//    this->hideColumn(6);
////    this->setModel(tableModel);
//    this->resize(800,1000);
//    this->setAlternatingRowColors(false);
//    this->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
//    this->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
//    this->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
//    this->setAutoFillBackground(true);
}

TableBaseView::~TableBaseView()
{

}
void TableBaseView::mouseMoveEvent(QMouseEvent *event)
{
    QModelIndex index = this->indexAt(event->pos());
    int row = index.row();
    if(row < 0)
    {
        return;
    }
//    qDebug() << event->pos();
//    qDebug() << "QModelIndex index"<< index.row();
    emit hoverIndexChanged(index);
}
