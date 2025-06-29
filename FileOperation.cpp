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

QString FileOperation::startPath=QDir(QDir::currentPath()).filePath("debug");  //在这里修改根目录（我认为根目录应当与类而非对象绑定）

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
        qDebug()<<user<<dir;
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
    deleteFile(QDir(QDir(startPath).filePath(username)).filePath("valid.md"));
    deleteFile(QDir(startPath).filePath("username.md"));
    encryptAll();
}

void FileOperation::setProfilePicture(const QPixmap& pic){
    QDir dir(QDir(startPath).filePath(username));
    pic.save(dir.filePath("profilePicture.png"));
}

QPixmap FileOperation::getProfilePicture(){
    return QPixmap(QDir(QDir(startPath).filePath(username)).filePath("profilePicture.png"));
}

bool FileOperation::changeUsername(QString newUsername){
    if (QFileInfo(QDir(startPath).filePath(newUsername)).isDir()){
        return false;
    }
    QDir(startPath).rename(username,newUsername);
    username = newUsername;

    QFile userFile(QDir(startPath).filePath("username.md"));  //创建markdown文件
    if (userFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&userFile);
        out << newUsername;
        userFile.close();
    } else {
        qDebug() << "无法创建文件:" << userFile.errorString() << Qt::endl;
        return false;
    }
    return true;
}

bool FileOperation::changePassword(QString newPassword){
    QDir dir(QDir(startPath).filePath(username));
    QFile file(dir.filePath("valid.md"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << newPassword;
        file.close();
    } else {
        qDebug() << "无法修改密码:" << file.errorString() << Qt::endl;
        return 0;
    }

    decryptAll();

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
    QDir dir(QDir(startPath).filePath(username));
    QFile file(dir.filePath("starred.md"));
    QString rootPath(dir.filePath("diary"));
    QDir diaryDir(dir.filePath("diary"));
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

QVector<Diary> FileOperation::allStarred(){
    QDir dir(QDir(startPath).filePath(username));
    QVector<Diary> starredFiles;
    QFile starFile(dir.filePath("starred.md"));
    if (starFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&starFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                starredFiles.append(fileToDiary(QDir(dir.filePath("diary")).filePath(line)));
            }
        }
        starFile.close();
    }
    return starredFiles;
}

Diary FileOperation::recommend(){
    QString dir = QDir(startPath).filePath(QDir(username).filePath("diary"));
    QDir parentDir(QDir(startPath).filePath(QDir(username).filePath("picture")));

    // 获取所有文件（排除目录和特殊条目）
    QStringList files;
    QStringList nameFilters;
    QStringList starredFiles;
    QString selectedFile;
    QStringList pictureFiles;
    nameFilters << "2*.md";

    // 设置过滤器，只获取文件夹
    parentDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    // 遍历所有直接子文件夹
    for (const QString &childDir : parentDir.entryList()) {
        QString childPath = parentDir.filePath(childDir);

        // 获取当前子文件夹的子文件夹
        QDir grandchildDir(childPath);
        grandchildDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);

        for (const QString &grandchild : grandchildDir.entryList()) {
            // 添加孙文件夹的名称到结果列表
            if (!QDir(QDir(parentDir.filePath(childDir)).filePath(grandchild)).isEmpty()){
                pictureFiles.append(QDir(childDir).filePath(grandchild)+".md");
            }
        }
    }

    // 先读取收藏文件（总是读取，但50%概率使用）
    QFile starFile(QDir(startPath).filePath(QDir(username).filePath("starred.md")));
    if (starFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&starFile);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (!line.isEmpty()) {
                starredFiles.append(line);
            }
        }
        starFile.close();
    }
    // 然后50%概率优先使用收藏文件
    if (!starredFiles.isEmpty() && QRandomGenerator::global()->bounded(2) == 0) {
        QStringList validFiles;
        for (const QString &filename : starredFiles) {
            if (QFile::exists(QDir(dir).filePath(filename)) && pictureFiles.contains(filename)) {
                validFiles.append(filename);
            }
        }

        if (!validFiles.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(validFiles.size());
            selectedFile = validFiles.at(randomIndex);
        }
    }

    if (selectedFile.isEmpty()) {
        // 过滤掉无图文件
        QStringList validFiles;
        for (const QString &filename : pictureFiles) {
            if (QFile::exists(QDir(dir).filePath(filename))) {
                validFiles.append(filename);
            }
        }
        if (!validFiles.isEmpty()) {
            int randomIndex = QRandomGenerator::global()->bounded(validFiles.size());
            selectedFile = validFiles.at(randomIndex);
        }
    }
    // 在不存在有图文件时扫描目录
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
            return Diary("",QDateTime::fromString("1970_01_01_00_00_00","yyyy_MM_dd_HH_mm_ss"),"","");
        }

        int randomIndex = QRandomGenerator::global()->bounded(files.size());
        selectedFile = files.at(randomIndex);
    }
    // 返回日记类
    return fileToDiary(QDir(dir).filePath(selectedFile));
}

