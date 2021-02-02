#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QSlider>
#include <QEvent>
#include <QMouseEvent>

#include "slider.h"

class SliderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SliderWidget(QWidget *parent = nullptr);
//    QDialog *vSliderDialog;
    QHBoxLayout *HLayout;
    QSlider *vSlider;

private:
    void initUi();
protected:
    bool eventFilter(QObject *obj, QEvent *event);   //鼠标滑块点击  事件过滤器
};

#endif // SLIDERWIDGET_H
