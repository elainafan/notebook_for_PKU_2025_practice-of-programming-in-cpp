#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QObject>
#include <QString>

class FileOperation : public QObject
{
    Q_OBJECT
public:
    explicit FileOperation(QObject *parent = nullptr);

    QString findFile(const QString &fileName, const QString &startPath);
    void newFile(const QString& content);
    QString readFile(const QString& filename);
    void storeFile(const QString& filename, const QString& content);
    bool deleteFile(const QString& filename);

};
#endif // FILEOPERATION_H
