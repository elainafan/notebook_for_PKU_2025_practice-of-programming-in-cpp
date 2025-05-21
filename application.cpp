#include "application.h"
#include<QObject>

Application::Application(int& argc, char* argv[]): QApplication(argc,argv) {

    loginWindow=new LoginWindow();
    QObject::connect(loginWindow,&LoginWindow::loggedIn,this,&Application::navigateToDailyPrompt);
    promptWindow=new DailyPromptWindow();
    mainWindow=new MainWindow();
    loginWindow->show();
    loginWindow->checkLogState();
}

Application::~Application(){}

void Application::navigateToLogin(){

}

void Application::navigateToDailyPrompt(){
    loginWindow->close();
    promptWindow->show();
}

void Application::navigateToMainWindow(){

}

void Application::navigateToDiaryDetail(){

}

