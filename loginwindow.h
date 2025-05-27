#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include"Headers.h"
#include "mywidget.h"
#include"FileOperation.h"

class LoginWindow : public MyWidget
{
    Q_OBJECT
    QLineEdit *usernameEdit;   // 用户名输入框
    QLineEdit *passwordEdit;   // 密码输入框
    QPushButton *loginButton;  // 登录按钮
    QCheckBox *rememberCheck; // 记住密码
    QCheckBox *autoLoginCheck; // 自动登录
    QLabel *errorLabel;        // 错误提示
    QLabel *regiLabel;
    QLabel *logo;
    FileOperation *fileOperator;
    bool autoLog;
public:
    explicit LoginWindow(FileOperation *fileOpt,MyWidget *parent = nullptr);

    bool checkLogState();//最初判断一下是否处于自动登录状态
    void setupUI();
    void setupConnection();
    void setupStyle();
    void logging();//按下按钮，判断是注册还是登录
    bool isAutoLogging(){return autoLog;}
signals:
    void loggedIn(MyWidget *fromWhere);
    void registIn(MyWidget *fromWhere);

public slots:
};

#endif // LOGINWINDOW_H
