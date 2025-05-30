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
#include <QRandomGenerator>
#include <QDebug>

QString FileOperation::startPath=QDir::currentPath();  //在这里修改根目录（我认为根目录应当与类而非对象绑定）

FileOperation::FileOperation(QString username_, QObject *parent)  //建议使用统一的初始化方法e.g.FileOperation f{};
    : QObject(parent), username(username_)
{
    searchword="";searchDiaryType="";
}

int FileOperation::signIn(QString user, QString password_){
    QDir dir(startPath);

    if(user==""){
        if(dir.exists("username.md")){
            QFile userFile(QDir(dir).filePath("username.md"));
            userFile.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&userFile);
            user=in.readAll();
            userFile.close();
        }
        else{
            return -1;
        }
    }

    username=user;
    if (dir.exists(user)){
        QString inputPath = dir.filePath(QDir(user).filePath("valid.crypt"));  //路径
        QString outputPath = dir.filePath(QDir(user).filePath("valid"));
        inputPath = QDir::toNativeSeparators(inputPath);  // 转换为本地分隔符，应当可以跨平台
        outputPath = QDir::toNativeSeparators(outputPath);

        QString userPath = dir.filePath("username.md");
        QFile userFile(userPath);  //创建markdown文件
        if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&userFile);
            out << user;
            userFile.close();
        } else {
            qDebug() << "无法创建文件:" << userFile.errorString() << Qt::endl;
        }

        if (QDir(QDir(dir).filePath(user)).exists("valid.md")){
            QFile valid(QDir(QDir(dir).filePath(user)).filePath("valid.md"));
            valid.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&valid);
            password_=in.readAll();
            valid.close();
        }
        if (CryptoUtils().decryptFile(inputPath,outputPath,password_,true)){
            password=password_;
            decryptAll();
            return 1;
        }  //没有删除加密文件
        return -1;

    } else {
        if(dir.mkdir(user)) {
            QString inputPath = dir.filePath(QDir(user).filePath("valid.md"));  //路径
            QString outputPath = dir.filePath(QDir(user).filePath("valid.crypt"));
            inputPath = QDir::toNativeSeparators(inputPath);  // 转换为本地分隔符，应当可以跨平台
            outputPath = QDir::toNativeSeparators(outputPath);

            QString userPath = dir.filePath("username.md");
            QFile userFile(userPath);  //创建markdown文件
            if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&userFile);
                out << user;
                userFile.close();
            } else {
                qDebug() << "无法创建文件:" << userFile.errorString() << Qt::endl;
            }

            QFile file(inputPath);  //创建markdown文件
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << password_;
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
            if (!newFolder(DiaryList("日记","daily",1)) || !newFolder(DiaryList("周记","weekly",2)) ||
                !newFolder(DiaryList("月记","monthly",3)) || !newFolder(DiaryList("年记","yearly",4))) {
                qWarning() << "无法创建diary下的子目录";
                return -1;
            }
            qDebug() << "目录创建成功\n";
            password=password_;
            return 0;
        } else {
            qWarning() << "目录创建失败\n";
            return -1;
        }
    }
}

void FileOperation::signOut(){  //退出登录，并加密所有未加密的日记
    deleteFile(QDir(username).filePath("valid.md"));
    deleteFile("username.md");
    encryptAll();
}

void FileOperation::setProfilePicture(const QPixmap& pic){
    QDir dir(username);
    pic.save(dir.filePath("profilePicture.png"));
}

QString FileOperation::getProfilePicture(){
    QString dir(QDir(username).filePath("profilePicture.png"));
    return dir;
}

void FileOperation::changeUsername(QString newUsername){
    QDir(startPath).rename(username,newUsername);
    username = newUsername;
}

