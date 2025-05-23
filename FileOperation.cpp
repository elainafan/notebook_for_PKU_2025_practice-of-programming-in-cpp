#include "FileOperation.h"
#include "CryptoUtils.h"
#include <QDateTime>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QVector>
#include <QDebug>

QString FileOperation::startPath=QDir::currentPath();  //在这里修改根目录（我认为根目录应当与类而非对象绑定）

FileOperation::FileOperation(QString username_, QString password_, QObject *parent)  //建议使用统一的初始化方法e.g.FileOperation f{};
    : QObject(parent), username(username_), password(password_)
{
}

int FileOperation::findUser(QString user, QString password_){
    QDir dir(startPath);
    username=user;
    password=password_;
    if (dir.exists(user)){
        QString inputPath = dir.filePath(QDir(user).filePath("valid.crypt"));  //路径
        QString outputPath = dir.filePath(QDir(user).filePath("valid"));
        inputPath = QDir::toNativeSeparators(inputPath);  // 转换为本地分隔符，应当可以跨平台
        outputPath = QDir::toNativeSeparators(outputPath);
        if (CryptoUtils().decryptFile(inputPath,outputPath,password_,true)){
            return 1;
        }  //没有删除加密文件
        return -1;

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
                return -1;
            }

            // 创建diary下的子目录
            QString diaryPath = userDir.filePath("diary");
            QDir diaryDir(diaryPath);
            if (!diaryDir.mkdir("daily") || !diaryDir.mkdir("weekly") ||
                !diaryDir.mkdir("monthly") || !diaryDir.mkdir("yearly")) {
                qWarning() << "无法创建diary下的子目录";
                return -1;
            }
            qDebug() << "目录创建成功\n";
            return 0;
        } else {
            qWarning() << "目录创建失败\n";
            return -1;
        }
    }
}

void FileOperation::signOut(){  //退出登录，并加密所有未加密的日记
    deleteFile(QDir(username).filePath("valid.md"));
    encryptDir();
}

QStringList FileOperation::findFile(const QString& fileName, const QString& diaryType) {

    QString dir = QDir(username).filePath("diary");
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

QStringList FileOperation::findFileByTime(int year, int month, int day, QString diaryType) {  //diaryType包括daily,weekly,monthly,yearly,以及自定义
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
    QDir diaryDir = QDir(username).filePath("diary");
    // 创建diary下的子目录
    if (!diaryDir.mkdir(folderName)) {
        qWarning() << "无法创建diary下的子目录";
        return 0;
    }
    return 1;
}

QPair<QString,QVector<int> > FileOperation::findFileByContent(const QString& target, QString diaryType){
    //每次返回一个搜到的文件（如有），以尽可能实时输出搜索结果；返回值0:文件的相对路径;1:词在该文件中的位置；diaryType包括daily,weekly,monthly,yearly,以及自定义

    QString dir = QDir(username).filePath("diary");
    if (diaryType!="")dir = QDir(dir).filePath(diaryType);
    QPair<QString,QVector<int> > resultFiles;

    if(unsearchedFiles.isEmpty()||target!=searchword||diaryType!=searchDiaryType){
        QStringList nameFilters;
        nameFilters << "*.md";

        // 创建递归迭代器
        QDirIterator it(
            dir,
            nameFilters,                     // 文件名过滤条件
            QDir::Files,                     // 只查找文件（忽略目录）
            QDirIterator::Subdirectories     // 递归搜索子目录
            );

        // 将所有匹配文件压入栈中
        while (it.hasNext()) {
            QString filePath = it.next();
            unsearchedFiles.push(filePath);
        }
    }
    searchword = target;
    searchDiaryType = diaryType;

    // 遍历所有匹配文件
    while (!unsearchedFiles.isEmpty()) {
        QString filePath = unsearchedFiles.pop();
        QVector<int> res = searchInFileByContent(filePath, target);
        if(!res.empty()){
            resultFiles.first = filePath;
            resultFiles.second = res;
            return resultFiles;
        }
    }

    return resultFiles;
}

QVector<int> FileOperation::searchInFileByContent(const QString& filePath, const QString& target) {
    //返回值vector为全文位置，从0开始；搜索的是未渲染的markdown；不支持词中换行
    QVector<int> matchPositions;
    if (target.isEmpty()) {
        qWarning() << "错误：目标字符串不能为空。";
        return matchPositions;
    }
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件：" << filePath;
        return matchPositions;
    }

    QTextStream in(&file);
    int lineNumber = 0;
    qint64 globalPosition = 0; // 全局字符位置（按字符计数）

    while (!in.atEnd()) {
        QString line = in.readLine();
        lineNumber++;
        int pos = 0;

        // 逐字符搜索匹配项
        while (line.indexOf(target, pos) != -1) {  //不支持词中换行
            pos = line.indexOf(target, pos);
            matchPositions.append(globalPosition + pos);
            qDebug() << "匹配项位置 - 行号: " << lineNumber << ", 行内位置: " << pos << ", 全局位置: " << (globalPosition + pos);
            pos += target.length(); // 跳过已匹配的部分
        }

        globalPosition += line.length() + 1; // 更新全局位置（+1 为换行符）
    }

    file.close();
    return matchPositions;
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

bool FileOperation::encryptDir(QString dir){
    if(dir=="")dir = QDir(username).filePath("diary");
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

bool FileOperation::decryptDir(QString dir){
    if(dir=="")dir = QDir(username).filePath("diary");
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
