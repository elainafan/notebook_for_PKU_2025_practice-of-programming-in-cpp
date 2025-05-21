#include "FileOperation.h"
#include <QDateTime>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QDebug>

FileOperation::FileOperation(QObject *parent)
    : QObject(parent)
{
}

QString FileOperation::findFile(const QString& fileName, const QString& startPath=QDir::currentPath()) {
    QDir dir(startPath);
    if (dir.exists(fileName)) {
        return dir.absoluteFilePath(fileName);
    }
    return QString();
}

void FileOperation::newFile(const QString& content){  //这里是QString而不是markdown文件。如有需要，后再更改
    QDateTime current = QDateTime::currentDateTime();
    QString current_formatted = current.toString("yyyy_MM_dd_HH_mm_ss");
    QFile file(current_formatted+".md");  //创建markdown文件

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
