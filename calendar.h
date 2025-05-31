#ifndef CALENDAR_H
#define CALENDAR_H

#include<QWidget>
#include"Headers.h"

class Calendar : public QWidget
{
    Q_OBJECT

    QCalendarWidget *calendarWidget;
    QString calendarType;
    QStackedLayout *stackLayout;
    QVBoxLayout *weekLayout;
    QWidget *weekContainer;
    QWidget *weekLine[10];
public:
    explicit Calendar(QString tp,QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
    QDate getCurDate();
signals:
    void dateUpdated();
public slots:
    void closeWeekLine();
    void updateWeekLine();
};

#endif // CALENDAR_H
