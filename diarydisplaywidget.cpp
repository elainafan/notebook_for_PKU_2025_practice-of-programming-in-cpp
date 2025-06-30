#include "diarydisplaywidget.h"

void DiaryWidget::setupUI(){
    setFixedSize(DIARY_WID,DIARY_HEI);
    diaryBlock = new QLabel(this);
    diaryBlock->setFixedSize(DIARY_WID+20,DIARY_HEI);
    diaryBlock->setPixmap(QPixmap(":/images/diaryBackground.png")
                              .scaled(DIARY_WID,DIARY_HEI,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(35,35,35,35);
    mainLayout->setSpacing(0);
    QHBoxLayout *dateTime = new QHBoxLayout();
    QString daystr;

    if(diary.getDiaryType()=="weekly")daystr =
            (diary.getDate().addDays(1-diary.getDate().dayOfWeek())).toString("yyyy.MM.dd") +
            " — " +
            (diary.getDate().addDays(7-diary.getDate().dayOfWeek())).toString("yyyy.MM.dd");
    else if(diary.getDiaryType()=="monthly")daystr = diary.getDate().toString("yyyy.MM");
    else if(diary.getDiaryType()=="yearly")daystr = diary.getDate().toString("yyyy");
    else daystr= diary.getDate().toString("yyyy.MM.dd");

    QLabel *dateTitle = new QLabel(daystr,this);
    dateTitle->setStyleSheet(R"(
        QLabel{
            background:transparent;
            font-family: "Yuanti SC","YouYuan",sans-serif;
            font-size: 32px;
            padding: 10px;
        }
    )");

    QLabel *timeTitle = new QLabel(diary.getTime().toString("HH:mm"),this);
    timeTitle->setStyleSheet(R"(
        QLabel{
            background:transparent;
            font-family: "Yuanti SC","YouYuan", sans-serif;
            font-size: 20px;
            color: #777777;
            padding: 25px;
        }
    )");
    dateTime->addWidget(dateTitle);
    dateTime->addSpacing(-300);
    //dateTime->addWidget(timeTitle);
    timeTitle->close();


    dateTime->setAlignment(Qt::AlignBottom);
    mainLayout->addLayout(dateTime);
    QVector<QPixmap> imgVec = diary.getImages();
    QHBoxLayout *imgLayout = new QHBoxLayout();
    imgLayout->setContentsMargins(10,10,10,10);
    imgLayout->setSpacing(10);
    imgLayout->setAlignment(Qt::AlignTop);
    int cnt=min(2,(int)imgVec.size());
    int rateWid = cnt==2? 12:16,rateHei=9;
    for(int i=0;i<cnt;i++){
        QLabel *img = new QLabel(this);
        int srcW = imgVec[i].width();
        int srcH = imgVec[i].height();
        if(srcH * rateWid < srcW * rateHei) {
            // 需要裁剪宽度
            int targetW = srcH * rateWid / rateHei;
            int x = (srcW - targetW) / 2; // 居中
            img->setPixmap(
                imgVec[i].copy(QRect(x, 0, targetW, srcH))
                .scaled(DIARY_WID/cnt, 10000, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            // 需要裁剪高度
            int targetH = srcW * rateHei / rateWid;
            int y = (srcH - targetH) / 2; // 居中
            img->setPixmap(
                imgVec[i].copy(QRect(0, y, srcW, targetH))
                .scaled(DIARY_WID/cnt, 10000, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }img->setStyleSheet("background:transparent");
        imgLayout->addWidget(img);
    }
    if(cnt)mainLayout->addLayout(imgLayout);
    //if(cnt==2)mainLayout->addSpacing(-100);
    mainLayout->setAlignment(Qt::AlignTop);
    QLabel *txt = new QLabel(diary.getMarkdownHtmlPreview(),this);
    txt->setWordWrap(true);
    txt->setStyleSheet("QLabel{"
                       "background:transparent;"
                       "padding: 20px;"
                       "}");
    mainLayout->addWidget(txt);

    star = new QPushButton(this);
    star->setCheckable(true);
    star->setFixedSize(30,30);
    star->move(DIARY_WID-70,40);
    //star->close();
}

void DiaryWidget::setupStyle(){
    //setObjectName("diarywidget");
    setAutoFillBackground(true);
    star->setStyleSheet(R"(
        QAbstructButton{
            background:transparent;
        }QLabel{
            background:transparent;
        }QWidget{
            background:transparent;
        }
        QPushButton {
            background:transparent;
            border: none;
            border-image: url(":/images/unstar.png") 0 0 0 0 stretch stretch;
        }QPushButton:checked{
            border-image: url(":/images/starred.png") 0 0 0 0 stretch stretch;
        }
    )");
    star->setAttribute(Qt::WA_TranslucentBackground);

}

void DiaryWidget::setupConnection(){
    connect(star,&QPushButton::toggled,this,[this](bool checked){
        emit toggleStar(QDateTime(diary.getDate(),diary.getTime()).toString("yyyy_MM_dd_HH_mm_ss")+".md");
    });
}

void DiaryWidget::mousePressEvent(QMouseEvent *event)
{
    // 判断是哪个鼠标按键
    if (event->button() == Qt::LeftButton) {
        // 左键点击
        qDebug()<<"clicked";
        emit leftClicked(this);
        emit diaryClicked(this, Qt::LeftButton);
    }
    else if (event->button() == Qt::RightButton) {
        // 右键点击
        qDebug()<<"rightclicked";
        emit rightClicked(this);
        emit diaryClicked(this, Qt::RightButton);
    }

    // 调用父类实现以保持其他默认行为
    QWidget::mousePressEvent(event);
}

void DiaryWidget::setStar(bool isstarred){
    QSignalBlocker blocker(star);
    star->setChecked(isstarred);
}


/*-------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*/

DiaryDisplayWidget::DiaryDisplayWidget(QVector<Diary> dVec,FileOperation *fileOpt,bool add,QWidget *parent)
    : MyWidget(parent),diaryVec(dVec),fileOperator(fileOpt)
{
    setFixedSize(DIARY_WID+200,dVec.size()*(DIARY_HEI+20)+250*add);
    setupUI(add);
    setupStyle();
    setupConnection();
}

void DiaryDisplayWidget::setupUI(bool can_add){

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins((800-DIARY_WID)/2-3,0,0,0);
    mainLayout->setSpacing(20);
    QVector<Diary> allStr = fileOperator->allStarred();
    for(int i=0;i<diaryVec.size();i++){
        diaWidgVec.push_back(new DiaryWidget(diaryVec[i],this));
        //diaWidgVec[i]->setStyleSheet("background:#888888;");
        mainLayout->addWidget(diaWidgVec[i]);
        bool fl = false;
        for(int j=0;j<allStr.size();j++){
            if(allStr[j].getDateTime()==diaryVec[i].getDateTime()){
                fl=true;
                break;
            }
        }
        diaWidgVec[i]->setStar(fl);
        connect(diaWidgVec[i],&DiaryWidget::leftClicked,this,[this,i](){
            qDebug()<<"openDiary";
            emit openDiary(diaryVec[i]);
        });
        connect(diaWidgVec[i],&DiaryWidget::toggleStar,this,[this,i](QString str){
            qDebug()<<"toggle: "<<str;
            fileOperator->setStar(str);
        });
    }
    qDebug()<<"HHHHHHHH is :"<<can_add;

    newDiary = new QPushButton(this);
    newDiary->setFixedSize(100,100);
    connect(newDiary,&QPushButton::released,this,&DiaryDisplayWidget::newingDiary);

    if(!can_add){
        newDiary->close();
        return;
    }QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setAlignment(Qt::AlignCenter);
    btnLayout->addWidget(newDiary);
    btnLayout->addSpacing(170);
    mainLayout->addSpacing(-80);
    mainLayout->addLayout(btnLayout);
}

void DiaryDisplayWidget::setupStyle(){
    newDiary->setStyleSheet(R"(
        QPushButton {
            font: bold 14px '微软雅黑';
            color: white;
            background:none;
            border-image: url(":/images/newDiary.png") 0 0 0 0 stretch stretch;
        }QPushButton:hover{
            border-image: url(":/images/newDiary_hover.png") 0 0 0 0 stretch stretch;
        }QPushButton:pressed{
            border-image: url(":/images/newDiary_pressed.png") 0 0 0 0 stretch stretch;
        }
    )");

}

void DiaryDisplayWidget::setupConnection(){

}

