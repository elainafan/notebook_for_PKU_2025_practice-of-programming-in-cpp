#include "AppWindow.h"
#include"application.h"
#include <QApplication>
#include <QCoreApplication>
//#include <QWebEngineSettings>

int main(int argc, char *argv[])
{
    // 在 main 函数里 QApplication 之前设置环境变量
    //qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --disable-software-rasterizer");
    Application a(argc,argv);

    return a.exec();
}
