#include "application.h"
#include<QObject>

Application::Application(int& argc, char* argv[]): QApplication(argc,argv) {



    loginWindow=new LoginWindow();
    promptWindow=new DailyPromptWindow();
    appWindow=new AppWindow();
    newGuider=new Guider();
    setupConnection();
    if(!loginWindow->checkLogState())loginWindow->show();
    //appWindow->show();
}

Application::~Application(){}

void Application::setupConnection(){
    connect(loginWindow,&LoginWindow::loggedIn,this,&Application::navigateToDailyPrompt);
    connect(loginWindow,&LoginWindow::registIn,this,&Application::navigateToGuider);
    connect(promptWindow,&DailyPromptWindow::entering,this,&Application::navigateToMainWindow);
    connect(newGuider,&Guider::entering,this,&Application::navigateToMainWindow);
}

void Application::navigateToLogin(){

}

void Application::navigateToDailyPrompt(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,promptWindow,&DailyPromptWindow::appear);
    fromWhere->fade();
}

void Application::navigateToMainWindow(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,appWindow,&AppWindow::appear);
    fromWhere->fade();
}

void Application::navigateToDiaryDetail(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,appWindow,&AppWindow::appear);
    fromWhere->fade();
}

void Application::navigateToGuider(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,newGuider,&Guider::appear);
    fromWhere->fade();
}