bool FileOperation::changePassword(QString newPassword){
    QDir dir(username);
    QFile file(dir.filePath("valid.md"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << newPassword;
        file.close();
    } else {
        qDebug() << "无法修改密码:" << file.errorString() << Qt::endl;
        return 0;
    }

    decryptDir();

    deleteFile(dir.filePath("valid.crypt"));
    QString inputPath = dir.filePath("valid.md");  //路径
    QString outputPath = dir.filePath("valid.crypt");
    inputPath = QDir::toNativeSeparators(inputPath);  // 转换为本地分隔符，应当可以跨平台
    outputPath = QDir::toNativeSeparators(outputPath);
    CryptoUtils().encryptFile(inputPath,outputPath,newPassword);

    password = newPassword;
    return 1;
}

void FileOperation::setStar(const QString& fileName){
    // 构造文件的绝对路径
    QDir dir(username);
    QFile file(dir.filePath("starred.md"));
    QString rootPath(QDir(username).filePath("diary"));
    QDir diaryDir(QDir(username).filePath("diary"));
    QString filePath;

    // 创建递归迭代器
    QDirIterator it(rootPath,
                    QStringList() << fileName, // 要搜索的文件名
                    QDir::Files,               // 只查找文件
                    QDirIterator::Subdirectories); // 递归子目录

    while (it.hasNext()) {
        it.next();
        filePath = it.filePath();
        break;
    }

    // 确保文件存在
    if (QFile::exists(filePath)) {
        QString basePath("/"+rootPath);
        QDir::toNativeSeparators(basePath);
        QDir baseDir(basePath);

        // 生成相对于basePath的路径
        QString relativePath = baseDir.relativeFilePath("/"+filePath);
        QDir::toNativeSeparators(relativePath);
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            QTextStream in(&file);
            QStringList entries;
            bool found = false;

            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (!line.isEmpty()) {
                    if (line == relativePath) {
                        found = true;
                    } else {
                        entries.append(line);
                    }
                }
            }

            // 根据是否找到匹配项决定操作
            if (found) {
                // 取消收藏：移除匹配项
                qDebug() << "已取消收藏文件:" << relativePath;
            } else {
                // 添加收藏
                entries.append(relativePath);
                qDebug() << "已收藏文件:" << relativePath;
            }

            // 清空文件并重新写入
            file.resize(0);
            file.seek(0);

            QTextStream out(&file);
            for (const QString &entry : entries) {
                out << entry << "\n";
            }

            file.close();
        } else {
            qDebug() << "无法打开文件:" << file.errorString();
        }
    }
}

QString FileOperation::recommend(){
    QString dir = QDir(username).filePath("diary");

    // 获取所有文件（排除目录和特殊条目）
    QStringList files;
    QStringList nameFilters;
    QStringList starredFiles;
    QString selectedFile;
    nameFilters << "2*.md";

    // 先读取收藏文件（总是读取，但50%概率使用）
    QFile starFile(QDir(username).filePath("starred.md"));
    if (starFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&starFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                starredFiles.append(QDir(dir).filePath(line));
            }
        }
        starFile.close();
    }

    // 然后50%概率优先使用收藏文件
    if (!starredFiles.isEmpty() && QRandomGenerator::global()->bounded(2) == 0) {
        // 过滤掉不存在的文件
        QStringList validFiles;
        for (const QString &path : starredFiles) {
            if (QFile::exists(path)) {
                validFiles.append(path);
            }
        }

        if (!validFiles.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(validFiles.size());
            selectedFile = validFiles.at(randomIndex);
        }
    }

    // 在未选择收藏文件时扫描目录
    if (selectedFile.isEmpty()) {
        QDirIterator it(
            dir,
            QStringList() << "2*.md",  // 匹配所有日记
            QDir::Files,
            QDirIterator::Subdirectories
            );

        while (it.hasNext()) {
            files.append(it.next());
        }

        if (files.isEmpty()) {
            qDebug() << "目录中没有文件:" << dir;
            return QString();
        }

        int randomIndex = QRandomGenerator::global()->bounded(files.size());
        selectedFile = files.at(randomIndex);
    }
    // 返回相对路径
    return selectedFile;
}

QStringList FileOperation::findFile(QDateTime start, QDateTime end, const QString& dirPath) {

    QDir dir(dirPath);
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    QStringList resultFiles;

    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            QStringList found = findFile(start, end, entry.filePath());
            if (!found.isEmpty())resultFiles.append(found);
        } else {
            QString baseName = entry.baseName(); // 获取不带扩展名的文件名
            if(baseName!="folderInfo"){
                QDateTime fileTime = QDateTime::fromString(baseName, "yyyy_MM_dd_HH_mm_ss");
                if (fileTime >= start && fileTime <= end) {
                    resultFiles.append(entry.filePath());
                }
            }
        }
    }
    return resultFiles;
}

QStringList FileOperation::findFileByTime(QDateTime start, QDateTime end, const DiaryList& diaryType) {
    //起始日期 , 终止日期的后一天

    QDateTime date2 = end.addSecs(-1);

    QString dirPath = QDir(username).filePath("diary");
    if (diaryType.getName()!="")dirPath = QDir(dirPath).filePath(diaryType.getType()+"_"+diaryType.getName());

    return findFile(start, date2, dirPath);
}

