#ifndef USERINFOWIDGET_H
#define USERINFOWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QFileDialog>
#include <QInputDialog>
#include "Headers.h"
#include"mywidget.h"

class UserInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UserInfoWidget(QWidget *parent = nullptr);
    ~UserInfoWidget();

    // 设置用户信息
    void setUserName(const QString &name);
    void setUserAvatar(const QPixmap &avatar);
    QString getUserName() const;
    QPixmap getUserAvatar() const;

signals:
    void logoutRequested();  // 退出登录信号
    void userNameChanged(const QString &newName);  // 用户名修改信号
    void userAvatarChanged(const QPixmap &newAvatar);  // 头像修改信号

private slots:
    void showMenu();  // 显示下拉菜单
    void changeUserName();  // 修改用户名
    void changeUserAvatar();  // 修改头像
    void logout();  // 退出登录

private:
    void setupUI();
    void setupStyle();
    void setupConnections();

    QLabel *avatarLabel;     // 头像标签
    QLabel *userNameLabel;   // 用户名标签
    QPushButton *menuButton; // 菜单按钮
    QMenu *dropdownMenu;     // 下拉菜单

    QString currentUserName;
    QPixmap currentAvatar;

    // 菜单尺寸
    static constexpr int AVATAR_SIZE = 30;  // 头像大小
    static constexpr int MENU_WIDTH = 150;  // 菜单宽度
};

#endif // USERINFOWIDGET_H
