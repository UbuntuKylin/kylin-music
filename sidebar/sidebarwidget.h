#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <base/allpupwindow.h>

#include "mytoolbutton.h"

class SideBarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SideBarWidget(QWidget *parent = nullptr);
public:
    void sidecolor();
public:
    //新建歌单弹窗界面
    AllPupWindow *newSonglistPup;
    //重命名歌单弹窗界面
    AllPupWindow *renameSongListPup;
public slots:
    //添加歌单弹窗输入框
    void addPlayList();
    //添加歌单
    void addItemToSongList();
    //歌单播放
    void playAll(QString btnText);
    void rename(QString btnText);
    //歌单重命名
    void renamePlayList();
    //删除歌单
    void removePlayList(QString text);
signals:
    void playListBtnCliced(QString listname);
    void playListRemoved(QString listname);
    void playListRenamed(QString oldName,QString newName);
    void playListAdded(QString listName);

private:
    void initWidget();
    void initConnect();
    //获取歌单名称
    void getPlayListName();
    //输入为空添加歌单
    QString newPlayListName();
private:
    //歌曲列表 QToolButton
    MyToolButton *playListBtn;
    //我喜欢 QToolButton
//    MyToolButton *loveBun;
    //添加歌单按钮
    QPushButton *myPlayListBtn;
    //新建歌单按钮界面(包含我喜欢在内)
    QWidget *newPlayListWidget;
    QWidget *mainWid;
    QVBoxLayout *newPlayListLayout;
    //歌单名
    QStringList playListName;
    //kylin-music logo
    QPushButton *logoPushButton;
    //音乐库label
    QLabel *libraryLabel;
    //我的歌单label
    QLabel *myPlayListLabel;
    //获取歌单名
    QString btnText;
};

#endif // SIDEBARWIDGET_H
