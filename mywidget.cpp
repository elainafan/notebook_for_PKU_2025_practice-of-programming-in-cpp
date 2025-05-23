#include "mywidget.h"
#define FADING_DURATION 500
#define APPEARING_DURATION 500
MyWidget::MyWidget(QWidget *parent)
    : QWidget{parent}
{}

void MyWidget::setupUI(){

}

void MyWidget::setupStyle(){

}

void MyWidget::setupConnection(){

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
