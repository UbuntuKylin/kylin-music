#ifndef MINIWIDGET_H
#define MINIWIDGET_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>

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
