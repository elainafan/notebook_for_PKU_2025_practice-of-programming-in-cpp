#ifndef APPLICATION_H
#define APPLICATION_H
#include<QApplication>
#include"MainWindow.h"
#include"loginwindow.h"
#include"QStackedWidget.h"
#include"dailypromptwindow.h"
#include"diarydetailwindow.h"

class Application: public QApplication
{
public:
    Application(int& argc, char* argv[]);
    ~Application();
private:
    QStackedWidget *mainStack;
    LoginWindow *loginWindow;
    DailyPromptWindow *promptWindow;
    MainWindow *mainWindow;
    DiaryDetailWindow *detailWindow;

public:
    // 界面导航函数
    void navigateToLogin();
    void navigateToDailyPrompt();
    void navigateToMainWindow();
    void navigateToDiaryDetail();
};

#endif // APPLICATION_H
