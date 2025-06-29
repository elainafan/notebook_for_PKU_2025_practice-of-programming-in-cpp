#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include "Headers.h"

class MyWidget : public QWidget
{
    Q_OBJECT
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

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
