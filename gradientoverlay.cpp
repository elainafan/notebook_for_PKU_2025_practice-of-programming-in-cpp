#include "gradientoverlay.h"

void GradientOverlay::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 定义渐变区域
    QLinearGradient gradient(0, height() * 0.3, 0, height());
    gradient.setColorAt(0.0, Qt::transparent);
    gradient.setColorAt(0.5, QColor(255,255,249,255));

    // 填充渐变
    painter.fillRect(rect(), gradient);
}
