#include "MainWindow.h"
#include "CryptoUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setWindowTitle("文件加密工具");
    resize(400, 300);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // 密码输入
    QLabel *passwordLabel = new QLabel("输入密码:", this);
    passwordInput = new QLineEdit(this);
    passwordInput->setEchoMode(QLineEdit::Password);

    // 文件选择
    QLabel *fileLabel = new QLabel("选择文件:", this);
    filePathInput = new QLineEdit(this);
    filePathInput->setReadOnly(true);

    QHBoxLayout *fileLayout = new QHBoxLayout();
    fileLayout->addWidget(filePathInput);
    browseButton = new QPushButton("浏览...", this);
    fileLayout->addWidget(browseButton);
    connect(browseButton, &QPushButton::clicked, this, &MainWindow::browseFile);

    // 操作按钮
    encryptButton = new QPushButton("加密文件", this);
    decryptButton = new QPushButton("解密文件", this);
    connect(encryptButton, &QPushButton::clicked, this, &MainWindow::encryptFile);
    connect(decryptButton, &QPushButton::clicked, this, &MainWindow::decryptFile);

    // 添加到主布局
    mainLayout->addWidget(passwordLabel);
    mainLayout->addWidget(passwordInput);
    mainLayout->addWidget(fileLabel);
    mainLayout->addLayout(fileLayout);
    mainLayout->addWidget(encryptButton);
    mainLayout->addWidget(decryptButton);
    mainLayout->addStretch();

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void MainWindow::browseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, "选择文件", "", "所有文件 (*)");
    if (!filePath.isEmpty()) {
        filePathInput->setText(filePath);
    }
}

void MainWindow::encryptFile()
{
    QString password = passwordInput->text();
    QString filePath = filePathInput->text();

    if (password.isEmpty() || filePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入密码并选择文件");
        return;
    }

    QString outputPath = QFileDialog::getSaveFileName(this, "保存加密文件", "", "加密文件 (*.crypt);;所有文件 (*)");
    if (outputPath.isEmpty()) {
        return;
    }

    CryptoUtils crypto;
    if (crypto.encryptFile(filePath, outputPath, password)) {
        QMessageBox::information(this, "成功", "文件加密成功！");
    } else {
        QMessageBox::critical(this, "错误", "文件加密失败");
    }
}

void MainWindow::decryptFile()
{
    QString password = passwordInput->text();
    QString filePath = filePathInput->text();

    if (password.isEmpty() || filePath.isEmpty()) {
        QMessageBox::warning(this, "警告", "请输入密码并选择文件");
        return;
    }

    QString outputPath = QFileDialog::getSaveFileName(this, "保存解密文件", "", "所有文件 (*)");
    if (outputPath.isEmpty()) {
        return;
    }

    CryptoUtils crypto;
    if (crypto.decryptFile(filePath, outputPath, password)) {
        QMessageBox::information(this, "成功", "文件解密成功！");
    } else {
        QMessageBox::critical(this, "错误", "文件解密失败，可能是密码错误或文件已损坏");
    }
}
