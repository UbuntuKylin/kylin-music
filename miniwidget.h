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

#ifndef MINIWIDGET_H
#define MINIWIDGET_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QAction>
#include "widgetstyle.h"

const QString miniMainColor = "QFrame{background:#FFFFFF;}";

#pragma pack(push)
#pragma pack(1)

class miniWidget : public QFrame
{
    Q_OBJECT
public:
    explicit miniWidget(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);

    void initMyApp();  // liucong add 20201222
    void songText(QString songName); //mini 正在播放

private:
    void initAction();


public slots:

public:
    QFrame *m_mainFrame;

    QLabel *m_coverLabel;
    QPushButton *m_loveBtn;
    QPushButton *m_volBtn;
    QPushButton *m_orderBtn;

    QPushButton *m_closeBtn;
    QPushButton *m_recoveryWinBtn;

    QPushButton *m_preBtn;
    QPushButton *m_playStateBtn;
    QPushButton *m_nextBtn;

    QLabel *m_songNameLab;
    QLabel *m_timeLab;

    void minicolor();


    QMenu *playModeMenu;
    QAction *playMode_Sequential_Action;
    QAction *playMode_Loop_Action;
    QAction *playMode_CurrentItemInLoop_Action;
    QAction *playMode_Random_Action;

private:
    void init_miniWidget();
    bool m_mouseState;

    QPoint  m_WindowPos;
    QPoint m_MousePos;

    QHBoxLayout *m_hMainLayout;
    QHBoxLayout *m_hPlayLayout;
    QHBoxLayout *m_hLayout;

    QFrame *m_palyFrame;
    QWidget *coverWid;

    QHBoxLayout *m_HMainLayout;


    QVBoxLayout *m_vInfoLayout;
    QVBoxLayout *m_vSysLayout;


};

#pragma pack(pop)

#endif // MINIWIDGET_H
