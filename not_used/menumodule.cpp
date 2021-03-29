#include "menumodule.h"
#include "xatom-helper.h"
menuModule::menuModule(QWidget *parent = nullptr) : QWidget(parent)
{
    init();
}

void menuModule::init(){
    initAction();
    setStyle();
}

void menuModule::initAction(){
    aboutWindow = new QWidget();
    titleText = new QLabel();
    bodyAppName = new QLabel();
    bodyAppVersion = new QLabel();
    bodySupport = new QLabel();
    menuButton = new QToolButton;
    menuButton->setProperty("isWindowButton", 0x1);
    menuButton->setProperty("useIconHighlightEffect", 0x2);
    menuButton->setPopupMode(QToolButton::InstantPopup);
    menuButton->setFixedSize(30,30);
    menuButton->setIconSize(QSize(16, 16));
    menuButton->setAutoRaise(true);
    menuButton->setIcon(QIcon::fromTheme("open-menu-symbolic"));
    m_menu = new QMenu();
    QList<QAction *> actions ;
    QAction *actionTheme = new QAction(m_menu);
    actionTheme->setText(tr("Theme"));
    QAction *actionHelp = new QAction(m_menu);
    actionHelp->setText(tr("Help"));
    QAction *actionAbout = new QAction(m_menu);
    actionAbout->setText(tr("About"));
    QAction *actionQuit = new QAction(m_menu);
//    actionQuit->setText(tr("Quit"));
//    actions<<actionTheme<<actionHelp<<actionAbout<<actionQuit;
//    actions<<actionTheme<<actionHelp<<actionAbout;    //隐藏关于主题颜色的设置，默认跟随主题，不随着用户操作更改。
    actions<<actionHelp<<actionAbout;
    m_menu->addActions(actions);
//    互斥按钮组
    QMenu *themeMenu = new QMenu;
    QActionGroup *themeMenuGroup = new QActionGroup(this);
    QAction *autoTheme = new QAction(tr("Auto"),this);
    autoTheme->setObjectName("Auto");//用TEXT判断有风险
    themeMenuGroup->addAction(autoTheme);
    themeMenu->addAction(autoTheme);
    autoTheme->setCheckable(true);
    QAction *lightTheme = new QAction(tr("Light"),this);
    lightTheme->setObjectName("Light");//用TEXT判断有风险
//    themeMenuGroup->addAction(lightTheme);
//    themeMenu->addAction(lightTheme);
    lightTheme->setCheckable(true);
    QAction *darkTheme = new QAction(tr("Dark"),this);
    darkTheme->setObjectName("Dark");//用TEXT判断有风险
//    themeMenuGroup->addAction(darkTheme);
//    themeMenu->addAction(darkTheme);
    darkTheme->setCheckable(true);
    QList<QAction* > themeActions;
    themeActions<<autoTheme<<lightTheme<<darkTheme;
//    autoTheme->setChecked(true);
    actionTheme->setMenu(themeMenu);
    menuButton->setMenu(m_menu);
    connect(m_menu,&QMenu::triggered,this,&menuModule::triggerMenu);
    initGsetting();
    setThemeFromLocalThemeSetting(themeActions);
    themeUpdate();
    connect(themeMenu,&QMenu::triggered,this,&menuModule::triggerThemeMenu);
}

void menuModule::setThemeFromLocalThemeSetting(QList<QAction* > themeActions)
{
#if DEBUG_MENUMODULE
    confPath = "org.kylin-music-data.settings";
#endif
    m_pGsettingThemeStatus = new QGSettings(confPath.toLocal8Bit());
    if(!m_pGsettingThemeStatus){
    	qDebug()<<"系统对应的gsetting字段未正确配置";
    }
    QString appConf = m_pGsettingThemeStatus->get("thememode").toString();
    if("lightonly" == appConf){
        themeStatus = themeLightOnly;
        themeActions[1]->setChecked(true);   //程序gsetting中为浅色only的时候就给浅色按钮设置checked
    }else if("darkonly" == appConf){
        themeStatus = themeBlackOnly;
        themeActions[2]->setChecked(true);
    }else{
        themeStatus = themeAuto;
        themeActions[0]->setChecked(true);
    }
}

