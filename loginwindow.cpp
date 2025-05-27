#include "loginwindow.h"

LoginWindow::LoginWindow(FileOperation *fileOpt,MyWidget *parent)
    : MyWidget{parent},fileOperator(fileOpt)
{
    /*
    loginButton=new QPushButton(this);
    userName=new QLineEdit(this);
    userKey=new QLineEdit(this);
    */
    autoLog=true;
    setupUI();
    setupStyle();
    setupConnection();
}

bool LoginWindow::checkLogState(){
    if(fileOperator->signIn()==1){
        emit loggedIn(this);
        return true;
    }else return false;
}

void LoginWindow::setupUI() {
    setFixedSize(400, 600);
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("登录", this);
    rememberCheck = new QCheckBox("记住密码", this);
    autoLoginCheck = new QCheckBox("自动登录", this);
    errorLabel = new QLabel("密码不正确",this);
    //errorLabel->setAlignment(Qt::AlignCenter);
    regiLabel = new QLabel("新用户将自动注册",this);
    logo = new QLabel(this);
    logo->setPixmap(QPixmap(":/images/logo.png").scaled(400,250,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    logo->setAlignment(Qt::AlignCenter);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(logo);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(usernameEdit);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(passwordEdit);

    mainLayout->addSpacing(20);

    QHBoxLayout *checkLayout = new QHBoxLayout;
    checkLayout->addWidget(rememberCheck);
    checkLayout->addWidget(autoLoginCheck);
    mainLayout->addLayout(checkLayout);

    mainLayout->addSpacing(20);
    mainLayout->addWidget(loginButton);

    mainLayout->setSpacing(5);
    QHBoxLayout *registLayout = new QHBoxLayout;
    registLayout->addSpacing(300);
    registLayout->addWidget(regiLabel);
    mainLayout->addLayout(registLayout);

    //mainLayout->addWidget(errorLabel);
    mainLayout->addStretch();
    errorLabel->move(20,555);
    errorLabel->close();
}

void LoginWindow::setupStyle() {//登录窗口的样式表，影响登录窗口中的所有构件
    //qDebug()<<QFontDatabase().families();
    setStyleSheet(R"(
            QWidget {
                background-color: #FFFFFF;
                font-family: "Yuanti SC",sans-serif;
            }
            QLineEdit {
                padding: 5px;
                border: 2px solid #CCCCCC;
                border-radius: 5px;
                font-size: 20px;
                min-height: 35px;
            }
            QLineEdit:focus {
                border-color: #009688;
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
    errorLabel->setObjectName("errorLabel");
    errorLabel->setStyleSheet("color:#EE0000; background-color:transparent;");
}

void LoginWindow::setupConnection(){
    connect(loginButton,&QPushButton::released,this,&LoginWindow::logging);
}

void LoginWindow::logging(){
    //qDebug()<<passwordEdit->text();
    int state=fileOperator->signIn(usernameEdit->text(),passwordEdit->text());
    //qDebug()<<state<<Qt::endl;
    if(state==1){
        emit loggedIn(this);
        autoLog=autoLoginCheck->isChecked();
    }else if (state==0){
        emit registIn(this);
        autoLog=autoLoginCheck->isChecked();
    }else errorLabel->show();
    return;
}


