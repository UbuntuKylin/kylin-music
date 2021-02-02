#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QToolButton>
#include <QPainter>
#include <QWidget>
#include <QPushButton>
const QString IS_SELECT = "IS_SELECT";
class MyToolButton : public QToolButton
{
     Q_OBJECT
public:
     MyToolButton();
    ~MyToolButton();
     void selectChanged();
     void defaultStyle();
signals:

public slots:


};
#endif
