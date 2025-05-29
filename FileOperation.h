#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QStack>
#include "diarylist.h"
#include "Reminder.h"

class FileOperation : public QObject
{
    Q_OBJECT
public:
    explicit FileOperation(QString username_="_", QObject *parent = nullptr);

    int signIn(QString user="",QString password=""); //缺省全部参数-自动登录，缺省密码-免密登录，不缺省-登录或注册
    void signOut();
    void setProfilePicture(const QPixmap& pic);
    QString getProfilePicture();            //返回图片文件地址，请在前端读取
    void changeUsername(QString newUsername);
    bool changePassword(QString newPassword);

    QString recommend();
    void setStar(const QString& fileName);  //若未starred则转为starred，若已starred则取消starred

    QStringList findFile(QDateTime start, QDateTime end, const QString& diaryType);
    QStringList findFileByTime(QDateTime start, QDateTime end, const DiaryList& diaryType=DiaryList("","",0)); //默认搜索全部笔记

    QPair<QString,QVector<int> > findFileByContent(const QString& target, bool newSearch, const DiaryList& diaryType=DiaryList("","",0));
    QVector<int> searchInFileByContent(const QString& filePath, const QString& target);

    bool newFolder(const DiaryList& diaryType);
    QVector<DiaryList> allFolders();
    bool deleteFile(const QString& filename);

    bool encryptDir(QString dir="");
    bool decryptDir(QString dir="");
    void encryptAll();
    void decryptAll();

    void setReminder(const Reminder& r);//若无该reminder则转为reminder，若已是reminder则取消reminder
    QVector<Reminder> getReminder();    //返回全部提醒事项

    QString username;
    QString password;
    static QString startPath;

private:
    QStack<QString> unsearchedFiles;
    QString searchword;
    QString searchDiaryType;
};

#endif // FILEOPERATION_H