void menuModule::themeUpdate(){
//    if(themeStatus == themeLightOnly)
//    {
//        setThemeLight();
//    }else if(themeStatus == themeBlackOnly){
//        setThemeDark();
//    }else{
//        setStyleByThemeGsetting();
//    }
      setStyleByThemeGsetting();   //应用颜色更新只跟随主题

}

void menuModule::setStyleByThemeGsetting(){
    QString nowThemeStyle = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == nowThemeStyle || "ukui-black" == nowThemeStyle)
    {
        setThemeDark();
    }else{
        setThemeLight();
    }
}

void menuModule::triggerMenu(QAction *act){


    QString str = act->text();
    if(tr("Quit") == str){
        emit menuModuleClose();
    }else if(tr("About") == str){
        aboutAction();
    }else if(tr("Help") == str){
        helpAction();
    }
}

void menuModule::triggerThemeMenu(QAction *act){
    if(!m_pGsettingThemeStatus)
    {
        m_pGsettingThemeStatus = new QGSettings(confPath.toLocal8Bit());  //m_pGsettingThemeStatus指针重复使用避免占用栈空间
    }
    QString str = act->objectName();
    if("Light" == str){
        themeStatus = themeLightOnly;
        disconnect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
        m_pGsettingThemeStatus->set("thememode","lightonly");
//        disconnect()
        setThemeLight();
    }else if("Dark" == str){
        themeStatus = themeBlackOnly;
        disconnect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
        m_pGsettingThemeStatus->set("thememode","darkonly");
        setThemeDark();
    }else{
        themeStatus = themeAuto;
        m_pGsettingThemeStatus->set("thememode","auto");
        initGsetting();
//        updateTheme();
        themeUpdate();
    }
}

void menuModule::aboutAction(){
//    关于点击事件处理
    initAbout();
}

void menuModule::helpAction(){
//    帮助点击事件处理
#if DEBUG_MENUMODULE
    appName = "tools/kylin-music";
#endif
    DaemonIpcDbus *ipcDbus = new DaemonIpcDbus();
    if(!ipcDbus->daemonIsNotRunning()){
        ipcDbus->showGuide(appName);
    }
}

void menuModule::initAbout(){
    aboutWindow->setWindowFlag(Qt::Tool);
    aboutWindow->setWindowTitle(tr("Kylin music"));
    MotifWmHints hints;
    hints.flags = MWM_HINTS_FUNCTIONS|MWM_HINTS_DECORATIONS;
    hints.functions = MWM_FUNC_ALL;
    hints.decorations = MWM_DECOR_BORDER;
    XAtomHelper::getInstance()->setWindowMotifHint(aboutWindow->winId(), hints);
    aboutWindow->setWindowModality(Qt::ApplicationModal); //弹出自定义对话框时主界面不可操作
    aboutWindow->setFixedSize(420,324);
    aboutWindow->setMinimumHeight(324);
    QRect availableGeometry = qApp->primaryScreen()->availableGeometry();
    aboutWindow->move((availableGeometry.width()-aboutWindow->width())/2,(availableGeometry.height()- aboutWindow->height())/2);
    if(mainlyt){
        aboutWindow->show();
        return ;
    }
    mainlyt = new QVBoxLayout();
    QHBoxLayout *titleLyt = initTitleBar();
    QVBoxLayout *bodylyt = initBody();
    mainlyt->setMargin(0);
    mainlyt->addLayout(titleLyt);
    mainlyt->addLayout(bodylyt);
    mainlyt->addStretch();
    aboutWindow->setLayout(mainlyt);
    //TODO:在屏幕中央显示

    aboutWindow->show();
}

QHBoxLayout* menuModule::initTitleBar(){
    QLabel* titleIcon = new QLabel();
    QPushButton *titleBtnClose = new QPushButton;
    titleIcon->setFixedSize(QSize(24,24));
#if DEBUG_MENUMODULE
    iconPath = ":/img/kylin-music.png";
    appShowingName = "kylin music";
#endif
    //TODO：直接从主题调图标，不会QIcon转qpixmap所以暂时从本地拿
    titleIcon->setPixmap(QPixmap::fromImage(QImage(iconPath)));

    titleIcon->setScaledContents(true);
    titleBtnClose->setFixedSize(30,30);
    titleBtnClose->setIcon(QIcon::fromTheme("window-close-symbolic"));
    titleBtnClose->setProperty("isWindowButton",0x2);
    titleBtnClose->setProperty("useIconHighlightEffect",0x8);
    titleBtnClose->setFlat(true);
    connect(titleBtnClose,&QPushButton::clicked,[=](){aboutWindow->close();});
    QHBoxLayout *hlyt = new QHBoxLayout;
    titleText->setText(tr("kylin music"));
    titleText->setStyleSheet("font-size:14px;");
    hlyt->setSpacing(0);
    hlyt->setMargin(4);
    hlyt->addSpacing(4);
    hlyt->addWidget(titleIcon,0,Qt::AlignBottom); //居下显示
    hlyt->addSpacing(8);
    hlyt->addWidget(titleText,0,Qt::AlignBottom);
    hlyt->addStretch();
    hlyt->addWidget(titleBtnClose,0,Qt::AlignBottom);
    return hlyt;
}

