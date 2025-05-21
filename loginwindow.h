#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include<QPushButton>
#include<QLineEdit>
#include<QCheckBox>
#include<QLabel>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QFontDatabase>
#include<QPixmap>

class LoginWindow : public QWidget
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
public:
    explicit LoginWindow(QWidget *parent = nullptr);

    void checkLogState();
    void setupUI();
    void setupConnection();
    void setupStyle();
signals:
    void loggedIn();
};

#endif // LOGINWINDOW_H
