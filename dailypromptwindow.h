#ifndef DAILYPROMPTWINDOW_H
#define DAILYPROMPTWINDOW_H

#include <QWidget>
#include "Headers.h"
#include "mywidget.h"
class DailyPromptWindow : public MyWidget
{
    Q_OBJECT
public:
    explicit DailyPromptWindow(MyWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
};

#endif // DAILYPROMPTWINDOW_H