QVBoxLayout* menuModule::initBody(){
    QLabel* bodyIcon = new QLabel();
    bodyIcon->setFixedSize(96,96);
    bodyIcon->setPixmap(QPixmap::fromImage(QImage(iconPath)));
    bodyIcon->setStyleSheet("font-size:14px;");
    bodyIcon->setScaledContents(true);
    bodyAppName->setFixedHeight(28);
//    bodyAppName->setText(tr(appShowingName.toLocal8Bit()));
    bodyAppName->setText(tr("kylin music"));
    bodyAppName->setStyleSheet("font-size:18px;");
    QLabel* bodyAppVersion = new QLabel();
    bodyAppVersion->setFixedHeight(24);
    bodyAppVersion->setText(tr("Version: ") + appVersion);
    bodyAppVersion->setAlignment(Qt::AlignLeft);
    bodyAppVersion->setStyleSheet("font-size:14px;");
    connect(bodySupport,&QLabel::linkActivated,this,[=](const QString url){
        QDesktopServices::openUrl(QUrl(url));
    });
    bodySupport->setContextMenuPolicy(Qt::NoContextMenu);
    bodySupport->setFixedHeight(24);
    bodySupport->setStyleSheet("font-size:14px;");
    QVBoxLayout *vlyt = new QVBoxLayout;
    vlyt->setMargin(0);
    vlyt->setSpacing(0);
    vlyt->addSpacing(20);
    vlyt->addWidget(bodyIcon,0,Qt::AlignHCenter);
    vlyt->addSpacing(16);
    vlyt->addWidget(bodyAppName,0,Qt::AlignHCenter);
    vlyt->addSpacing(12);
    vlyt->addWidget(bodyAppVersion,0,Qt::AlignHCenter);
    vlyt->addSpacing(12);
    vlyt->addWidget(bodySupport,0,Qt::AlignHCenter);
    vlyt->addStretch();
    return vlyt;
}

void menuModule::setStyle(){
//    menuButton->setStyleSheet("QPushButton::menu-indicator{image:None;}");
}

void menuModule::initGsetting(){
    if(QGSettings::isSchemaInstalled(FITTHEMEWINDOW)){
        m_pGsettingThemeData = new QGSettings(FITTHEMEWINDOW);
        connect(m_pGsettingThemeData,&QGSettings::changed,this,&menuModule::dealSystemGsettingChange);
    }

}

void menuModule::dealSystemGsettingChange(const QString key){
    if(key == "styleName"){
        refreshThemeBySystemConf();
    }
}

void menuModule::refreshThemeBySystemConf(){
    QString themeNow = m_pGsettingThemeData->get("styleName").toString();
    if("ukui-dark" == themeNow || "ukui-black" == themeNow){
        setThemeDark();
    }else{
        setThemeLight();
    }
}

void menuModule::setThemeDark(){
    aboutWindow->setStyleSheet("background-color:rgba(31,32,34，1);");
    emit menuModuleSetThemeStyle("dark-theme");
    bodySupport->setText(tr("Service & Support: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(225,225,225,1)\">"
                         "support@kylinos.cn</a>");
}

void menuModule::setThemeLight(){
    aboutWindow->setStyleSheet("background-color:rgba(255，255，255，1);");
    emit menuModuleSetThemeStyle("light-theme");
    bodySupport->setText(tr("Service & Support: ") +
                         "<a href=\"mailto://support@kylinos.cn\""
                         "style=\"color:rgba(0,0,0,1)\">"
                         "support@kylinos.cn</a>");

}
