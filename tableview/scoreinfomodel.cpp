#include "scoreinfomodel.h"
#include <QStandardItem>
#include <QVariant>

ScoreInfoModel::ScoreInfoModel(QObject* parent) : QObject(parent)
{
    QStringList list;
    list.append("Title");
    list.append("Singer");
    list.append("Album");
    list.append("Time");
    list.append("Path");
    list.append("Type");
    list.append("Size");
    m_model.setHorizontalHeaderLabels(list); //使用list设置水平标题标签。如有必要，将列数增加到标签的大小
}

bool ScoreInfoModel::add(musicDataStruct info)
{
    QStandardItem* root = m_model.invisibleRootItem();
    QStandardItem* item0 = new QStandardItem();
    QStandardItem* item1 = new QStandardItem();
    QStandardItem* item2 = new QStandardItem();
    QStandardItem* item3 = new QStandardItem();
    QStandardItem* item4 = new QStandardItem();
    QStandardItem* item5 = new QStandardItem();
    QStandardItem* item6 = new QStandardItem();

    bool ret = true;
    if( (root != NULL) && (item0 != NULL) && (item1 != NULL) && (item2 != NULL) )
    {
//        item0->setData(info.title, Qt::DisplayPropertyRole);
//        item1->setData(info.singer, Qt::DisplayPropertyRole);
//        item2->setData(info.album, Qt::DisplayPropertyRole);
//        item3->setData(info.time, Qt::DisplayPropertyRole);

        item0->setData(info.title, Qt::DisplayRole);
        item1->setData(info.singer, Qt::DisplayRole);
        item2->setData(info.album, Qt::DisplayRole);
        item3->setData(info.time, Qt::DisplayRole);
        item3->setTextAlignment(Qt::AlignRight);
        item4->setData(info.filepath, Qt::DisplayRole);
        item5->setData(info.filetype, Qt::DisplayRole);
        item6->setData(info.size, Qt::DisplayRole);

        item0->setEditable(false);
        item1->setEditable(false);
        item2->setEditable(false);
        item3->setEditable(false);
        item4->setEditable(false);
        item5->setEditable(false);
        item6->setEditable(false);

        int newRow = count();

        root->setChild(newRow, 0, item0);
        root->setChild(newRow, 1, item1);
        root->setChild(newRow, 2, item2);
        root->setChild(newRow, 3, item3);
        root->setChild(newRow, 4, item4);
        root->setChild(newRow, 5, item5);
        root->setChild(newRow, 6, item6);

        resList.append(info);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool ScoreInfoModel::add(QList<musicDataStruct> list)
{
    bool ret = true;

    for(int i=0; i<list.count(); i++)
    {
        ret = ret && add(list[i]);
    }

    return ret;
}

bool ScoreInfoModel::remove(int i)
{
    bool ret = true;

    if( (0 <= i) && (i < count()) )
    {
        m_model.removeRow(i);
    }
    else
    {
        ret = false;
    }

    return ret;
}

void ScoreInfoModel::clear()
{
    m_model.clear();
}

musicDataStruct ScoreInfoModel::getItem(int i)
{
    musicDataStruct ret;

    if( (0 <= i) && (i < count()) )
    {
        QModelIndex index0 = m_model.index(i, 0, QModelIndex());
        QModelIndex index1 = m_model.index(i, 1, QModelIndex());
        QModelIndex index2 = m_model.index(i, 2, QModelIndex());
        QModelIndex index3 = m_model.index(i, 3, QModelIndex());
        QModelIndex index4 = m_model.index(i, 4, QModelIndex());
        QModelIndex index5 = m_model.index(i, 5, QModelIndex());
        QModelIndex index6 = m_model.index(i, 6, QModelIndex());

        QVariant v0 = index0.data(Qt::DisplayRole);
        QVariant v1 = index1.data(Qt::DisplayRole);
        QVariant v2 = index2.data(Qt::DisplayRole);
        QVariant v3 = index3.data(Qt::DisplayRole);
        QVariant v4 = index4.data(Qt::DisplayRole);
        QVariant v5 = index5.data(Qt::DisplayRole);
        QVariant v6 = index6.data(Qt::DisplayRole);


        ret.title = v0.toString();
        ret.singer = v1.toString();
        ret.album = v2.toString();
        ret.time = v3.toString();
        ret.filepath = v4.toString();
        ret.filetype = v5.toString();
        ret.size = v6.toString();
    }
    return ret;
}

QStringList ScoreInfoModel::getPathList()
{
    qDebug() << "resList.size():" << resList.size();
    QStringList pathList;
    for(int i = 0; i < resList.size(); i++)
    {
        pathList.append(resList[i].filepath);
    }
    return pathList;
}

void ScoreInfoModel::getMusicDateList(QList<musicDataStruct> list)
{
    list = resList;
}

int ScoreInfoModel::count()
{
    return m_model.rowCount();
}

void ScoreInfoModel::setView(QTableView& view)
{
    view.setModel(&m_model);
}
