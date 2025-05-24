#include "AppWindow.h"
#include "CryptoUtils.h"


AppWindow::AppWindow(MyWidget *parent)
    : MyWidget(parent)
{
    setupUI();
    setupStyle();
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
        line[i]->setStyleSheet("QFrame { background-color: #888888; margin: 7;}");
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
    diaryList = new DiaryListWidget(diaryListWin);
    calendar = new Calendar(calendarWin);
    reminder = new Reminder(reminderWin);
}

void AppWindow::setupStyle(){
    setStyleSheet(R"(
            QWidget{
                background-color: #FFFFFC;
                font-family: "Yuanti SC",sans-serif;
            }
        )");
    leftColumn->setStyleSheet(R"(
            QWidget {
                background-color: #EEEEE9;
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
                background-color: #FFFFFC;
                font-family: "Yuanti SC",sans-serif;
                /*border: 2px solid #AAAAAA;*/
            }
        )");
}
