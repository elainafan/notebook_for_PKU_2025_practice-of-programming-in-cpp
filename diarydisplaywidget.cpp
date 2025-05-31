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
    QVector<QPixmap> imgVec = diary.getImages();
    QHBoxLayout *imgLayout = new QHBoxLayout(this);
    imgLayout->setContentsMargins(10,10,10,10);
    imgLayout->setSpacing(10);
    imgLayout->setAlignment(Qt::AlignTop);
    int cnt=min(2,(int)imgVec.size());
    int rateWid = cnt==2? 12:16,rateHei=9;
    for(int i=0;i<cnt;i++){
        QLabel *img = new QLabel(this);
        if(imgVec[i].height()*rateWid<imgVec[i].width()*rateHei)
            img->setPixmap(
                imgVec[i].copy(QRect(0,0,imgVec[i].height()*rateWid/rateHei,imgVec[i].height()))
                .scaled(DIARY_WID/cnt,10000,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        else img->setPixmap(
                imgVec[i].copy(QRect(0,0,imgVec[i].width(),imgVec[i].width()*rateHei/rateWid))
                .scaled(DIARY_WID/cnt,10000,Qt::KeepAspectRatio,Qt::SmoothTransformation));
        img->setStyleSheet("background:transparent");
        imgLayout->addWidget(img);
    }mainLayout->addLayout(imgLayout);
    //if(cnt==2)mainLayout->addSpacing(-100);
    mainLayout->setAlignment(Qt::AlignTop);
    QLabel *txt = new QLabel(diary.getMarkdownHtmlPreview(),this);
    txt->setStyleSheet("QLabel{"
                       "background:transparent;"
                       "padding: 20px;"
                       "}");
    mainLayout->addWidget(txt);

}

void DiaryWidget::setupStyle(){
    //setObjectName("diarywidget");
    setAutoFillBackground(true);
    setStyleSheet(R"(
    )");

}

void DiaryWidget::setupConnection(){

}

DiaryDisplayWidget::DiaryDisplayWidget(QVector<Diary> dVec,QWidget *parent)
    : MyWidget(parent),diaryVec(dVec)
{
    setFixedSize(DIARY_WID+200,dVec.size()*(DIARY_HEI+20)+250);
    setupUI();
    setupStyle();
    setupConnection();
}

void DiaryDisplayWidget::setupUI(){

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins((800-DIARY_WID)/2-3,0,0,0);
    mainLayout->setSpacing(20);
    for(int i=0;i<diaryVec.size();i++){
        diaWidgVec.push_back(new DiaryWidget(diaryVec[i],this));
        //diaWidgVec[i]->setStyleSheet("background:#888888;");
        mainLayout->addWidget(diaWidgVec[i]);
    }
    newDiary = new QPushButton(this);
    newDiary->setFixedSize(100,100);
    QHBoxLayout *btnLayout = new QHBoxLayout(this);
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
