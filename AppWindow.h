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

class AppWindow : public MyWidget
{
    Q_OBJECT

    FileOperation *fileOperator;
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
public:
    void setupUI();
    void setupStyle();
    void setupConnection();
    void setupAnimation();

signals:
    void signOut(MyWidget *fromWhere);

public slots:
    void setupUserInfo();
};



#endif // APPWINDOW_H
