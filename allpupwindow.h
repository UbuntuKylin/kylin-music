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

#ifndef ALLPUPWINDOW_H
#define ALLPUPWINDOW_H
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QStackedWidget>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QListWidget>
#include <QListWidgetItem>
#include <QDialog>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QTextEdit>
#include <QLineEdit>
#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QDialog>

#include "changelistwid.h"
#include "songitem.h"



class AllPupWindow :public QWidget
{
    Q_OBJECT
public:
    explicit AllPupWindow(QWidget *parent = nullptr);

    QDialog     *pupDialog;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;
    QToolButton *closeBtn;
    QLabel      *titleLab;
    QLineEdit   *enterLineEdit;
    QVBoxLayout *testLayout;
    QHBoxLayout *titleLayout;
    QHBoxLayout *btnLayout;

    QStackedWidget *stackWid;
    void dlgcolor();



public slots:
    void closeDialog();

private:
    void inpupdialog();

};


class SongInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SongInfoWidget(QWidget *parent = nullptr);

    QDialog     *songInfoDlg;
    QPushButton *songInfoConfirmBtn;
    QPushButton *songInfoCancelBtn;
    QToolButton *songInfoCloseBtn;
    QLabel      *songInfoTitleLab;

//    QLabel *songInfoDlgLabel;

    QLabel *titleLab;
    QLabel *artistLab;
    QLabel *albumLab;
    QLabel *typeLab;
    QLabel *sizeLab;
    QLabel *timeLab;
    QLabel *pathLab;

//    QLabel *musicNameLab;
//    QLabel *singerNameLab;
//    QLabel *albumNameLab;
    QLineEdit   *musicNameEdit;
    QLineEdit   *singerNameEdit;
    QLineEdit   *albumNameEdit;
    QLabel *fileTypeLab;
    QLabel *fileSizeLab;
    QLabel *fileTimeLab;
    QLabel *filePathLab;
    void songInfoDlgColor();

public slots:
    void songInfoCloseDialog();

private:
    void inSongInfoDlg();


};
#endif // ALLPUPWINDOW_H
