#include "diarylistwidget.h"

DiaryListEntry::DiaryListEntry(const DiaryList &DL,QWidget *parent)
    :QWidget(parent)
{
    setFixedSize(wid,hei);

    diaryName = new QLabel(DL.getName(),this);
    diaryName->move(hei+15,3);
    diaryName->setFixedSize(wid-hei-20,hei-5);

    bcgrdBtn = new QPushButton(this);
    bcgrdBtn->move(hei+10,3);
    bcgrdBtn->setFixedSize(wid-hei+30,hei-3);
    if(DL.getColourType())bcgrdBtn->setCheckable(true);
    //bcgrdBtn->setChecked(true);

    diaryListIcon = new QLabel(this);
    QString filePath = QString(":/images/diaryListIcon%1.png").arg(DL.getColourType());
    if(DL.getColourType()==0) diaryListIcon->setPixmap(QPixmap(filePath).scaled(hei-15,hei-15,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    else diaryListIcon->setPixmap(QPixmap(filePath).scaled(hei-5,hei-5,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    if(DL.getColourType()==0) diaryListIcon->move(15,10);
    else diaryListIcon->move(10,5);

    underLine = new QFrame(this);
    underLine->setFrameShape(QFrame::HLine);
    underLine->setFixedSize(wid-hei-10,2);
    underLine->move(hei+10,hei-2);

    bcgrdBtn->raise();
    setupStyle();
    setupConnection();
}

void DiaryListEntry::setupStyle(){
    underLine->setStyleSheet("QFrame { background-color: rgba(120,120,120,40); border-radius:1px; margin: 7;}");
    diaryName->setStyleSheet("QLabel{font-size: 25px;}");
}

void DiaryListEntry::setupConnection(){
    connect(bcgrdBtn,&QPushButton::toggled,this,[this](bool checked){
        if(checked) emit checkUpdated();
    });
    connect(bcgrdBtn,&QPushButton::pressed,this,&DiaryListEntry::beingPressed);
}

void DiaryListEntry::setChecked(bool bl){
    bcgrdBtn->setChecked(bl);
}


/*--------------------------------------------------------------------------*/

DiaryListWidget::DiaryListWidget(FileOperation *fileOpt,QWidget *parent)
    : QWidget{parent},fileOperator(fileOpt)
{
    newdl = new NewDiaryList();
    setFixedSize(400,281);
    scrArea = new QScrollArea(parent);
    scrArea->setWidget(this);
    scrArea->setFixedSize(400,281);
    scrollBarTimer = new QTimer(this);
    scrollBarTimer->setSingleShot(true);
    scrollBarTimer->setInterval(1000);
    scrArea->verticalScrollBar()->installEventFilter(this);
    connect(scrollBarTimer, &QTimer::timeout, this, &DiaryListWidget::hideScrollBar);
    setupUI();
    setupStyle();
    setupConnection();
    scrArea->verticalScrollBar()->setVisible(false);
}

void DiaryListWidget::setupUI(){
    /*
    buildDiaryLists(vector<DiaryList>{ //测试用
        DiaryList("TEST1","weekly",1),
        DiaryList("TEST2","daily",2),
        DiaryList("TEST3","monthly",3),
        DiaryList("TEST4","yearly",4),
        DiaryList("TEST5","daily",5),
        DiaryList("TEST6","monthly",6),
        DiaryList("TEST1","weekly",7),
        DiaryList("TEST2","weekly",8),
        DiaryList("TEST3","weekly",1),
        DiaryList("TEST4","weekly",2),
        DiaryList("TEST5","weekly",3),
        DiaryList("TEST6","weekly",4)
    });*/

}

void DiaryListWidget::setupStyle(){
    scrArea->setStyleSheet(R"(
        QWidget{
            border: none;
        }
QScrollBar:vertical {
        background: rgba(200, 200, 200, 50);  /* 半透明背景 */
        width: 10px;                          /* 滚动条宽度 */
        margin: 0px;                          /* 边距清零 */
    }

    /* 垂直滚动条滑块 */
    QScrollBar::handle:vertical {
        background: rgba(150, 150, 150, 150); /* 半透明滑块 */
        border-radius: 5px;                    /* 圆角 */
        min-height: 20px;                      /* 最小高度 */
    }

    /* 垂直滚动条滑块悬停效果 */
    QScrollBar::handle:vertical:hover {
        background: rgba(120, 120, 120, 200);
    }

    /* 隐藏垂直滚动条的上下箭头 */
    QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
        border: none;
        background: none;
        height: 0px;                          /* 高度设为0彻底隐藏 */
    }

    /* 垂直滚动条背景区域 */
    QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
        background: none;
    }
    QPushButton{
        background: rgba(120,120,120,0);
        border:none;
        border-radius:0px;
    }QPushButton:checked{
        background: rgba(120,120,120,40);
    }QPushButton:pressed{
        background: rgba(120,120,120,40);
    }
    )");
    scrArea->verticalScrollBar()->setProperty("visible", false);
}



void DiaryListWidget::setupConnection(){

}

void DiaryListWidget::buildDiaryLists(const QVector<DiaryList> &vec){
    setFixedSize(wid,(hei+11)*vec.size());
    if(mainLayout)delete mainLayout;
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,5);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(10);
    for(int i=0;i<dlEntry.size();i++)delete dlEntry[i];
    dlEntry.clear();
    for(int i=0;i<vec.size();i++){
        dlEntry.push_back( new DiaryListEntry(vec[i],this));
        mainLayout->addWidget(dlEntry[i]);

        connect(dlEntry[i],&DiaryListEntry::checkUpdated,this,[this,i](){
            updateBtnState(i);
            emit changeList(i);
        });
    }if(newEntry)delete newEntry;
    newEntry = new DiaryListEntry(DiaryList("添加新笔记本","weekly",0),this);
    mainLayout->addWidget(newEntry);
    connect(newEntry,&DiaryListEntry::beingPressed,newdl,&NewDiaryList::show);
    connect(newdl,&NewDiaryList::diaryListCreated,this,&DiaryListWidget::addNewList);
    return;
}

bool DiaryListWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == scrArea->verticalScrollBar()) {
        // 检测滚动条值变化和鼠标事件
        if (event->type() == QEvent::Wheel ||
            event->type() == QEvent::MouseMove ||
            event->type() == QEvent::MouseButtonPress) {

            // 显示滚动条
            scrArea->verticalScrollBar()->setVisible(true);

            // 重置定时器
            scrollBarTimer->start();

            return false; // 让事件继续传播
        }
    }
    return QWidget::eventFilter(watched, event);
}

// 隐藏滚动条的槽函数
void DiaryListWidget::hideScrollBar()
{
    scrArea->verticalScrollBar()->setVisible(false);
}

void DiaryListWidget::updateBtnState(const int &num){
    for(int i=0;i<dlEntry.size();i++){
        if(i!=num)dlEntry[i]->setChecked(false);
    }
}

void DiaryListWidget::addNewList(const DiaryList &dl){
    fileOperator->newFolder(dl);
    buildDiaryLists(fileOperator->allFolders());
}
