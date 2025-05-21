#ifndef GUIDERWINDOW_H
#define GUIDERWINDOW_H

#include <QWidget>
#include"mywidget.h"
class Guider : public MyWidget
{
    Q_OBJECT
public:
    explicit Guider(MyWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
};

#endif // GUIDERWINDOW_H
