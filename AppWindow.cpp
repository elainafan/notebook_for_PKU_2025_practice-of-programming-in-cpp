#include "AppWindow.h"
#include "CryptoUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

AppWindow::AppWindow(MyWidget *parent)
    : MyWidget(parent)
{
    setupUI();
    setWindowTitle("Easy Diary");
    resize(1200, 900);
}

AppWindow::~AppWindow()
{
}

void AppWindow::setupUI(){

}
