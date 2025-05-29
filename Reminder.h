#ifndef REMINDER_H
#define REMINDER_H

#include <QDateTime>

class Reminder
{
public:
    QDateTime time;
    QString task;
    Reminder(const QDateTime &time_,const QString &task_)
        :time(time_),task(task_)
    {}
};

#endif // REMINDER_H
