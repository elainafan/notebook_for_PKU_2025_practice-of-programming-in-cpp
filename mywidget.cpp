#include "mywidget.h"
#define FADING_DURATION 500
#define APPEARING_DURATION 500

MyWidget::MyWidget(QWidget *parent)
    : QWidget(parent) // 关键：调用基类构造函数
{
    //setupUI();
    //setupConnection();
    //setupStyle();
}

void MyWidget::setupUI() {
    // 设置布局和子控件
    /*
    QPushButton *button = new QPushButton("Test", this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(button);
    resize(300, 200); // 设置默认尺寸
    */
}

void MyWidget::setupConnection() {
    // 连接信号槽（示例）
    //connect(button, &QPushButton::clicked, this, &MyWidget::fade);
}

void MyWidget::setupStyle() {
    //setStyleSheet("background: #F0F0F0; border: 1px solid #CCCCCC;");
}




void MyWidget::fade(){
    QPropertyAnimation *fadeIn = new QPropertyAnimation(this, "windowOpacity");
    fadeIn->setDuration(FADING_DURATION);
    fadeIn->setStartValue(1.0);
    fadeIn->setEndValue(0.0);

    QPropertyAnimation *riseUp = new QPropertyAnimation(this,"pos");
    riseUp->setDuration(FADING_DURATION);
    riseUp->setEasingCurve(QEasingCurve::InSine);
    riseUp->setStartValue(pos());
    riseUp->setEndValue(pos()-QPoint(0,50));

    QParallelAnimationGroup *fadeGroup = new QParallelAnimationGroup(this);
    fadeGroup->addAnimation(fadeIn);
    fadeGroup->addAnimation(riseUp);

    connect(fadeGroup,&QParallelAnimationGroup::finished,this,&MyWidget::close);
    connect(fadeGroup,&QParallelAnimationGroup::finished,this,&MyWidget::fadeAway);
    fadeGroup->start();
}

void MyWidget::appear(){
    show();

    QPropertyAnimation *appearing=new QPropertyAnimation(this,"windowOpacity");
    appearing->setDuration(APPEARING_DURATION);
    appearing->setStartValue(0.0);
    appearing->setEndValue(1.0);

    QPropertyAnimation *fallDown = new QPropertyAnimation(this,"pos");
    fallDown->setDuration(APPEARING_DURATION);
    fallDown->setEasingCurve(QEasingCurve::OutSine);
    fallDown->setStartValue(pos()-QPoint(0,25));
    fallDown->setEndValue(pos()+QPoint(0,25));

    QParallelAnimationGroup *appearGroup = new QParallelAnimationGroup(this);
    appearGroup->addAnimation(appearing);
    appearGroup->addAnimation(fallDown);

    connect(appearGroup,&QParallelAnimationGroup::finished,this,&MyWidget::appeared);

    appearGroup->start();
}
