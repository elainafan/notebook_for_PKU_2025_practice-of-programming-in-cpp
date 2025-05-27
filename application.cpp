#include "application.h"
#include<QObject>

Application::Application(int& argc, char* argv[]): QApplication(argc,argv) {


    fileOperator= new FileOperation();
    loginWindow=new LoginWindow(fileOperator);
    promptWindow=new DailyPromptWindow();
    appWindow=new AppWindow(fileOperator);
    newGuider=new Guider();
    setupConnection();
    //fileOperator->signOut();
    if(!loginWindow->checkLogState())loginWindow->show();
    //appWindow->show();
}

Application::~Application(){
    if(!loginWindow->isAutoLogging())fileOperator->signOut();
}

void Application::setupConnection(){
    connect(loginWindow,&LoginWindow::loggedIn,this,&Application::navigateToDailyPrompt);
    connect(loginWindow,&LoginWindow::registIn,this,&Application::navigateToGuider);
    connect(promptWindow,&DailyPromptWindow::entering,this,&Application::navigateToMainWindow);
    connect(newGuider,&Guider::entering,this,&Application::navigateToMainWindow);
    connect(appWindow,&AppWindow::signOut,this,&Application::navigateToLogin);
}

void Application::navigateToLogin(MyWidget *fromWhere){
    //qDebug()<<"navigation...";
    connect(fromWhere,&MyWidget::fadeAway,loginWindow,&LoginWindow::appear);
    fromWhere->fade();
}

void Application::navigateToDailyPrompt(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,promptWindow,&DailyPromptWindow::appear,Qt::SingleShotConnection);
    fromWhere->fade();
}

void Application::navigateToMainWindow(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,appWindow,&AppWindow::appear,Qt::SingleShotConnection);
    connect(fromWhere,&MyWidget::fadeAway,appWindow,&AppWindow::setupUserInfo,Qt::SingleShotConnection);
    fromWhere->fade();
}

void Application::navigateToDiaryDetail(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,appWindow,&AppWindow::appear,Qt::SingleShotConnection);
    fromWhere->fade();
}

void Application::navigateToGuider(MyWidget *fromWhere){
    connect(fromWhere,&MyWidget::fadeAway,newGuider,&Guider::appear,Qt::SingleShotConnection);
    fromWhere->fade();
}
