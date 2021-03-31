#include "tableone.h"

TableOne::TableOne(QString listName, QWidget *parent) : QWidget(parent)
{
    nowListName = listName;
    delegate = new TableViewDelegate;
    initUI();
    initRightMenu();
    initConnect();
    initStyle();

}

void TableOne::initStyle()
{

    listTitleLabel->setStyleSheet("width:96px;height:24px;"
                                 "font-size:24px;\
                                 font-weight: 600;\
                                 line-height: 24px;");

    listTotalNumLabel->setStyleSheet("font-weight: 400;\
                                        border:none;\
                                        line-height: 14px;");

}

void TableOne::initTableViewStyle()
{
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    tableView->setColumnWidth(3,50);

    tableView->setStyleSheet("QTableView{border:none;}"
                             "QTableView::item:selected{color:black;background:#F0F0F0;border:none;}");
    tableView->horizontalHeader()->setVisible(false);// 水平不可见
    tableView->verticalHeader()->setVisible(false);// 垂直不可见
    tableView->setAutoScroll(true);
    tableView->verticalScrollBarPolicy();
    tableView->hideColumn(4);
    tableView->hideColumn(5);
    tableView->hideColumn(6);
    //    tableView->setModel(tableModel);
    tableView->resize(800,1000);
    tableView->show();
    tableView->setAlternatingRowColors(false);
    tableView->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tableView->setAutoFillBackground(true);
}

void TableOne::initUI()
{
    QVBoxLayout *tableLayout = new QVBoxLayout();
    this->setLayout(tableLayout);

    listTitleLabel = new QLabel(this);
    listTotalNumLabel = new QLabel(this);
    addMusicButton = new QToolButton(this);
    listTitleHBoxLayout = new QHBoxLayout();

    QWidget *titleWid = new QWidget(this);
    titleWid->setLayout(listTitleHBoxLayout);
    listTitleHBoxLayout->setAlignment(Qt::AlignLeft);
    listTitleHBoxLayout->addWidget(listTitleLabel);
    listTitleHBoxLayout->addWidget(listTotalNumLabel);
    listTitleHBoxLayout->addStretch(0);
    listTitleHBoxLayout->addWidget(addMusicButton,1,Qt::AlignRight);
    listTitleHBoxLayout->setMargin(0);
    listTitleHBoxLayout->setSpacing(0);


    addMusicButton->setText(tr("添加歌曲"));
    addMusicButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    addMusicButton->setIconSize(QSize(16,16));
//    addMusicButton->setText(tr("   Add"));
    addMusicButton->setFixedSize(100,30);

    QHBoxLayout *tableTitleLayout = new QHBoxLayout();
    QWidget *tableTitleWidget = new QWidget(this);
    tableTitleWidget->setLayout(tableTitleLayout);
    QLabel *songNameTitle = new QLabel(tr("歌曲"));
    QLabel *singerTitle = new QLabel(tr("歌手"));
    QLabel *albumTitle = new QLabel(tr("专辑"));
    QLabel *songtimeTitle = new QLabel(tr("时长"));
    songtimeTitle->setAlignment(Qt::AlignRight);
    songtimeTitle->setFixedWidth(60);
    tableTitleLayout->setAlignment(Qt::AlignLeft);
    tableTitleLayout->addWidget(songNameTitle,1);
    tableTitleLayout->addWidget(singerTitle,1);
    tableTitleLayout->addWidget(albumTitle,1);
    tableTitleLayout->addStretch(0);
    tableTitleLayout->addWidget(songtimeTitle,Qt::AlignRight);
    tableTitleLayout->setSpacing(0);
    tableTitleLayout->setContentsMargins(3.5,0,2.5,0);
    songNameTitle->setStyleSheet("color:#8F9399;");
    singerTitle->setStyleSheet("color:#8F9399;");
    albumTitle->setStyleSheet("color:#8F9399;");
    songtimeTitle->setStyleSheet("color:#8F9399;");

    tableView = new TableBaseView();
    tableLayout->addWidget(titleWid);
    tableLayout->addSpacing(10);
    tableLayout->addWidget(tableTitleWidget);
    tableLayout->addSpacing(10);
    tableLayout->addWidget(tableView);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行
    tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);//设置按ctrl键选中多个
    QList<musicDataStruct> resList;
    int ret;
    if(nowListName == tr("Song List"))
    {
        ret = g_db->getSongInfoListFromLocalMusic(resList);

    }
    else
    {
        ret = g_db->getSongInfoListFromPlayList(resList,nowListName);
    }
    //    qDebug() << resList[0].filepath;
    //    if(ret == DB_OP_SUCC)
    //    {
    m_model = new ScoreInfoModel;
    m_model->add(resList);
    m_model->setView(*tableView);
//    tableView->setItemDelegate(delegate);
    initTableViewStyle();
    int num = m_model->count();
    listTotalNumLabel->setText(tr("共") + QString::number(num) + tr("首"));
    listTitleLabel->setFixedWidth(120);
    listTotalNumLabel->setFixedWidth(120);
    listTitleLabel->setText(nowListName);
}

void TableOne::initConnect()
{
    connect(tableView,&TableBaseView::doubleClicked,this,&TableOne::playSongs);
    connect(tableView,&TableBaseView::customContextMenuRequested,this,&TableOne::showRightMenu);
    connect(tableView,&TableBaseView::hoverIndexChanged,delegate,&TableViewDelegate::onHoverIndexChanged);
    connect(addMusicButton,&QToolButton::clicked,this,&TableOne::addMusicFromLocal);
    connect(this,&TableOne::countChanges,this,&TableOne::changeNumber);

}

