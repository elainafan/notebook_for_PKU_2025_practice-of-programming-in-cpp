#ifndef GUIDERWINDOW_H
#define GUIDERWINDOW_H

#include <QWidget>
#include"Headers.h"
#include"mywidget.h"
class Guider : public MyWidget
{
    Q_OBJECT
    QLabel *welcomeLabel;
    QLabel *textLabel;
    QPushButton *enterButton;
public:
    explicit Guider(MyWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
    void setupAnimation();
signals:
};

#endif // GUIDERWINDOW_H
