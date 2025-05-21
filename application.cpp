#include "application.h"
#include<QObject>

Application::Application(int& argc, char* argv[]): QApplication(argc,argv) {

    loginWindow=new LoginWindow();
    promptWindow=new DailyPromptWindow();
    mainWindow=new MainWindow();
    newGuider=new Guider();
    setupConnection();
    loginWindow->show();
    //loginWindow->checkLogState();
}

Application::~Application(){}

void Application::setupConnection(){
    connect(loginWindow,&LoginWindow::loggedIn,this,&Application::navigateToDailyPrompt);
    connect(loginWindow,&LoginWindow::registIn,this,&Application::navigateToGuider);
}

void Application::navigateToLogin(){

}

void Application::navigateToDailyPrompt(MyWidget *fromwhere){
    connect(fromwhere,&MyWidget::fadeAway,promptWindow,&DailyPromptWindow::appear);
    fromwhere->fade();
}

void Application::navigateToMainWindow(){

}

void Application::navigateToDiaryDetail(){

}

void Application::navigateToGuider(MyWidget *fromwhere){
    connect(fromwhere,&MyWidget::fadeAway,newGuider,&Guider::appear);
    fromwhere->fade();
}
