#ifndef MUSICSLIDER_H
#define MUSICSLIDER_H

#include <QObject>
#include <QSlider>
#include <QDebug>
class MusicSlider : public QSlider
{
    Q_OBJECT
public:
    MusicSlider(QWidget *parent);

private:
    void initStyle();
};

#endif // MUSICSLIDER_H
