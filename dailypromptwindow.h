#ifndef DAILYPROMPTWINDOW_H
#define DAILYPROMPTWINDOW_H

#include <QWidget>
#include "Headers.h"
#include "mywidget.h"
#include"gradientoverlay.h"
class DailyPromptWindow : public MyWidget
{
    Q_OBJECT
    QLabel *imageLabel;
    QLabel *titleLabel;
    QLabel *contentLabel;
    QLabel *textBlockLabel;
    MyWidget *contentWidget;
    QPushButton *enterButton;
    QParallelAnimationGroup *entranceGroup;
    QParallelAnimationGroup *exitGroup;
    QParallelAnimationGroup *titleGroup;
    QSequentialAnimationGroup *btnGroup;
    QPropertyAnimation *titleSlide;
    QPropertyAnimation *titleFadeIn;
    QPropertyAnimation *btnWait;
    QPropertyAnimation *btnRiseUp;
    QPropertyAnimation *btnFadeIn;
    QPropertyAnimation *imageFadeIn;
    QPropertyAnimation *contentFadeIn;
    QGraphicsOpacityEffect *imageOpacityEffect;
    QGraphicsOpacityEffect *titleOpacityEffect;
    QGraphicsOpacityEffect *contentOpacityEffect;
    QGraphicsOpacityEffect *buttonOpacityEffect;
    GradientOverlay *myGradient;
public:
    explicit DailyPromptWindow(MyWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
    void setupAnimation();
    void promptAnimation();
    void setContent(const QString& title, const QString& text, const QPixmap& image);
public slots:
    void tryEntering();
signals:
    void entering(MyWidget *fromWhere);
};

#endif // DAILYPROMPTWINDOW_H
