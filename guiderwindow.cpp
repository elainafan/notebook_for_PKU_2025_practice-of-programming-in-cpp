#include "guiderwindow.h"

Guider::Guider(MyWidget *parent)
    : MyWidget{parent}
{
    setupUI();
    setupStyle();
    setupConnection();
}

void Guider::setupUI(){
    setFixedSize(600,900);
    //setWindowTitle("Guider");
}

void Guider::setupStyle(){

}

void Guider::setupConnection(){

}

