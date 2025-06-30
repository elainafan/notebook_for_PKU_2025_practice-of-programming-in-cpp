#include <QApplication>
#include <QScrollArea>
#include <QWidget>
#include <QWheelEvent>
#include <QPropertyAnimation>
#include<QTimer>
#include<QScrollBar>

class SmoothScrollArea : public QScrollArea {
    Q_OBJECT
public:
    SmoothScrollArea(QWidget *parent = nullptr)
        : QScrollArea(parent), m_velocity(0), m_isScrolling(false) {
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // 隐藏默认滚动条
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        viewport()->installEventFilter(this); // 监听视口事件
        setStyleSheet(R"(
            QScrollBar:vertical {
                background: rgba(200, 200, 200, 50);
                width: 8px;
                margin: 2px;
            }

            QScrollBar::handle:vertical {
                background: rgba(100, 100, 100, 150);
                border-radius: 4px;
            }
                      )");
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override {
        if (obj == viewport() && event->type() == QEvent::Wheel) {
            QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
            handleWheelEvent(wheelEvent); // 自定义处理触控板事件
            return true; // 阻止事件继续传递
        }
        return QScrollArea::eventFilter(obj, event);
    }

private:
    void handleWheelEvent(QWheelEvent *event) {
        // 优先使用像素级精度（触控板/触摸屏）
        QPoint delta;
        if (!event->pixelDelta().isNull()) {
            delta = event->pixelDelta();
        } else {
            delta = event->angleDelta() / 8; // 兼容鼠标滚轮
        }

        // 惯性滚动逻辑
        m_velocity = delta.y() * 0.3; // 速度衰减系数
        if (!m_isScrolling) startInertialScroll();
    }

    void startInertialScroll() {
        m_isScrolling = true;
        QTimer::singleShot(16, this, [this]() { // 60fps更新
            if (qAbs(m_velocity) < 0.5) {
                m_isScrolling = false;
                return;
            }

            QScrollBar *vBar = verticalScrollBar();
            int newValue = vBar->value() - m_velocity;
            vBar->setValue(newValue);

            m_velocity *= 0.95; // 摩擦系数
            startInertialScroll();
        });
    }

    qreal m_velocity;
    bool m_isScrolling;
};
