#ifndef MYTOOLBUTTON_H
#define MYTOOLBUTTON_H

#include <QToolButton>
#include <QPainter>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QtWidgets>

const QString IS_SELECT = "IS_SELECT";
class MyToolButton : public QToolButton
{
     Q_OBJECT
public:
     MyToolButton();
    ~MyToolButton();
     void selectChanged();
     void defaultStyle();
     void requestCustomContextMenu(const QPoint &pos);
public:
    QAction *deleteAct;
    QAction *renameAct;
signals:
    void playall(QString text);
    void renamePlayList(QString text);
    void removePlayList(QString text);

    void selectButtonChanged(QString text);
public slots:

private:

};


#endif
