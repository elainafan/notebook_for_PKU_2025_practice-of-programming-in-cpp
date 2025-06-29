#ifndef REMINDERWIDGET_H
#define REMINDERWIDGET_H

#include <QWidget>

class ReminderWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ReminderWidget(QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
};

#endif // REMINDERWIDGET_H
