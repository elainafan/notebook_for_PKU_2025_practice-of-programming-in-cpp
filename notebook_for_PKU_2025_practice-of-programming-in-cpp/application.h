#ifndef APPLICATION_H
#define APPLICATION_H
#include<QApplication>
#include"AppWindow.h"
#include"loginwindow.h"
#include"QStackedWidget.h"
#include"dailypromptwindow.h"
#include"diarydetailwindow.h"
#include"guiderwindow.h"
#include"FileOperation.h"
#include"Headers.h"
#include"newdiarylist.h"

class Application: public QApplication
{
public:
    Application(int& argc, char* argv[]);
    ~Application();
private:
    LoginWindow *loginWindow;
    DailyPromptWindow *promptWindow;
    AppWindow *appWindow;
    DiaryDetailWindow *detailWindow;
    Guider *newGuider;
    FileOperation *fileOperator;
    void setupConnection();
public slots:
    // 界面导航函数
    void navigateToLogin(MyWidget *fromWhere);
    void navigateToDailyPrompt(MyWidget *fromWhere);
    void navigateToMainWindow(MyWidget *fromWhere);
    void navigateToDiaryDetail(MyWidget *fromWhere);
    void navigateToGuider(MyWidget *fromWhere);
};

#endif // APPLICATION_H