bool FileOperation::newFolder(const DiaryList& diaryType){
    QDir diaryDir = QDir(username).filePath("diary");
    // 创建diary下的子目录
    if (!diaryDir.mkdir(diaryType.getType()+"_"+diaryType.getName())) {
        qWarning() << "无法创建diary下的子目录";
        return 0;
    }
    QDir notebookDir = diaryDir.filePath(diaryType.getType()+"_"+diaryType.getName());
    QFile file(notebookDir.filePath("folderInfo.md"));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "无法打开文件：" << "folderInfo.md";
    }
    QTextStream out(&file);
    out << diaryType.getColourType();
    file.close();
    return 1;
}

QVector<DiaryList> FileOperation::allFolders(){
    QVector<DiaryList> folders;
    QDir dir = QDir(username).filePath("diary");
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable);
    for (const QFileInfo &entry : entries) {
        QString folderName=entry.fileName();
        int first = folderName.lastIndexOf ("_"); //从后面查找"_"位置
        QString name = folderName.right(folderName.length ()-first-1); //从右边截取
        QString type = folderName.left(first); //从左边截取
        QString filename=QDir(dir.filePath(folderName)).filePath("folderInfo.md");
        filename = QDir::toNativeSeparators(filename);
        QFile file(filename);

        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "无法打开文件：" << "folderInfo.md";
        }
        QTextStream in(&file);
        QString colour = in.readAll();
        file.close();
        folders.append(DiaryList(name,type,colour.toInt()));
    }
    return folders;
}

QPair<QString,QVector<int> > FileOperation::findFileByContent(const QString& target, bool newSearch, const DiaryList& diaryType){
    //每次返回一个搜到的文件（如有），以尽可能实时输出搜索结果；返回值0:文件的相对路径;1:词在该文件中的位置

    QString dir = QDir(username).filePath("diary");
    if (diaryType.getName()!="")dir = QDir(dir).filePath(diaryType.getType()+"_"+diaryType.getName());
    QPair<QString,QVector<int> > resultFiles;

    if(target!=searchword||diaryType.getName()!=searchDiaryType||newSearch){
        QStringList nameFilters;
        nameFilters << "*.md";

        while (!unsearchedFiles.isEmpty()){
            unsearchedFiles.pop();
        }

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
    searchDiaryType = diaryType.getName();

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
    nameFilters << "*.md" << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp";

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

void FileOperation::encryptAll(){
    encryptDir();
    encryptDir(QDir(username).filePath("picture"));
}

void FileOperation::decryptAll(){
    decryptDir();
    decryptDir(QDir(username).filePath("picture"));
}

void FileOperation::setReminder(const reminder& r){
    QDir dir(username);
    QFile file(dir.filePath("reminder.md"));
    QString time = r.time.toString("yyyy_MM_dd");
    QString task = r.task;

    if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        QTextStream in(&file);
        QStringList reminders;
        bool found = false;

        while (!in.atEnd()) {
            QString lineTime = in.readLine().trimmed();
            QString lineTask;
            if (!in.atEnd())lineTask = in.readLine().trimmed();
            if (!lineTime.isEmpty() && !lineTask.isEmpty()) {
                if (lineTime == time && lineTask == task) {
                    found = true;
                } else {
                    reminders.append(lineTime);
                    reminders.append(lineTask);
                }
            }
        }

        // 根据是否找到匹配项决定操作
        if (found) {
            qDebug() << "已取消提醒事项:" << time << ":" << task << "\n";
        } else {
            reminders.append(time);
            reminders.append(task);
            qDebug() << "已添加提醒事项:" << time << ":" << task << "\n";
        }

        // 清空文件并重新写入
        file.resize(0);
        file.seek(0);

        QTextStream out(&file);
        for (const QString &entry : reminders) {
            out << entry << "\n";
        }

        file.close();
    } else {
        qDebug() << "无法打开文件:" << file.errorString();
    }
}

QVector<reminder> FileOperation::getReminder(){
    QDir dir(username);
    QFile file(dir.filePath("reminder.md"));
    QVector<reminder> reminders;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);

        while (!in.atEnd()) {
            QString lineTime = in.readLine().trimmed();
            QString lineTask;
            if (!in.atEnd())lineTask = in.readLine().trimmed();
            if (!lineTime.isEmpty() && !lineTask.isEmpty()) {
                reminders.append(reminder(QDateTime::fromString(lineTime, "yyyy_MM_dd"),lineTask));
            }
        }

        file.close();
    } else {
        qDebug() << "无法打开文件:" << file.errorString();
    }
    return reminders;
}
