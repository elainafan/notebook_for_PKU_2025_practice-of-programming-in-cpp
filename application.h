#ifndef APPLICATION_H
#define APPLICATION_H
#include<QApplication>
#include"MainWindow.h"
#include"loginwindow.h"
#include"QStackedWidget.h"
#include"dailypromptwindow.h"
#include"diarydetailwindow.h"
#include"guiderwindow.h"

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
    Guider *newGuider;

    void setupConnection();
public slots:
    // 界面导航函数
    void navigateToLogin();
    void navigateToDailyPrompt(MyWidget *fromwhere);
    void navigateToMainWindow();
    void navigateToDiaryDetail();
    void navigateToGuider(MyWidget *fromwhere);
};

#endif // APPLICATION_H
