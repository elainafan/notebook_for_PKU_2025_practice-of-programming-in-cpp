#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QObject>
#include <QString>
#include <QDir>

class FileOperation : public QObject
{
    Q_OBJECT
public:
    explicit FileOperation(QString username_="_", QObject *parent = nullptr);

    bool findUser(QString user,QString password);
    QStringList findFile(const QString& fileName, const QString& diaryType);
    QStringList findFilebyTime(int year, int month=0, int day=0, QString dairyType="");

    bool newFolder(QString folderName);
    bool deleteFile(const QString& filename);

    QString username;
    static QString startPath;
};

#endif // FILEOPERATION_H
