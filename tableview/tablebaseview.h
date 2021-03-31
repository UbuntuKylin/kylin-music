#ifndef TABLEBASEVIEW_H
#define TABLEBASEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
#include <QStandardItemModel>
#include "scoreinfomodel.h"
#include "base/musicDataBase.h"

class TableBaseView : public QTableView
{
    Q_OBJECT
//    void mouseMoveEvent(QMouseEvent *event);
public:
    explicit TableBaseView(QTableView *parent = nullptr);
    ~TableBaseView();
    ScoreInfoModel *m_model;
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void hoverIndexChanged(QModelIndex index);
};

#endif // TABLEBASEVIEW_H
