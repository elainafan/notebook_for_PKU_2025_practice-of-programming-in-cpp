#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QStack>

class FileOperation : public QObject
{
    Q_OBJECT
public:
    explicit FileOperation(QString username_="_", QString password_="_", QObject *parent = nullptr);

    int findUser(QString user,QString password="");
    void signOut();

    QStringList findFile(QDateTime start, QDateTime end, const QString& diaryType);
    QStringList findFileByTime(int year, int month, int day, int year2, int month2, int day2, QString diaryType=""); //默认搜索全部笔记

    QPair<QString,QVector<int> > findFileByContent(const QString& target, QString diaryType="");
    QVector<int> searchInFileByContent(const QString& filePath, const QString& target);

    bool newFolder(QString folderName);
    bool deleteFile(const QString& filename);

    bool encryptDir(QString dir="");
    bool decryptDir(QString dir="");

    QString username;
    QString password;
    static QString startPath;

private:
    QStack<QString> unsearchedFiles;
    QString searchword;
    QString searchDiaryType;
};

#endif // FILEOPERATION_H
