#include "loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget{parent}
{
    /*
    loginButton=new QPushButton(this);
    userName=new QLineEdit(this);
    userKey=new QLineEdit(this);
    */
    setupUI();
    setupStyle();
    setWindowTitle("LOG IN");
}

void LoginWindow::checkLogState(){

    //emit loggedIn();
}

void LoginWindow::setupUI() {//登陆窗口的UI设置，可以调整位置和组成构件
    setWindowTitle("用户登录");
    setFixedSize(400, 600); // 固定窗口大小

    // 创建组件
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("请输入用户名");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("请输入密码");
    passwordEdit->setEchoMode(QLineEdit::Password); // 密码模式

    loginButton = new QPushButton("登录", this);
    rememberCheck = new QCheckBox("记住密码", this);
    autoLoginCheck = new QCheckBox("自动登录", this);
    errorLabel = new QLabel(this);
    errorLabel->setAlignment(Qt::AlignCenter);
    regiLabel = new QLabel("新用户将自动注册",this);
    logo = new QLabel(this);
    logo->setPixmap(QPixmap(":/images/logo.png").scaled(450,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    // 布局管理
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(logo);//此处可以插入一个logo图片替代空格
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

    mainLayout->addWidget(errorLabel);
    mainLayout->addStretch();
}

void LoginWindow::setupStyle() {//登录窗口的样式表，影响登录窗口中的所有构件
    //qDebug()<<QFontDatabase().families();
    setStyleSheet(R"(
            QWidget {
                background-color: #FFFFF9;
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
}