void TableOne::initRightMenu()
{
    m_menu = new QMenu(tableView);

    playRow = new QAction("播放");
    removeRow = new QAction("删除");
    showInfoRow = new QAction("查看歌曲信息");
    addToOtherListMenu = new QMenu("添加到歌单");
    QStringList allList;
    int ret = g_db->getPlayList(allList);
    if(ret == 0)
    {
        for(int i = 0 ;i < allList.size() ;i++)
        {
            qDebug() << allList[i];
            QAction *listNameAct = new QAction;
            addToOtherListMenu->addAction(listNameAct);
            listNameAct->setText(allList[i]);
        }
    }
    else
    {
        qDebug() << "获取所有歌单失败";
    }
    connect(playRow,&QAction::triggered,this,&TableOne::playSongs);
    connect(removeRow,&QAction::triggered,this,&TableOne::deleteSongs);
    connect(showInfoRow,&QAction::triggered,this,&TableOne::showInfo);
    connect(addToOtherListMenu,&QMenu::triggered,this,&TableOne::addToOtherList);

    tableView->installEventFilter(this);
}

void TableOne::showRightMenu(const QPoint &pos)
{
    qDebug() << "鼠标位置 " << pos;
//    m_menu->move(0,0);
    m_menu->addAction(playRow);      //添加动作到菜单
    m_menu->addMenu(addToOtherListMenu);
    m_menu->addAction(removeRow);  //添加动作到菜单
    m_menu->addAction(showInfoRow); //添加动作到菜单
    QMap<int,QString> map1 = getSelectedTaskIdList();
    if(map1.size() > 1)
    {
        m_menu->removeAction(playRow);
        m_menu->removeAction(showInfoRow);
    }
    m_menu->exec(QCursor::pos());
}

void TableOne::deleteSongs()
{
    QMap<int,QString> map1 = getSelectedTaskIdList();
    while (!map1.empty()) {
        QMap<int,QString>::iterator iter;
        iter = map1.end();
        iter--;
        qDebug() << "Iterator " << iter.key(); // 迭代器
        int ret;
        if(nowListName != tr("Song List"))
        {
           ret = g_db->delMusicFromPlayList(iter.value(),nowListName);
        }
        else
        {
           ret = g_db->delMusicFromLocalMusic(iter.value());

        }
        if(ret == 0)
        {
            m_model->remove(iter.key());
            qDebug() << "删除结果" << ret << "filepath" <<iter.value();
            emit countChanges();
        }
        else
        {
            qDebug() << "delete failed" << iter.value();
        }
        map1.remove(iter.key());
    }
}

void TableOne::playSongs()
{
    qDebug() << "播放";
    int index = tableView->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    qDebug() << "tableView->selectedIndexes();" << index << date.filepath;
    pathList = m_model->getPathList();
    playController::getInstance().setCurPlaylist(nowListName,pathList);
    playController::getInstance().play(nowListName,index);
}

void TableOne::showInfo()
{
    qDebug() << "显示歌曲信息";
    int index = tableView->currentIndex().row();
    musicDataStruct date = m_model->getItem(index);
    qDebug() << "歌曲信息：" << date.title << date.filepath << date.size;
}

void TableOne::addToOtherList(QAction *listNameAction)
{
    QString listName = listNameAction->text();
    qDebug() << listName;
    QMap<int,QString> map1 = getSelectedTaskIdList();
    QMap<int,QString>::Iterator it = map1.begin();
    while(it != map1.end())
    {
        qDebug()<<it.key()<<"\t"<<it.value();
        g_db->addMusicToPlayList(it.value(),listName);
        it++;
    }
}
void TableOne::addMusicFromLocal()
{
    qDebug() << "添加歌曲";
    //获取歌曲路径
    QStringList songFiles = QFileDialog::getOpenFileNames(this, tr("Open the file"),"","音乐文件(*.mp3 *.ogg *.wma *.spx *.flac)");  //歌曲文件
    qDebug() << songFiles;
//    QFileDialog *fileDialog = new QFileDialog;
//    fileDialog->setFileMode(QFileDialog::Directory);
//    QStringList m_fileNames;
//    if (fileDialog->exec())
//    {
//        m_fileNames = fileDialog->selectedFiles();
//    }
//    qDebug() << m_fileNames;

    emit countChanges();
}

QMap<int,QString> TableOne::getSelectedTaskIdList()
{
    QTableView *m_pTableView = tableView;
    QModelIndexList selected = m_pTableView->selectionModel()->selectedRows();
//    QList<int> rows;
    QMap<int,QString> map1;
    foreach( const QModelIndex & index, selected)
    {
//        rows.append( index.row() );
        musicDataStruct date = m_model->getItem(index.row());
        map1.insert(index.row(),date.filepath);
    }
    return map1;
}

void TableOne::setHightLight(int index)
{

}

void TableOne::changeNumber()
{
    int num = m_model->count();
    listTotalNumLabel->setText(tr("共") + QString::number(num) + tr("首"));
}

QList<musicDataStruct> TableOne::getMusicList()
{
    QList<musicDataStruct> resList;
    int ret;
    if(nowListName == tr("Song List"))
    {
        ret = g_db->getSongInfoListFromLocalMusic(resList);

    }
    else
    {
        ret = g_db->getSongInfoListFromPlayList(resList,nowListName);
    }
    m_model->clear();
    m_model->add(resList);
    initTableViewStyle();

    return resList;
}

//void TableOne::mouseMoveEvent(QMouseEvent *event)
//{
//    QModelIndex index = tableView->indexAt(event->pos());
//    int row = index.row();
//    qDebug() << event->pos();
//    qDebug() << "QModelIndex index"<< index.row();
//    emit hoverIndexChanged(index);
//}
