#include "MainWindow.h"
#include"application.h"
#include <QApplication>
#include <QCoreApplication>
//#include <QWebEngineSettings>
#include "widget.h"

int main(int argc, char *argv[])
{
    Application a(argc,argv);
    Widget w;
    w.resize(800,600);
    w.show();
    w.close();
    return a.exec();
}
