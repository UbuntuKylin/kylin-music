#include "musicslider.h"

MusicSlider::MusicSlider(QWidget *parent):QSlider(parent)
{
    //现在进度条样式已基本符合要求，但是两端稍微有点瑕疵，暂搁置
    //逻辑也稍微有点问题，如果不选择歌曲，进度条应该禁止操作
    //所以将音乐播放进度条独立出来，方便协作和扩展


   initStyle();//初始化样式
}

void MusicSlider::initStyle()
{
    this->setFixedSize(750,12);
    this->installEventFilter(this);
    this->setOrientation(Qt::Horizontal);
    this->setStyleSheet("QSlider::groove:horizontal{padding-left:0px;height: 2px;background:#3790FA;}"
                        "QSlider::sub-page:horizontal{left: 10px;right:10px;background:#3790FA;}"
                        "QSlider::add-page:horizontal{background:#ECEEF5;}"
                        "QSlider::handle:horizontal{}"
                        "QSlider::handle:horizontal:hover {margin-top: -3px;margin-bottom: -3px;}");
}
