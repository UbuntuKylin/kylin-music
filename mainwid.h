#ifndef MAINWID_H
#define MAINWID_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSystemTrayIcon>
#include <QTableWidgetItem>
#include <QPropertyAnimation>
#include <QSettings>
#include <QDialog>
#include <QButtonGroup>
#include <QRadioButton>
#include <QCheckBox>
#include <QMediaPlaylist>
#include <QMediaPlayer>
#include <QSqlTableModel>
#include <QTime>
#include <QMessageBox>
#include <QMediaMetaData>
#include <QContextMenuEvent>

#include "titlebar.h"
#include "sidebar.h"
#include "changelistwid.h"
#include "playsongarea.h"
#include "slider.h"

class MainWid : public QMainWindow
{
    Q_OBJECT

public:
    MainWid(QWidget *parent = nullptr);
    ~MainWid();
    QString getMp3FileName(QString sqlName);
    void updatalistwidget(int value);//更新listWidget

    void updateSongPlaying();


public slots:
//    void playSong(bool);
    void play_Song();   //播放和暂停
    void contextMenuEvent(QContextMenuEvent *event);  //歌曲列表右键菜单
//    void contextMenuEvent(QContextMenuEvent *);
    void Action1_slot();    //右键播放
    void Action2_slot();    //右键下一首
    void Action3_slot();    //右键添加到我喜欢
    void Action4_slot();   //从歌单删除
    void Action7_slot();   //歌曲信息
    void on_listWidget_doubleClicked(QListWidgetItem *item);//双击播放playlist
    void on_listWidget1_doubleClicked(QListWidgetItem *item);//双击播放playlist1
    void Music_stateChang(QMediaPlayer::State state);//播放状态改变
    void on_lastBtn_clicked();             //上一首
    void on_nextBtn_clicked();             //上一首
    void positionChange(qint64 position);

    // 拖动进度条
    void slidePress();
    void slideMove(int position);
    void slideRelease();

    void PlayModeChanged();    //播放模式
private:

    QString play_pause = "播放";
    QWidget *mainWidget;

    TitleBar *myTitleBar;
    SideBar *mySideBar;
//    ChangeListWid *myChangeListWid;
    PlaySongArea *myPlaySongArea;

    QSqlTableModel *model;
    QSqlTableModel *model_1;

    QMenu *Menu;
    QAction *Action1;
    QAction *Action2;
    QAction *Action3;
    QAction *Action4;
//    QAction *Action5;
//    QAction *Action6;
    QAction *Action7;

//    QSlider *hSlider;
    Slider *hSlider;
    int moved = 0;


};
#endif // MAINWID_H
