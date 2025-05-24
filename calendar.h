#ifndef CALENDAR_H
#define CALENDAR_H

#include<QWidget>
#include"Headers.h"

class Calendar : public QWidget
{
    Q_OBJECT
public:
    explicit Calendar(QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
};

#endif // CALENDAR_H
