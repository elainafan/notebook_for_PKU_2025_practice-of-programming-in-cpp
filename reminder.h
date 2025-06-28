#ifndef REMINDER_H
#define REMINDER_H

#include <QDateTime>

class reminder
{
public:
    QDateTime time;
    QString task;
    reminder(const QDateTime &time_,const QString &task_)
        :time(time_),task(task_)
    {}
};

#endif // REMINDER_H
