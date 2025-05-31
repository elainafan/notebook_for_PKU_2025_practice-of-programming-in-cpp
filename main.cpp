#include "AppWindow.h"
#include"application.h"
#include <QApplication>
#include <QCoreApplication>
//#include <QWebEngineSettings>

int main(int argc, char *argv[])
{
    Application a(argc,argv);

    return a.exec();
}
