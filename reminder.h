#ifndef REMINDER_H
#define REMINDER_H

#include <QWidget>
#include"Headers.h"

class Reminder : public QWidget
{
    Q_OBJECT
public:
    explicit Reminder(QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
};

#endif // REMINDER_H
