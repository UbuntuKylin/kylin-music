#ifndef TableOne_H
#define TableOne_H

#include <QWidget>
#include <QListView>
#include <QLabel>
#include <QToolButton>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlRelationalTableModel>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDebug>
#include <QHeaderView>
#include <QMenu>
#include <QVBoxLayout>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include <QMouseEvent>

#include "base/musicDataBase.h"
#include "scoreinfomodel.h"
#include "player/player.h"
#include "tableviewdelegate.h"
#include "tablebaseview.h"

class TableOne : public QWidget
{
    Q_OBJECT
public:
    explicit TableOne(QString listName,QWidget *parent = nullptr);

    ScoreInfoModel *m_model;
//    QTableView *tableView;
    TableBaseView *tableView;
    void addMusicFromLocal();
    QLabel *listTitleLabel;
    QString nowListName;
    void changeNumber();

    QList<musicDataStruct> getMusicList();
private:
    void initUI();
    void initConnect();
    void initRightMenu();
    void tableViewDoubleClicked();

    void showRightMenu(const QPoint &pos);
    void deleteSongs();
    void playSongs();
    void showInfo();
    void addToOtherList(QAction *listNameAction);
    QMenu *m_menu;  //新建一个Menu属性
    QSqlQueryModel *tableModel;

    QAction * playRow;
    QAction * removeRow;
    QAction * showInfoRow;
    QMenu * addToOtherListMenu;
    QMap<int, QString> getSelectedTaskIdList();

    QStringList pathList;
//    void mouseMoveEvent(QMouseEvent *event);
    TableViewDelegate *delegate;

    QHBoxLayout *listTitleHBoxLayout;
    QToolButton *addMusicButton;
    QLabel *listTotalNumLabel;
    void setHightLight(int index);
    void initStyle();
    void initTableViewStyle();
signals:
    void sendPathToPlayer(QString fp);

    void countChanges();
//    void hoverIndexChanged(QModelIndex index);

public slots:
};

#endif // TableOne_H
