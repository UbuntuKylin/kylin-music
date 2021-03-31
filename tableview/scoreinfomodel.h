#ifndef SCOREINFOMODEL_H
#define SCOREINFOMODEL_H

#include <QObject>
#include <QStandardItem>
#include <QTableView>
#include <QList>
#include <QDebug>
#include "base/musicDataBase.h"

class ScoreInfoModel : public QObject
{
    Q_OBJECT
public:
    explicit ScoreInfoModel(QObject *parent = 0);
    bool add(musicDataStruct info);
    bool add(QList<musicDataStruct> list);
    bool remove(int i);
    int count();
    void clear();
    void setView(QTableView &view);
    musicDataStruct getItem(int i);
    QStringList getPathList();
    void getMusicDateList(QList<musicDataStruct> list);
    QStandardItemModel m_model;
private:
    QList<musicDataStruct> resList;

};

#endif // SCOREINFOMODEL_H
