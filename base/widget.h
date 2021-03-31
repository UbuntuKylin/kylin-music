#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>

#include "musicDataBase.h"
#include "tableview/tableone.h"
#include "tableview/scoreinfomodel.h"
#include "tableview/tablebaseview.h"
#include "widgetstyle.h"
#include "sidebar/sidebarwidget.h"
#include "player/playsongarea.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:

private slots:
    void selectListChanged(QString listname);
private:

    QVBoxLayout *mainVBoxLayout;
    QHBoxLayout *mainHBoxLayout;

//    TableBaseView *musicListTable;
    TableOne *musicListTable;
//    TableOne *historyListTable;

    void initAllComponent();
    void allConnect();
    void showHistroryPlayList();

    void initGSettings();
    QGSettings *themeData = nullptr;
    SideBarWidget *sideBarWid;
    void playListAdded(QString listname);
    void playListRemoved(QString listname);
    void playListRenamed(QString oldName, QString newName);

    PlaySongArea *playSongArea;
};
#endif // WIDGET_H
