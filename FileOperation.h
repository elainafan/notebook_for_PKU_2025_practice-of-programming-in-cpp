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
    explicit FileOperation(QString username_="_", QObject *parent = nullptr);

    bool findUser(QString user,QString password);
    QStringList findFile(const QString& fileName, const QString& diaryType);
    QStringList findFileByTime(int year, int month=0, int day=0, QString dairyType="");

    QPair<QString,QVector<int> > findFileByContent(const QString& target, QString diaryType);
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
};

#endif // FILEOPERATION_H
