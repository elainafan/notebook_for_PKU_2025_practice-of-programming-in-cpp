#include "FileOperation.h"
#include "CryptoUtils.h"
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QDebug>

QString FileOperation::startPath=QDir::currentPath();  //在这里修改根目录（我认为根目录应当与类而非对象绑定）

FileOperation::FileOperation(QString username_, QObject *parent)  //建议使用统一的初始化方法e.g.FileOperation f{};
    : QObject(parent), username(username_)
{
}

bool FileOperation::findUser(QString user, QString password_){
    QDir dir(startPath);
    username=user;
    password=password_;
    if (dir.exists(user)){
        QString inputPath = dir.filePath(QDir(user).filePath("valid.crypt"));  //路径
        QString outputPath = dir.filePath(QDir(user).filePath("valid1"));
        inputPath = QDir::toNativeSeparators(inputPath);  // 转换为本地分隔符，应当可以跨平台
        outputPath = QDir::toNativeSeparators(outputPath);
        if (CryptoUtils().decryptFile(inputPath,outputPath,password_)){
            deleteFile(dir.filePath(QDir(user).filePath("valid1.md")));
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
            CryptoUtils().encryptFile(inputPath,outputPath,password_);  //没有删除未加密文件
            QString userFullPath = dir.filePath(user); // 用户目录的绝对路径
            QDir userDir(userFullPath); // 初始化为用户目录

            // 创建diary和picture目录
            if (!userDir.mkdir("diary") || !userDir.mkdir("picture")) {
                qWarning() << "无法创建diary或picture目录";
                return 0;
            }

            // 创建diary下的子目录
            QString diaryPath = userDir.filePath("diary");
            QDir diaryDir(diaryPath);
            if (!diaryDir.mkdir("daily") || !diaryDir.mkdir("weekly") ||
                !diaryDir.mkdir("monthly") || !diaryDir.mkdir("yearly")) {
                qWarning() << "无法创建diary下的子目录";
                return 0;
            }
            qDebug() << "目录创建成功\n";
            return 1;
        } else {
            qWarning() << "目录创建失败\n";
            return 0;
        }
    }
}

QStringList FileOperation::findFile(const QString& fileName, const QString& diaryType) {

    QString dir = QDir(QDir(startPath).filePath(username)).filePath("diary");
    dir = QDir(dir).filePath(diaryType);
    QStringList resultFiles;
    QStringList nameFilters;
    nameFilters << fileName;

    // 创建递归迭代器
    QDirIterator it(
        dir,
        nameFilters,                     // 文件名过滤条件
        QDir::Files,                     // 只查找文件（忽略目录）
        QDirIterator::Subdirectories     // 递归搜索子目录
        );

    // 遍历所有匹配文件
    while (it.hasNext()) {
        QString filePath = it.next();
        resultFiles.append(filePath);
    }

    return resultFiles;
}

QStringList FileOperation::findFilebyTime(int year, int month, int day, QString diaryType) {  //diaryType包括daily,weekly,monthly,yearly,以及自定义
    if (diaryType==""){
        if(month==0)diaryType="yearly";
        else{
            if(day==0)diaryType="monthly";
            else diaryType="daily";
        }
    }    //在缺省diaryType时根据时间值缺省情况推导diaryType
    QString yyyy=QString("%1").arg(year, 4, 10, QLatin1Char('0'));
    QString MM=QString("%1").arg(month, 2, 10, QLatin1Char('0'));
    QString dd=QString("%1").arg(day, 2, 10, QLatin1Char('0'));
    if (diaryType=="daily")return findFile(yyyy+"_"+MM+"_"+dd+"*.md", diaryType);
    if (diaryType=="weekly")return findFile(yyyy+"_"+MM+"_"+dd+"*.md", diaryType);
    if (diaryType=="monthly")return findFile(yyyy+"_"+MM+"*.md", diaryType);
    if (diaryType=="yearly")return findFile(yyyy+"*.md", diaryType);
    return findFile(yyyy+"_"+MM+"_"+dd+"*.md", diaryType);
}

bool FileOperation::newFolder(QString folderName){
    QDir diaryDir = QDir(QDir(QDir(startPath).filePath(username)).filePath("diary"));
    // 创建diary下的子目录
    if (!diaryDir.mkdir(folderName)) {
        qWarning() << "无法创建diary下的子目录";
        return 0;
    }
    return 1;
}

bool FileOperation::deleteFile(const QString& filePath){  //文件不会被放入回收站，请谨慎删除。可以考虑在前端弹窗提示
    if (filePath.isEmpty()) {
        qDebug() << "文件未找到:" << filePath;
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

bool FileOperation::encryptDiary(){
    QString dir = QDir(QDir(startPath).filePath(username)).filePath("diary");
    QStringList resultFiles;
    QStringList nameFilters;
    nameFilters << "*.md";

    // 创建递归迭代器
    QDirIterator it(
        dir,
        nameFilters,                     // 文件名过滤条件
        QDir::Files,                     // 只查找文件（忽略目录）
        QDirIterator::Subdirectories     // 递归搜索子目录
        );

    // 遍历所有匹配文件
    while (it.hasNext()) {
        QString filePath = it.next();
        if(!CryptoUtils().encryptFile(filePath,filePath+".crypt",password))return 0;
        deleteFile(filePath);
    }
    return 1;
}

bool FileOperation::decryptDiary(){
    QString dir = QDir(QDir(startPath).filePath(username)).filePath("diary");
    QStringList resultFiles;
    QStringList nameFilters;
    nameFilters << "*.crypt";

    // 创建递归迭代器
    QDirIterator it(
        dir,
        nameFilters,                     // 文件名过滤条件
        QDir::Files,                     // 只查找文件（忽略目录）
        QDirIterator::Subdirectories     // 递归搜索子目录
        );

    // 遍历所有匹配文件
    while (it.hasNext()) {
        QString filePath = it.next();

        QFileInfo fileInfo(filePath);
        QString dirPath = fileInfo.path();          // 目录
        QString baseName = fileInfo.baseName();  // 无后缀文件名
        QString outputPath = QDir::cleanPath(dirPath + "/" + baseName);
        outputPath = QDir::toNativeSeparators(outputPath);  // 转换为本地分隔符，应当可以跨平台

        if(!CryptoUtils().decryptFile(filePath,outputPath,password))return 0;
        deleteFile(filePath);
    }
    return 1;
}
