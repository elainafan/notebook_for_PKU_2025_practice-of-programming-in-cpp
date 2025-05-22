#include "FileOperation.h"
#include "CryptoUtils.h"
#include <QDateTime>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDebug>

QString FileOperation::startPath=QDir::currentPath();  //在这里修改根目录（我认为根目录应当与类而非对象绑定）

FileOperation::FileOperation(QString username_, QObject *parent)  //建议使用统一的初始化方法e.g.FileOperation f{};
    : QObject(parent), username(username_)
{
}

bool FileOperation::findUser(QString user, QString password){
    QDir dir(startPath);
    username=user;
    if (dir.exists(user)){
        QString inputPath = dir.filePath(QDir(user).filePath("valid.crypt"));  //路径
        QString outputPath = dir.filePath(QDir(user).filePath("valid1"));
        inputPath = QDir::toNativeSeparators(inputPath);  // 转换为本地分隔符，应当可以跨平台
        outputPath = QDir::toNativeSeparators(outputPath);
        if (CryptoUtils().decryptFile(inputPath,outputPath,password)){
            deleteFile("valid1.md");
            return 1;
        }  //没有删除加密文件
        return 0;

    } else {
        if(dir.mkdir(user)) {
            QString inputPath = dir.filePath(QDir(user).filePath("valid.md"));  //路径
            QString outputPath = dir.filePath(QDir(user).filePath("valid.crypt"));
            inputPath = QDir::toNativeSeparators(inputPath);  // 转换为本地分隔符，应当可以跨平台
            outputPath = QDir::toNativeSeparators(outputPath);
            QFile file(inputPath);  //创建markdown文件
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << "_VALID_";
                file.close();
            } else {
                qDebug() << "无法创建文件:" << file.errorString() << Qt::endl;
            }
            CryptoUtils().encryptFile(inputPath,outputPath,password);  //没有删除未加密文件
            qDebug() << "目录创建成功\n";
            return 1;
        } else {
            qWarning() << "目录创建失败\n";
            return 0;
        }
    }
}

QString FileOperation::findFile(const QString& fileName) {
    QDir dir(QDir(startPath).filePath(username));
    if (dir.exists(fileName)) {
        return dir.absoluteFilePath(fileName);
    }
    return QString();
}

void FileOperation::newFile(const QString& content){  //这里是QString而不是markdown文件。如有需要，后再更改
    QDateTime current = QDateTime::currentDateTime();
    QString current_formatted = current.toString("yyyy_MM_dd_HH_mm_ss");
    QDir dir(startPath);
    if(!dir.exists(username)) {
        // 创建目录（单层）
        if(dir.mkdir(username)) {
            qDebug() << "目录创建成功\n";
        } else {
            qWarning() << "目录创建失败\n";
        }
    }
    // 拼接完整文件路径
    QString fullPath = dir.filePath(QDir(username).filePath(current_formatted + ".md"));  //路径
    fullPath = QDir::toNativeSeparators(fullPath);  // 转换为本地分隔符，应当可以跨平台
    QFile file(fullPath);  //创建markdown文件

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << content;
        file.close();
    } else {
        qDebug() << "无法创建文件:" << file.errorString() << Qt::endl;
    }
}

QString FileOperation::readFile(const QString& filename){  //这里是QString而不是markdown文件。如有需要，后再更改
    QString filePath = findFile(filename);    //默认当前路径，不过findFile已经支持在路径下搜索
    if (filePath.isEmpty()) {
        qDebug() << "文件未找到:" << filename;
        return QString();
    }
    QFile file(filePath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString content = in.readAll();
        file.close();
        return content;
    } else {
        qDebug() << "无法读取文件:" << file.errorString() << Qt::endl;
        return QString();
    }
}

void FileOperation::storeFile(const QString& filename, const QString& content){  //这里是QString而不是markdown文件。如有需要，后再更改
    QString filePath = findFile(filename);    //默认当前路径，不过findFile已经支持在路径下搜索
    if (filePath.isEmpty()) {
        qDebug() << "文件未找到:" << filename;
        return;
    }
    QFile file(filePath);

    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {  //这里是覆写
        QTextStream out(&file);
        out << content;
        file.close();
    } else {
        qDebug() << "无法写入文件:" << file.errorString() << Qt::endl;
    }
}

bool FileOperation::deleteFile(const QString& filename){  //文件不会被放入回收站，请谨慎删除。可以考虑在前端弹窗提示
    QString filePath = findFile(filename);
    if (filePath.isEmpty()) {
        qDebug() << "文件未找到:" << filename;
        return false;
    }
    QFileInfo fileInfo(filePath);

    // 检查文件是否存在且确实是文件（不是目录）
    if (!fileInfo.exists() || !fileInfo.isFile()) {
        qDebug() << "不是有效的文件或文件不存在:" << filePath;
        return false;
    }
    return QFile::remove(filePath);
}
