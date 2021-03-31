#ifndef SLIDERWIDGET_H
#define SLIDERWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QSlider>
#include <QEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QFocusEvent>

class SliderWidget : public QDialog
{
    Q_OBJECT
public:
    explicit SliderWidget(QWidget *parent = nullptr);
//    QDialog *vSliderDialog;
    QHBoxLayout *HLayout;
    QSlider *vSlider;
public:
    void changeVolumePos(int posX, int posY, int width, int height);
    void volSliderColor();
protected:
    bool eventFilter(QObject *obj, QEvent *event)override;   //鼠标滑块点击  事件过滤器
    bool nativeEvent(const QByteArray &eventType, void *message, long *result)override;
private:
    void initUi();

private:
    int volunmPosX;
    int volunmPosY;
    int volunmPosWidth;
    int volunmPosHeight;
};

#endif // SLIDERWIDGET_H
