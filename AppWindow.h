#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "Headers.h"
#include <QMainWindow>
#include"mywidget.h"
#include"calendar.h"
#include"reminder.h"
#include"searchwidget.h"
#include"diarylistwidget.h"

class AppWindow : public MyWidget
{
    Q_OBJECT

public:
    AppWindow(MyWidget *parent = nullptr);
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
    Reminder *reminder;
    void setupUI();
    void setupStyle();
    void setupConnection();
    void setupAnimation();

};



#endif // APPWINDOW_H
