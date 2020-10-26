#ifndef SONGITEM_H
#define SONGITEM_H


#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QToolButton>
#include <QMediaPlayer>
#include <QMediaPlaylist>

#include <songitem.h>

class SongItem : public QWidget
{
    Q_OBJECT

public:

    explicit SongItem(QWidget *parent = nullptr);
    void initItem();
    QWidget *songNameWid;
    QLabel *singerLabel;
    QLabel *albumLabel;
    QLabel *songTimeLabel;
    QLabel *songNameLabel;
    QPushButton *hoverLike;
    QPushButton *hoverPlay;



public slots:


private:



};

#endif // SONGITEM_H
