#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "Headers.h"

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = nullptr);
    void setupUI();
    void setupConnection();
    void setupStyle();
signals:
    void fadeAway();
    void appeared();
public slots:
    void fade();
    void appear();
};

#endif // MYWIDGET_H
