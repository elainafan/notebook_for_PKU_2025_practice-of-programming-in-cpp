#ifndef GRADIENTOVERLAY_H
#define GRADIENTOVERLAY_H

#include"Headers.h"
#include"mywidget.h"

class GradientOverlay:public MyWidget
{
public:
    GradientOverlay(MyWidget *parent = nullptr) : MyWidget(parent) {
        // 设置透明背景和忽略鼠标事件
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_TransparentForMouseEvents);
        setWindowFlags(Qt::FramelessWindowHint);
    }

public:
    void paintEvent(QPaintEvent *event) override;
};

#endif // GRADIENTOVERLAY_H
