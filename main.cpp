#include <QApplication>
#include <QCoreApplication>
#include <QWebEngineSettings>
#include "widget.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);  // 禁用 OpenGL，改为软件渲染
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --disable-software-rasterizer --disable-gpu-compositing");

    QApplication a(argc, argv);
    Widget w;
    w.resize(800,600);
    w.show();
    return a.exec();
}
