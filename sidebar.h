#ifndef SIDEBAR_H
#define SIDEBAR_H

#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialog>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <QPainter>


#include "changelistwid.h"

class SideBar :public QFrame
{
    Q_OBJECT
public:
    explicit SideBar(QWidget *parent = nullptr);

    QLabel *recommendLabel;
    QLabel *logoLabel;
    QToolButton *PlayListBtn;
    QLabel *MySongListLabel;
    QPushButton *addSongListBtn;
    QToolButton *favoriteMusicBtn;
    QToolButton *songListBtn[10];

    QDialog *addSongListDialog;

    QListWidget *songListWidget;
    QListWidgetItem *myLoveItem;
    QListWidgetItem *newSongList[10];
    QToolButton *newSongListBtn[10];
    QStackedWidget *rightChangeWid;  //进行界面切换      右部除标题栏和播放区域之外的部分

    MusicListWid *musicListChangeWid[11];  //我喜欢，以及预留出的新建歌单对应的切换界面
    MusicListWid *myMusicListWid;   //歌曲列表按钮对应的切换界面

    // 当前播放列表索引
    int currentPlayList = 0;
    // 当前选中列表索引
    int currentSelectList = 0;

public slots:

    void addSongList();
    void dialogClosed();
    void createSongList();

    void ChangePage();
    void AlterPage();
    void AlterthisPage();


private:
    void initTopWidget();

};


#endif // SIDEBAR_H
