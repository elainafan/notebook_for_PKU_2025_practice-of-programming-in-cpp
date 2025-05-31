#include "newdiarylist.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QIcon>
#include <QFont>
#include <QScreen>

NewDiaryList::NewDiaryList(QWidget *parent)
    : QDialog(parent), accepted(false)
{
    setWindowTitle("创建新日记本");
    setMinimumWidth(600); // 原来400 * 1.5

    // 设置更大的字体
    QFont largerFont = font();
    largerFont.setPointSize(largerFont.pointSize() * 1.5);
    setFont(largerFont);

    setupUI();
    setupConnections();
    loadColorIcons();

    // 调整对话框位置居中
    if (parent) {
        setGeometry(
            QStyle::alignedRect(
                Qt::LeftToRight,
                Qt::AlignCenter,
                size(),
                parent->geometry()
                )
            );
    }
}

NewDiaryList::~NewDiaryList()
{
}

void NewDiaryList::setupUI()
{
    // 设置整个对话框的样式
    setStyleSheet(R"(
        QWidget {
            font-family: "Yuanti SC",sans-serif;
            font-size: 20px;
            font-color:#7030A0;
        }
        QDialog {
            background-color: #FFFFFF;
            border-radius: 10px;
        }
        QLabel {
            color: #333333;
            font-weight: 500;
        }
        QLineEdit {
            background-color: #ffffff;
            border: 1px solid #cccccc;
            border-radius: 6px;
            padding: 8px;
            selection-background-color: #7030A0;
        }
        QLineEdit:focus {
            border: 2px solid #7030A0;
        }
        QPushButton {
            background-color: #7030A0;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 10px 20px;
            font-weight: bold;
            font-size: 20px;
        }
        QPushButton:hover {
            background-color: #8040B0;
        }
        QPushButton:pressed {
            background-color: #602090;
        }
        QPushButton:disabled {
            background-color: #cccccc;
            color: #999999;
        }
        QRadioButton {
            color: #333333;
            spacing: 8px;
        }
        QRadioButton::indicator {
            width: 20px;
            height: 20px;
        }
        QRadioButton::indicator:unchecked {
            border: 2px solid #cccccc;
            background-color: white;
            border-radius: 10px;
        }
        QRadioButton::indicator:checked {
            border: 2px solid #7030A0;
            background-color: white;
            border-radius: 10px;
        }
        QRadioButton::indicator:checked:disabled {
            border: 2px solid #cccccc;
            background-color: #eeeeee;
        }
        QRadioButton::indicator::checked {
            image: url(:/images/radiobutton_checked.png);
            background-color: #7030A0;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(30);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // 标题
    QLabel *titleLabel = new QLabel("创建新日记本", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(titleFont.pointSize() * 1.5);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("color: #7030A0; margin-bottom: 10px;");
    mainLayout->addWidget(titleLabel);

    // 日记本名称
    QLabel *nameLabel = new QLabel("日记本名称:", this);
    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("请输入日记本名称");
    nameEdit->setMinimumHeight(45);

    QHBoxLayout *nameLayout = new QHBoxLayout;
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameEdit);
    nameLayout->setSpacing(15);
    mainLayout->addLayout(nameLayout);

    // 日记本类型
    QGroupBox *typeGroupBox = new QGroupBox("日记本类型", this);
    typeGroupBox->setMinimumHeight(120);

    // 放大标题字体
    QFont groupBoxFont = typeGroupBox->font();
    groupBoxFont.setPointSize(groupBoxFont.pointSize() * 1.5);
    groupBoxFont.setBold(true);
    typeGroupBox->setFont(groupBoxFont);

    // 自定义GroupBox样式
    typeGroupBox->setStyleSheet(R"(
        QGroupBox {
            border: 2px solid #7030A0;
            border-radius: 8px;
            margin-top: 30px;
            padding-top: 15px;
            background-color: rgba(112, 48, 160, 0.05);
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 0 10px;
            color: #7030A0;
            background-color: #FFFFFF;
        }
    )");

    QHBoxLayout *typeLayout = new QHBoxLayout(typeGroupBox);
    typeLayout->setContentsMargins(20, 20, 20, 20);
    typeLayout->setSpacing(20);

    dailyRadio = new QRadioButton("每日", this);
    weeklyRadio = new QRadioButton("每周", this);
    monthlyRadio = new QRadioButton("每月", this);
    yearlyRadio = new QRadioButton("每年", this);

    // 放大单选按钮
    QList<QRadioButton*> radios = {dailyRadio, weeklyRadio, monthlyRadio, yearlyRadio};
    for (QRadioButton* radio : radios) {
        radio->setMinimumHeight(30);
        QFont radioFont = radio->font();
        radioFont.setPointSize(radioFont.pointSize() * 1.2);
        radio->setFont(radioFont);
    }

    dailyRadio->setChecked(true);

    typeGroup = new QButtonGroup(this);
    typeGroup->addButton(dailyRadio, 0);
    typeGroup->addButton(weeklyRadio, 1);
    typeGroup->addButton(monthlyRadio, 2);
    typeGroup->addButton(yearlyRadio, 3);

    typeLayout->addWidget(dailyRadio);
    typeLayout->addWidget(weeklyRadio);
    typeLayout->addWidget(monthlyRadio);
    typeLayout->addWidget(yearlyRadio);

    mainLayout->addWidget(typeGroupBox);

    // 日记本颜色
    QLabel *colorLabel = new QLabel("日记本颜色:", this);
    colorCombo = new QComboBox(this);
    colorCombo->setIconSize(QSize(48, 48));
    colorCombo->setFixedSize(120, 75);

    colorCombo->setStyleSheet(R"(
    QComboBox {
        min-height: 75px;
        max-height: 75px;
        line-height: 75px;
        padding-left: 25px;
        border: 2px solid #7030A0;
        border-radius: 8px;
    }
    QComboBox::drop-down {
        width: 0px; /* 设置为0宽度，完全隐藏下拉按钮区域 */
    }
    QComboBox::down-arrow {
        width: 0px; /* 隐藏下拉箭头 */
        height: 0px;
        image: none; /* 清除箭头图像 */
    }
    QComboBox QAbstractItemView {
        border: 2px solid #7030A0;
        border-radius: 8px;
        background-color: white;
        selection-background-color: #e0d0f0;
    }
    )");
    // 美化下拉框
    /*
    colorCombo->setStyleSheet(R"(
        QComboBox {
            padding-left: 25px;
            border: 2px solid #7030A0;
            border-radius: 8px;
            background-color: white;
        }
        QComboBox:hover {
            border-color: #8040B0;
        }
        QComboBox::drop-down {
            subcontrol-origin: padding;
            subcontrol-position: right;
            width: 25px;
            border-left: none;
            border-top-right-radius: 8px;
            border-bottom-right-radius: 8px;
        }
        QComboBox::down-arrow {
            image: url(:/images/dropdown_arrow.png);
            width: 16px;
            height: 16px;
        }
        QComboBox QAbstractItemView {
            border: 2px solid #7030A0;
            border-radius: 8px;
            background-color: white;
            selection-background-color: #e0d0f0;
            min-width: 120px;
            padding: 8px;
        }
    )");*/

    QHBoxLayout *colorLayout = new QHBoxLayout;
    colorLayout->addWidget(colorLabel);
    colorLayout->addWidget(colorCombo);
    colorLayout->setSpacing(15);
    mainLayout->addLayout(colorLayout);

    // 确认/取消按钮
    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    // 放大并美化按钮
    QList<QAbstractButton*> buttons = buttonBox->buttons();
    for (QAbstractButton* button : buttons) {
        button->setMinimumHeight(45);
        button->setMinimumWidth(120);

        if (button->text().contains("OK")) {
            button->setText("创建");
        } else if (button->text().contains("Cancel")) {
            button->setText("取消");
            // 取消按钮使用不同样式
            button->setStyleSheet(R"(
                QPushButton {
                    font-size:20px;
                    background-color: #f0f0f0;
                    color: #333333;
                    border: 1px solid #cccccc;
                }
                QPushButton:hover {
                    background-color: #e0e0e0;
                }
                QPushButton:pressed {
                    background-color: #d0d0d0;
                }
            )");
        }
    }

    mainLayout->addWidget(buttonBox, 0, Qt::AlignCenter);

    // 添加一个底部装饰条
    QFrame* decorLine = new QFrame(this);
    decorLine->setFrameShape(QFrame::HLine);
    decorLine->setFrameShadow(QFrame::Sunken);
    decorLine->setStyleSheet("background-color: #7030A0; min-height: 2px;");
    mainLayout->addWidget(decorLine);

    setLayout(mainLayout);
}

void NewDiaryList::setupConnections()
{
    // 连接逻辑保持不变
    connect(buttonBox, &QDialogButtonBox::accepted, this, [this]() {
        if (nameEdit->text().isEmpty()) {
            // 可以添加错误提示
            return;
        }

        diaryName = nameEdit->text();

        // 获取选中的日记本类型
        int typeId = typeGroup->checkedId();
        switch (typeId) {
        case 0: diaryType = "daily"; break;
        case 1: diaryType = "weekly"; break;
        case 2: diaryType = "monthly"; break;
        case 3: diaryType = "yearly"; break;
        default: diaryType = "daily"; break;
        }

        colorIndex = colorCombo->currentIndex()+1;

        DiaryList newList = DiaryList(diaryName, diaryType, colorIndex);
        emit diaryListCreated(newList); // 发出信号

        accepted = true;
        accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, this, [this]() {
        accepted = false;
        reject();
    });
}

void NewDiaryList::loadColorIcons()
{
    // 加载所有颜色图标
    for (int i = 1; i < NUM_COLORS; ++i) {
        QString iconPath = QString(":/images/diaryListIcon%1.png").arg(i);
        QPixmap pixmap(iconPath);

        if (!pixmap.isNull()) {
            colorCombo->addItem(QIcon(pixmap),"");
        } else {
            // 如果图标加载失败，添加一个占位符
            QPixmap emptyPixmap(48, 48); // 原来的尺寸 * 1.5
            emptyPixmap.fill(Qt::transparent);
            colorCombo->addItem(QIcon(emptyPixmap),"");
        }
    }
}

DiaryList NewDiaryList::getDiaryList() const
{
    return DiaryList(diaryName, diaryType, colorIndex);
}

bool NewDiaryList::isAccepted() const
{
    return accepted;
}
