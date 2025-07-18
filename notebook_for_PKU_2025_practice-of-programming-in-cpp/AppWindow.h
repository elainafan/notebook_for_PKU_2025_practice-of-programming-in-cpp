#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "Headers.h"
#include <QMainWindow>
#include"mywidget.h"
#include"calendar.h"
#include"reminder.h"
#include"searchwidget.h"
#include"diarylistwidget.h"
#include"FileOperation.h"
#include"userinfowidget.h"
#include"reminderwidget.h"
#include"diarydisplaywidget.h"
#include"newdiarylist.h"
#include"markdowneditorwidget.h"

class AppWindow : public MyWidget
{
    Q_OBJECT

    FileOperation *fileOperator;
    QVector<DiaryList> diaryListVec;
    int curDiaryList=0;
public:
    AppWindow(FileOperation *fileOpt,MyWidget *parent = nullptr);
    ~AppWindow();

private slots:

private:
    QWidget *leftColumn;
    QWidget *searchWidgetWin;
    QWidget *diaryListWin;
    QWidget *calendarWin;
    QWidget *reminderWin;
    QWidget *rightColumn;
    SearchWidget *searchWidget;
    DiaryListWidget *diaryList;
    Calendar *calendar;
    ReminderWidget *reminder;
    UserInfoWidget *userInfo;
    QScrollArea *diaryScroll=nullptr;
    DiaryDisplayWidget *diaryDisplay=nullptr;
    MarkdownEditorWidget *mdEditor=nullptr;
public:
    void setupUI();
    void setupStyle();
    void setupConnection();
    void setupAnimation();

signals:
    void signOut(MyWidget *fromWhere);

public slots:
    void setupUserInfo();
    void buildDiaries(QVector<Diary> diaryVec,bool);
    void refresh();
    void search_refresh(QString txt="");
    void showAll_refresh();
    void getAllStarred_refresh();
};



#endif // APPWINDOW_H
