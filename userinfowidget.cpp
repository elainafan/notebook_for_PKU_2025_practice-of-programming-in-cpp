#include "UserInfoWidget.h"

UserInfoWidget::UserInfoWidget(QWidget *parent) : QWidget(parent)
{
    setFixedSize(MENU_WIDTH+10,AVATAR_SIZE+10);
    setupUI();
    setupStyle();
    setupConnections();

    // 设置默认头像
    QPixmap defaultAvatar(":/images/default_avatar.png");
    if (defaultAvatar.isNull()) {
        // 如果没有默认头像资源，创建一个彩色圆形作为默认头像
        defaultAvatar = QPixmap(AVATAR_SIZE, AVATAR_SIZE);
        defaultAvatar.fill(Qt::transparent);
        QPainter painter(&defaultAvatar);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(100, 149, 237));  // 使用一个好看的蓝色
        painter.drawEllipse(0, 0, AVATAR_SIZE, AVATAR_SIZE);

        // 在圆形内绘制默认文本
        painter.setPen(Qt::white);
        painter.setFont(QFont("Arial", 16, QFont::Bold));
        painter.drawText(defaultAvatar.rect(), Qt::AlignCenter, "U");
    }

    setUserAvatar(defaultAvatar);
    setUserName("用户");
}

UserInfoWidget::~UserInfoWidget()
{
}

void UserInfoWidget::setupUI()
{
    // 创建主布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);
    mainLayout->setSpacing(10);

    // 创建头像标签
    avatarLabel = new QLabel(this);
    avatarLabel->setFixedSize(AVATAR_SIZE, AVATAR_SIZE);
    avatarLabel->setScaledContents(true);

    // 创建用户名标签
    userNameLabel = new QLabel(this);
    userNameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // 创建菜单按钮
    menuButton = new QPushButton(this);
    //menuButton->setIcon(QIcon(":/images/dropdown.png")); // 下拉图标
    menuButton->setFixedSize(MENU_WIDTH, AVATAR_SIZE);
    menuButton->setCursor(Qt::PointingHandCursor);

    // 创建下拉菜单
    dropdownMenu = new QMenu(this);
    dropdownMenu->setFixedWidth(MENU_WIDTH);

    QAction *changeNameAction = new QAction("修改用户名", this);
    QAction *changeAvatarAction = new QAction("修改头像", this);
    QAction *logoutAction = new QAction("退出登录", this);

    // 添加操作到菜单
    dropdownMenu->addAction(changeNameAction);
    dropdownMenu->addAction(changeAvatarAction);
    dropdownMenu->addSeparator();
    dropdownMenu->addAction(logoutAction);

    // 添加控件到布局
    mainLayout->addWidget(avatarLabel);
    mainLayout->addWidget(userNameLabel);
    //mainLayout->addWidget(menuButton);
    setLayout(mainLayout);
}

void UserInfoWidget::setupStyle()
{
    setStyleSheet(R"(
        UserInfoWidget {
            background-color: transparent;
        }

        QLabel#userNameLabel {
            color: #333333;
            font-size: 20px;
            font-weight: bold;
        }

        QPushButton {
            background-color: transparent;
            border: none;
        }



        QMenu {
            background-color: white;
            border: 1px solid #cccccc;
            padding: 5px;
        }

        QMenu::item {

            padding: 8px 25px;
            background-color: transparent;
        }

        QMenu::item:selected {
            background-color: #f0f0f0;
        }

        QMenu::separator {
            height: 1px;
            background-color: #e0e0e0;
            margin: 5px 0px;
        }
    )");

    // 给用户名标签添加对象名，用于样式识别
    userNameLabel->setObjectName("userNameLabel");

    // 设置头像为圆形
    avatarLabel->setStyleSheet("border-radius: " + QString::number(AVATAR_SIZE/2) + "px; background-color: transparent;");
}

void UserInfoWidget::setupConnections()
{
    // 连接菜单按钮点击信号
    connect(menuButton, &QPushButton::clicked, this, &UserInfoWidget::showMenu);

    // 连接菜单动作信号
    connect(dropdownMenu->actions().at(0), &QAction::triggered, this, &UserInfoWidget::changeUserName);
    connect(dropdownMenu->actions().at(1), &QAction::triggered, this, &UserInfoWidget::changeUserAvatar);
    connect(dropdownMenu->actions().at(3), &QAction::triggered, this, &UserInfoWidget::logout);
}

void UserInfoWidget::setUserName(const QString &name)
{
    currentUserName = name;
    userNameLabel->setText(name);
}

void UserInfoWidget::setUserAvatar(const QPixmap &avatar)
{
    currentAvatar = avatar;

    // 创建圆形头像
    QPixmap roundedAvatar(avatar.size());
    roundedAvatar.fill(Qt::transparent);

    QPainter painter(&roundedAvatar);
    painter.setRenderHint(QPainter::Antialiasing);

    // 创建圆形裁剪路径
    QPainterPath path;
    path.addEllipse(0, 0, avatar.width(), avatar.height());
    painter.setClipPath(path);

    // 绘制头像
    painter.drawPixmap(0, 0, avatar);

    // 设置到标签
    avatarLabel->setPixmap(roundedAvatar);
}

QString UserInfoWidget::getUserName() const
{
    return currentUserName;
}

QPixmap UserInfoWidget::getUserAvatar() const
{
    return currentAvatar;
}

void UserInfoWidget::showMenu()
{
    // 计算菜单显示位置
    QPoint pos = menuButton->mapToGlobal(menuButton->rect().bottomRight());
    pos.setX(pos.x() - dropdownMenu->width());

    // 显示菜单
    dropdownMenu->exec(pos);
}

void UserInfoWidget::changeUserName()
{
    bool ok;
    QString newName = QInputDialog::getText(this, "修改用户名",
                                            "请输入新的用户名:", QLineEdit::Normal,
                                            currentUserName, &ok);
    if (ok && !newName.isEmpty()) {
        setUserName(newName);
        emit userNameChanged(newName);
    }
}

void UserInfoWidget::changeUserAvatar()
{
    QString fileName = QFileDialog::getOpenFileName(this, "选择头像",
                                                    QString(),
                                                    "图片文件 (*.png *.jpg *.jpeg *.bmp)");
    if (!fileName.isEmpty()) {
        QPixmap newAvatar(fileName);
        if (!newAvatar.isNull()) {
            setUserAvatar(newAvatar);
            emit userAvatarChanged(newAvatar);
        }
    }
}

void UserInfoWidget::logout()
{
    qDebug()<<"logout called.";
    emit logoutRequested();
}
