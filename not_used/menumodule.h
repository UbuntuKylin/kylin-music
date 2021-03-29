#ifndef MENUMODULE_H
#define MENUMODULE_H
#define DEBUG_MENUMODULE 0  //DEBUG模式开关，代码上线之前务必关掉
#define FITTHEMEWINDOW "org.ukui.style"


#include <QObject>
#include <QWidget>
#include <QMenu>
#include <QPushButton>
#include <QToolButton>
#include <QDebug>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRect>
#include <QScreen>
#include <QGSettings>
#include <QMetaEnum>
#include <QDesktopServices>
#include <QUrl>
#include "daemonipcdbus.h"
#include "widgetstyle.h"
class menuModule : public QWidget
{
    Q_OBJECT
public:
    explicit menuModule(QWidget *);
    void themeUpdate();

    enum typeThemeStatus {
        themeAuto = 0,
        themeBlackOnly = 1,
        themeLightOnly = 2
    } themeStatus;

signals:
    void menuModuleClose();
    void menuModuleSetThemeStyle(QString);
public:
    QToolButton *menuButton = nullptr;

public:
//    程序在实例化的时候需要传递的信息字段,打开debug开关后这些字段会被自动填充
    QString appName = "kylin-music"; //格式kylin-usb-creator
//    QString appShowingName = tr("kylin music"); //格式kylin usb creator ,用来在前端展示
    QString appVersion = "1.0.0";
    QString appDesc = "2020.01.08";
    QString iconPath = ":/img/kylin-music.png";
    QString confPath = "org.kylin-music-data.settings";

private:
    QMenu *m_menu = nullptr;
    QMenu *themeMenu = nullptr;
    QSize iconSize;
    QString appPath = "tools/kylin-music"; //拉起帮助菜单时使用appName字段
    QWidget *aboutWindow = nullptr;
    QGSettings *m_pGsettingThemeData = nullptr;
    QGSettings *m_pGsettingThemeStatus = nullptr;
public slots:
    void dealSystemGsettingChange(const QString);
private:
    void init();
    QHBoxLayout* initTitleBar(); //关于窗口标题栏初始化
    QVBoxLayout* initBody();    // 关于窗口body初始化
    void initGsetting();
    void initAction();
    void setStyle();
    void triggerMenu(QAction* act); //主菜单动作4
    void triggerThemeMenu(QAction* act); //主题动作
    void aboutAction();
    void initAbout(); //关于窗口初始化
    void helpAction();
    void setThemeFromLocalThemeSetting(QList<QAction* >); //获取本地主题配置
    void setStyleByThemeGsetting(); //通过外部主题配置设置主题
    void setThemeStyle();
    void setThemeLight();
    void setThemeDark();
    QLabel* titleText = nullptr;
    QLabel* bodyAppName = nullptr;
    QLabel* bodyAppVersion = nullptr;
    QLabel* bodySupport = nullptr;
//    void updateTheme(); //点击菜单中的主题设置后更新一次主题
    QVBoxLayout *mainlyt = nullptr;

    void refreshThemeBySystemConf();    //通过系统配置更改主题
};

#endif // MENUMODULE_H
