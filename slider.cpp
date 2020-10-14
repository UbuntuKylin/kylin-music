#include "slider.h"

Slider::Slider(QWidget *parent)
    :QSlider(parent)
{

}
Slider::~Slider()
{

}
void Slider::wheelEvent(QWheelEvent *e)
{
//    qDebug()<<"wheelEvent";
    QWidget::wheelEvent(e);
}