QVector<Diary> FileOperation::findFile(QDateTime start, QDateTime end, const QString& dirPath) {

    QDir dir(dirPath);
    QFileInfoList entries = dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    QVector<Diary> resultFiles;

    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            QVector<Diary> found = findFile(start, end, entry.filePath());
            if (!found.isEmpty())resultFiles.append(found);
        } else {
            QString baseName = entry.baseName(); // 获取不带扩展名的文件名
            if(baseName!="folderInfo"){
                QDateTime fileTime = QDateTime::fromString(baseName, "yyyy_MM_dd_HH_mm_ss");
                if (fileTime >= start && fileTime <= end) {
                    resultFiles.append(fileToDiary(entry.filePath()));
                }
            }
        }
    }
    return resultFiles;
}

QVector<Diary> FileOperation::findFileByTime(QDateTime start, QDateTime end, const DiaryList& diaryType) {
    //起始日期 , 终止日期的后一天

    QDateTime date2 = end.addSecs(-1);

    QString dirPath = QDir(startPath).filePath(QDir(username).filePath("diary"));
    if (diaryType.getName()!="")dirPath = QDir(dirPath).filePath(diaryType.getType()+"_"+diaryType.getName());

    return findFile(start, date2, dirPath);
}

bool FileOperation::newFolder(const DiaryList& diaryType){
    QDir diaryDir = QDir(startPath).filePath(QDir(username).filePath("diary"));
    QDir picDir = QDir(startPath).filePath(QDir(username).filePath("picture"));
    // 创建diary下的子目录
    if (!diaryDir.mkdir(diaryType.getType()+"_"+diaryType.getName())) {
        qWarning() << "无法创建diary下的子目录";
        return 0;
    }
    // 创建picture下的子目录
    if (!picDir.mkdir(diaryType.getType()+"_"+diaryType.getName())) {
        qWarning() << "无法创建picture下的子目录";
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

bool FileOperation::deleteFolder(const DiaryList& diaryType){
    QString folderName = diaryType.getType()+"_"+diaryType.getName();
    QString diaryPath = QDir(QDir(startPath).filePath(QDir(username).filePath("diary"))).filePath(folderName);
    QString picPath = QDir(QDir(startPath).filePath(QDir(username).filePath("picture"))).filePath(folderName);

    // 检查日记文件夹是否存在且确实是文件夹（不是文件）
    QFileInfo diaryInfo(diaryPath);
    if (!diaryInfo.exists() || !diaryInfo.isDir()) {
        qDebug() << "不是有效的文件夹或文件夹不存在:" << diaryPath;
        return false;
    }
    // 检查图片文件夹是否存在且确实是文件夹（不是文件）
    QFileInfo picInfo(picPath);
    if (!picInfo.exists() || !picInfo.isDir()) {
        qDebug() << "不是有效的文件夹或文件夹不存在:" << picPath;
        return false;
    }

    if (!QDir(diaryPath).removeRecursively() || !QDir(picPath).removeRecursively()){
        return false;
    }
    return true;
}

QVector<DiaryList> FileOperation::allFolders(){
    QVector<DiaryList> folders;
    QDir dir = QDir(startPath).filePath(QDir(username).filePath("diary"));
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

QString FileOperation::getBaseDir(const DiaryList& diaryType){
    QDir dir = QDir(startPath).filePath(QDir(username).filePath("diary"));
    if (dir.exists(diaryType.getType()+"_"+diaryType.getName())){
        return dir.filePath(diaryType.getType()+"_"+diaryType.getName());
    }
    return QString();
}

QPair<Diary,QVector<int> > FileOperation::findFileByContent(const QString& target, bool newSearch, const DiaryList& diaryType){
    //每次返回一个搜到的文件（如有），以尽可能实时输出搜索结果；返回值0:文件的相对路径;1:词在该文件中的位置

    QString dir = QDir(startPath).filePath(QDir(username).filePath("diary"));
    if (diaryType.getName()!="")dir = QDir(dir).filePath(diaryType.getType()+"_"+diaryType.getName());
    QPair<Diary,QVector<int> > resultFiles=make_pair(Diary("",QDateTime::fromString("1970_01_01_00_00_00","yyyy_MM_dd_HH_mm_ss"),"",""),QVector<int>());

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
            resultFiles.first = fileToDiary(filePath);
            resultFiles.second = res;
            return resultFiles;
        }
    }
    searchword = "";
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
    if(dir=="")dir = QDir(startPath).filePath(QDir(username).filePath("diary"));
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
    if(dir=="")dir = QDir(startPath).filePath(QDir(username).filePath("diary"));
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
    encryptDir(QDir(startPath).filePath(QDir(username).filePath("picture")));
}

void FileOperation::decryptAll(){
    decryptDir();
    decryptDir(QDir(startPath).filePath(QDir(username).filePath("picture")));
}

Diary FileOperation::fileToDiary(const QString& filePath){
    QFile file(filePath);
    QString content;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        content = in.readAll();
        file.close();
    } else {
        qDebug() << "无法读取文件内容:" << file.errorString();
    }

    QFileInfo info(filePath);
    QDir parentDir = info.dir();
    QString folderName = parentDir.dirName();  // 文件夹名
    int first = folderName.lastIndexOf ("_"); //从后面查找"_"位置
    QString name = folderName.right(folderName.length ()-first-1); //从右边截取
    QString type = folderName.left(first); //从左边截取

    // 获取文件夹下所有图片路径
    QVector<QPixmap> pixmaps;
    QStringList imageFiles;
    QString picParentDir = QDir(startPath).filePath(QDir(username).filePath("picture"));
    QString picPath(QDir(QDir(picParentDir).filePath(folderName)).filePath(info.baseName()));

    qDebug()<<picPath;

    // 设置支持的图片格式（Qt支持的格式）
    QStringList formats;
    formats << "*.png" << "*.jpg" << "*.jpeg" << "*.bmp" << "*.gif";

    // 设置搜索选项
    QDir::Filters filters = QDir::Files | QDir::Readable | QDir::NoDotAndDotDot;

    // 遍历文件夹
    QDirIterator it(picPath, formats, filters);
    while (it.hasNext()) {
        imageFiles << it.next();
    }

    // 将图片文件转为QPixmap列表
    foreach (const QString& path, imageFiles) {
        QPixmap pixmap;

        if (pixmap.load(path)) {
            if (!pixmap.isNull()) {
                pixmaps.append(pixmap);
            } else {
                qWarning() << "加载图片失败(空图像):" << path;
            }
        } else {
            qWarning() << "加载图片失败:" << path;
        }
    }

    Diary d(name,
            QDateTime::fromString(info.baseName(), "yyyy_MM_dd_HH_mm_ss"),
            "",
            content,
            pixmaps,
            username,
            type);
    return d;
}

void FileOperation::setReminder(const reminder& r){
    QDir dir(QDir(startPath).filePath(username));
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
    QDir dir(QDir(startPath).filePath(username));
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
