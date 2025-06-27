#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QDirIterator>
#include <QStack>
#include "diarylist.h"
#include "Reminder.h"
#include "diary.h"

class FileOperation : public QObject
{
    Q_OBJECT
public:
    explicit FileOperation(QString username_="_", QObject *parent = nullptr);

    int signIn(QString user="",QString password=""); //缺省全部参数-自动登录，缺省密码-免密登录，不缺省-登录或注册
    void signOut();
    void setProfilePicture(const QPixmap& pic);
    QPixmap getProfilePicture();
    bool changeUsername(QString newUsername);
    bool changePassword(QString newPassword);

    Diary recommend();
    void setStar(const QString& fileName);  //若未starred则转为starred，若已starred则取消starred
    QVector<Diary> allStarred();

    QVector<Diary> findFile(QDateTime start, QDateTime end, const QString& diaryType);
    QVector<Diary> findFileByTime(QDateTime start, QDateTime end, const DiaryList& diaryType=DiaryList("","",0)); //默认搜索全部笔记

    QPair<Diary,QVector<int> > findFileByContent(const QString& target, bool newSearch=false, const DiaryList& diaryType=DiaryList("","",0));
    QVector<int> searchInFileByContent(const QString& filePath, const QString& target);

    bool newFolder(const DiaryList& diaryType);
    QVector<DiaryList> allFolders();
    bool deleteFile(const QString& filename);
    QString getBaseDir(const DiaryList& diaryType);

    bool encryptDir(QString dir="");
    bool decryptDir(QString dir="");
    void encryptAll();
    void decryptAll();

    void setReminder(const reminder& r);//若无该reminder则转为reminder，若已是reminder则取消reminder
    QVector<reminder> getReminder();    //返回全部提醒事项

    Diary fileToDiary(const QString& filePath);

    QString username;
    QString password;
    static QString startPath;

private:
    QStack<QString> unsearchedFiles;
    QString searchword;
    QString searchDiaryType;
};

#endif // FILEOPERATION_H
