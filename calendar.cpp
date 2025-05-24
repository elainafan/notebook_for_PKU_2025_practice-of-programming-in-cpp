#include "calendar.h"

Calendar::Calendar(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(400,281);
    setupUI();
    setupStyle();
    setupConnection();
}

void Calendar::setupUI(){

}

void Calendar::setupStyle(){
    setStyleSheet(R"(
        /*
        QWidget {
            background-color: #000000;
            font-family: "Yuanti SC",sans-serif;
            border: 2px solid #000000;
        }*/
    )");
}

void Calendar::setupConnection(){

}
