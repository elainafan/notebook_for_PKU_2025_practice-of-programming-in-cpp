#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent)
    : QWidget{parent}
{
    setFixedSize(400,55);
    setupUI();
    setupStyle();
    setupConnection();
}

void SearchWidget::setupUI(){
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    searchIcon = new QLabel(this);
    searchIcon->setPixmap(QPixmap(":/images/searchIcon.png").scaled(height()-15,height()-15,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    searchEdit = new QLineEdit(this);
    searchEdit->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    searchEdit->setFixedSize(width()-height()-40,height()-15);
    searchEdit->setPlaceholderText("按关键词搜索");
    mainLayout->setContentsMargins(15,7,15,7);
    mainLayout->setSpacing(0);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(searchIcon);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(searchEdit);
}

void SearchWidget::setupStyle(){
    setStyleSheet(R"(
            QLineEdit {
                padding: 5px;
                border:0px;
                border-bottom: 2px solid #CCCCCC;
                font-size: 25px;
            }
            QLineEdit:focus {
                border-color: #660077;
            })");
}

void SearchWidget::setupConnection(){
    connect(searchEdit,&QLineEdit::editingFinished,this,[this](){
        emit searchText(searchEdit->text());
    });
}
