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
    QString findFile(const QString& fileName);
    void newFile(const QString& content="");
    QString readFile(const QString& filename);
    void storeFile(const QString& filename, const QString& content);
    bool deleteFile(const QString& filename);

    QString username;
    static QString startPath;
};

#endif // FILEOPERATION_H
