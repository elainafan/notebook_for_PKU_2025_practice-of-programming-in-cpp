#include "mywidget.h"

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
    fadeIn->setDuration(300);
    fadeIn->setStartValue(1.0);
    fadeIn->setEndValue(0.0);

    connect(fadeIn,&QPropertyAnimation::finished,this,&MyWidget::close);
    connect(fadeIn,&QPropertyAnimation::finished,this,&MyWidget::fadeAway);
    fadeIn->start();
}

void MyWidget::appear(){
    QPropertyAnimation *appearing=new QPropertyAnimation(this,"windowOpacity");
    appearing->setDuration(300);
    appearing->setStartValue(0.0);
    appearing->setEndValue(1.0);
    show();
    appearing->start();
}
