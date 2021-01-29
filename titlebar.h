/*
 * Copyright (C) 2020, KylinSoft Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QSqlTableModel>
#include <QListWidget>
#include <QSqlQuery>
#include <QDebug>
#include <QListWidgetItem>

#include "changelistwid.h"
#include "allpupwindow.h"
#include "widgetstyle.h"
#include "menumodule.h"

const QString main_style = "QFrame{background:#FFFFFF;border-top-right-radius:12px;}";

#pragma pack(push)
#pragma pack(1)

class TitleBar : public QFrame
{
    Q_OBJECT
public:
    explicit TitleBar(QWidget *parent = nullptr);
    ~TitleBar(){}

    menuModule *menumodule;

    QLabel *nullLabel;

    QPushButton *leftBtn;
    QPushButton *rightBtn;
    QPushButton *searchBtn;

    QLineEdit *searchEdit;

    QPushButton *userIconBtn;
    QPushButton *loginBtn;

    QPushButton *setBtn;
    QPushButton *miniBtn;
    QPushButton *minimumBtn;
    QPushButton *maximumBtn;
    QPushButton *closeBtn;

    //搜索相关
    QSqlTableModel *searchModel;
    QListWidget *searchWidget;
    MusicListWid *searchResultWidget;

    //设置菜单
    QMenu *settingMenu;
    QMenu *changeThemeColorMenu;
    QAction *darkThemeAct;
    QAction *lightThemeAct;
    QAction *aboutAct;

//    AllPupWindow *aboutWidget;

    void titlecolor();

public slots:
    void searchMusic();

//    void showAboutWidget();

protected:
//    virtual void mouseDoubleClickEvent(QMouseEvent *event);

private slots:
    //进行最小化、最大化、还原、关闭
//    void slot_showMaximized();
//    void slot_quit();
private:


private:
    void initTitle();

    QHBoxLayout *titleLayout;
    QHBoxLayout *searchLayout;
    QHBoxLayout *LayoutLeft;
    QHBoxLayout *LayoutRight;
};



#pragma pack(pop)


#endif // TITLEBAR_H
