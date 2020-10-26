#ifndef SLIDER_H
#define SLIDER_H

#include <QSlider>
#include <QObject>
#include <QWheelEvent>
#include <QDebug>
class Slider : public QSlider
{
    Q_OBJECT
public:
    explicit Slider(QWidget *parent = 0);
    ~Slider();

    void wheelEvent(QWheelEvent *e);
};

#endif // SLIDER_H
