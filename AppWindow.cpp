#include "AppWindow.h"
#include "CryptoUtils.h"


AppWindow::AppWindow(FileOperation *fileOpt,MyWidget *parent)
    : MyWidget(parent),fileOperator(fileOpt)
{
    setupUI();
    setupStyle();
    setupConnection();
}

AppWindow::~AppWindow()
{
}

void AppWindow::setupUI(){
    setWindowTitle("Easy Diary");
    setFixedSize(1200, 900);

    leftColumn = new QWidget(this);
    searchWidgetWin = new QWidget(leftColumn);
    diaryListWin = new QWidget(leftColumn);
    calendarWin = new QWidget(leftColumn);
    reminderWin = new QWidget(leftColumn);



    leftColumn->setGeometry(0,0,400,900);
    rightColumn = new QWidget(this);
    rightColumn->setGeometry(400,-2,1202,902);
    //leftColumn->setFixedSize(100,100);
    QVBoxLayout *leftLayer = new QVBoxLayout(leftColumn);
    QFrame *line[10];
    for(int i=0;i<3;i++){
        line[i]= new QFrame(leftColumn);
        line[i]->setFrameShape(QFrame::HLine);
        line[i]->setFrameShadow(QFrame::Sunken);
        line[i]->setStyleSheet("QFrame { background-color: #888888; margin: 10;}");
        line[i]->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        //line[i]->setLineWidth(5);
        //line->show();
        line[i]->setFixedSize(400,1);
        //line->move(10,10);
        //line->raise();
    }
    leftLayer->setContentsMargins(0,0,0,0);
    leftLayer->setSpacing(0);
    leftLayer->addWidget(searchWidgetWin,1);
    leftLayer->addWidget(line[0]);
    leftLayer->addWidget(diaryListWin,5);
    leftLayer->addWidget(line[1]);
    leftLayer->addWidget(calendarWin,5);
    leftLayer->addWidget(line[2]);
    leftLayer->addWidget(reminderWin,5);
    //line->show();
    //leftColumn->show();
    /*
    leftColumn->show();
    searchWidget->show();
    diaryList->show();
    calendar->show();
    reminder->show();
    */
    searchWidget= new SearchWidget(searchWidgetWin);
    diaryList = new DiaryListWidget(fileOperator,diaryListWin);
    calendar = new Calendar("weekly",calendarWin);
    reminder = new ReminderWidget(reminderWin);


    userInfo = new UserInfoWidget(fileOperator,this);
    userInfo->move(1050,0);

}

void AppWindow::setupStyle(){
    /*setStyleSheet(R"(
            QWidget{
                background-color: #FFFFFF;
                font-family: "Yuanti SC",sans-serif;
            }
        )");*/

    leftColumn->setStyleSheet(R"(
            QWidget {
                background-color: #F3F3F3;
                font-family: "Yuanti SC",sans-serif;
                /*border: 2px solid #000000;*/
            }
            QPushButton {
                background-color: #009688;
                color: white;
                padding: 12px;
                border: none;
                border-radius: 5px;
                font-size: 20px;
            }
            QPushButton:hover {
                background-color: #008879;
            }QPushButton:pressed{
                background-color: #00796B;
            }
            QCheckBox {
                color: #666666;
                font-size: 20px;
            }
            QLabel {
                color: #666666;
                font-size: 15px;
            }
        )");
    rightColumn->setStyleSheet(R"(
            QWidget{
                background-color: #FFFFFA;
                font-family: "Yuanti SC",sans-serif;
                /*border: 2px solid #AAAAAA;*/
            }
        )");


}

void AppWindow::setupConnection(){
    connect(userInfo, &UserInfoWidget::logoutRequested, this, [this]() {
        fileOperator->signOut();
        emit signOut(this);
        //qDebug()<<"lambda...";
    });
    connect(this,&AppWindow::appeared, this, [this](){
        buildDiaries(QVector<Diary>{
            Diary("test0",QDateTime(QDate::currentDate(),QTime()),"TEST","## TEST"),
            Diary("test",QDateTime(QDate::currentDate(),QTime()),"TEST","## This is a piece of test text.",
                  QVector<QPixmap>{QPixmap(":/images/testImage.png"),QPixmap(":/images/testImage.png")}),
            Diary("test2",QDateTime(QDate::currentDate(),QTime()),"TEST2","## This is another piece of test text.",
                  QVector<QPixmap>{QPixmap(":/images/testImage.png")})
        });
    });
    connect(diaryDisplay,&DiaryDisplayWidget::openDiary,this,[this](Diary dia){
        if(mdEditor)delete mdEditor;
        mdEditor = new MarkdownEditorWidget(dia);
    });
    connect(diaryList,&DiaryListWidget::newListAdded,this,[this](){
        diaryListVec = fileOperator->allFolders();
        curDiaryList = -1;
    });
    connect(mdEditor,&MarkdownEditorWidget::saved,this,&AppWindow::refresh);
    connect(diaryList,&DiaryListWidget::changeList,this,&AppWindow::refresh);
    connect(calendar,&Calendar::dateUpdated,this,&AppWindow::refresh);

    /*
    connect(calendar, &Calendar::dateUpdated, this, [this](){
        showDiaries(QVector<Diary>{
            Diary("test",QDateTime(calendar->getCurDate(),QTime(0)),"TEST","## This is a piece of test text.",
                  QVector<QPixmap>{QPixmap(":/images/logo.png"),QPixmap(":/images/diaryListIcon.png")}),
            Diary("test2",QDateTime(calendar->getCurDate(),QTime(0)),"TEST2","## This is another piece of test text.",
                  QVector<QPixmap>{QPixmap(":/images/logo.png")})
        });
    });
    */
}

void AppWindow::setupUserInfo(){
    diaryListVec=fileOperator->allFolders(); //导入所有日记本
    diaryList->buildDiaryLists(diaryListVec);
    curDiaryList = -1; //未选中日记本
    QString userName = fileOperator->username;
    QPixmap avatar = fileOperator->getProfilePicture();

    userInfo->setUserName(userName);
    if (!avatar.isNull()) {
        userInfo->setUserAvatar(avatar);
    }
    userInfo->move(1140-15*userName.length(),0);
}

void AppWindow::buildDiaries(QVector<Diary> diaryVec){ //新建右侧的日记预览滚动框
    if(diaryScroll)delete diaryScroll;
    diaryScroll = new QScrollArea(rightColumn);
    diaryScroll->move(5,0);
    diaryScroll->setStyleSheet("border:none;");
    diaryScroll->setFixedSize(rightColumn->size());
    diaryDisplay = new DiaryDisplayWidget(diaryVec,rightColumn);
    diaryScroll->setWidget(diaryDisplay);
    diaryScroll->show();
    //diaryScroll->setStyleSheet("border:2px solid green;");
    /*
    QVBoxLayout *rightLayer = new QVBoxLayout(rightColumn);
    rightLayer->setContentsMargins(0,0,0,0);
    rightLayer->setAlignment(Qt::AlignCenter);
    rightLayer->addWidget(diaryScroll->scrArea);
    */
    //diaryScroll->appear();

}

void AppWindow::refresh(){
    if(curDiaryList<0)return;
    buildDiaries(
        fileOperator->findFileByTime(
            QDateTime(calendar->getCurDate(),QTime()),
            QDateTime(calendar->getCurDate().addDays(1),QTime()),
            diaryListVec[curDiaryList]
        )
    );
}
