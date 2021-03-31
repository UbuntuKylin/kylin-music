#ifndef PLAYSONGAREA_H
#define PLAYSONGAREA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QVariant>
#include <QShortcut>

#include "sliderwidget.h"
#include "playbackmodewidget.h"
#include "musicslider.h"

//enum PlayBackMode{
//    modeNull = -1,
//    Loop,
//    Random,
//    Sequential,
//    CurrentItemInLoop,
//};

class PlaySongArea : public QWidget
{
    Q_OBJECT
public:
    explicit PlaySongArea(QWidget *parent = nullptr);
public:
     void playcolor();
public:
      SliderWidget *m_volSliderWid;
      PlayBackModeWidget *m_playBackModeWid;
signals:

public slots:
    //显示音量界面
    void slotVolSliderWidget();
//    void increaseVolume();   //音量调高
//    void reduceVolume();     //音量调低
    //改变音量
    void slotVolumeChanged(int values);
    //我喜欢按钮
    void slotFav();
    //改变播放模式
    void slotPlayBackModeChanged();
    void slotLoopClicked();
    void slotRandomClicked();
    void slotSequentialClicked();
    void slotCurrentItemInLoopClicked();
protected:
    void resizeEvent(QResizeEvent *event)override;
private:
    void initWidget();
    void initConnect();
    //移动音量
    void moveVolSliderWid();
    //计算新的播放模式
    void movePlayModeWid();
private:

    QHBoxLayout *m_mainLayout;
    //上一首
    QPushButton *preBtn;
    //播放/暂停
    QPushButton *playBtn;
    //下一首
    QPushButton *nextBtn;
    //音量
    QPushButton *volumeBtn;
    //我喜欢
    QPushButton *favBtn;
    //播放模式
    QPushButton *playModeBtn;
    //歌词
    QPushButton *lyricBtn;
    //播放列表
    QPushButton *listBtn;

    QLabel *coverPhotoLabel;
    QLabel *playingLabel;
    QLabel *timeLabel;

};

#endif // PLAYSONGAREA_H
