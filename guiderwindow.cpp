#include "guiderwindow.h"

Guider::Guider(MyWidget *parent)
    : MyWidget{parent}
{
    setupUI();
    setupStyle();
    setupConnection();
}

void Guider::setupUI(){
    setFixedSize(500,700);
    welcomeLabel= new QLabel("欢迎使用Easy Diary！",this);
    textLabel = new QLabel("<p style='line-height:130%'>"
                           "- 在这里，你可以建立真正安全的、自由的私人日记本。"
                           "日记、周记、月记、年记、自定义日记……随心记。<br/>"
                           "- 你的日记内容将通过安全的加密方式被保护，完全存储于本地，不需担心个人隐私泄露。<br/>"
                           "- 日记本内容支持Markdown语法，实时渲染pdf文件，排版自由快捷。"
                           "同时支持本地图片直接导入，不需图床，不依赖本地路径，不必担心内容丢失。<br/>"
                           "<br>还在等待什么？快开始使用吧！"
                           "</p>",this);
    enterButton = new QPushButton(this);
    enterButton->setFixedSize(80,80);
    enterButton->move(210,600);
    QVBoxLayout *mainLayer = new QVBoxLayout(this);
    mainLayer->addWidget(welcomeLabel);
    mainLayer->addSpacing(0);
    mainLayer->addWidget(textLabel);
    mainLayer->addSpacing(100);
    //mainLayer->addWidget(enterButton);
    //setWindowTitle("Guider");

}

void Guider::setupStyle(){
    setStyleSheet(R"(
            QWidget {
                background-color: #FFFFFF;
                font-family: "Yuanti SC",sans-serif;
            }
        )");
    welcomeLabel->setStyleSheet(R"(
            QLabel {
                font: 30px "Heiti SC";
                color: #9900CC;
                padding: 10px;
                background: none;
                border:none;
            }
        )");
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet(R"(
            QLabel {
                font: 20px "Yuanti SC";
                color: #555555;
                padding: 10px;
                background: none;
                border:none;
            }
        )");
    enterButton->setStyleSheet(R"(
            QPushButton {
                font: bold 14px '微软雅黑';
                color: white;
                background:none;
                border-image: url(":/images/enter.png") 0 0 0 0 stretch stretch;
            }QPushButton:hover{
                border-image: url(":/images/enter_hover.png") 0 0 0 0 stretch stretch;
            }QPushButton:pressed{
                border-image: url(":/images/enter_pressed.png") 0 0 0 0 stretch stretch;
            }
        )");
}

void Guider::setupConnection(){

}

