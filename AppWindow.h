#ifndef APPWINDOW_H
#define APPWINDOW_H

#include "Headers.h"
#include <QMainWindow>
#include"mywidget.h"

class QLineEdit;
class QPushButton;

class AppWindow : public MyWidget
{
    Q_OBJECT

public:
    AppWindow(MyWidget *parent = nullptr);
    ~AppWindow();

private slots:

private:
    void setupUI();
    void setupStyle();
    void setupConnection();
    void setupAnimation();
    QLineEdit *passwordInput;
    QLineEdit *filePathInput;
    QPushButton *browseButton;
    QPushButton *encryptButton;
    QPushButton *decryptButton;
};

#endif // APPWINDOW_H
