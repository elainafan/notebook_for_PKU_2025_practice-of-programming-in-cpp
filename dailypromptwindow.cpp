#include "dailypromptwindow.h"

DailyPromptWindow::DailyPromptWindow(MyWidget *parent)
    : MyWidget{parent}
{
    setupUI();
    setupStyle();
    setupConnection();
    setupAnimation();
    setContent("TEST",
                       "This is the test text.\n"
               "Test twice.\n"
               "Test three times.\n"
               ,QPixmap(":/images/testImage.png"));
}


void DailyPromptWindow::setupUI(){
    setFixedSize(600,900);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 图片标签
    imageLabel = new QLabel(this);
    imageLabel->setAlignment(Qt::AlignCenter);
    //imageLabel->setStyleSheet("border-radius: 10px;");
    mainLayout->addWidget(imageLabel);

    // 标题标签
    titleLabel = new QLabel(this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->raise();
    titleLabel->move(50,50);
    titleLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //mainLayout->addWidget(titleLabel);

    //渐变覆盖
    myGradient = new GradientOverlay(this);
    myGradient->resize(size());
    //myGradient->raise();

    // 内容标签（附带自制文本框）
    contentWidget = new MyWidget(this);
    //contentWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    //contentWidget->setFixedSize(600,600);
    textBlockLabel = new QLabel(contentWidget);
    textBlockLabel->setPixmap(QPixmap(":/images/textBlockForPrompt.png").scaled(580,900,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    contentLabel = new QLabel(contentWidget);
    //contentLabel->setGraphicsEffect(shadowEffect);
    contentLabel->setWordWrap(true);
    contentLabel->setAlignment(Qt::AlignCenter);
    //mainLayout->setContentsMargins(20, 20, 20, 20);

    QVBoxLayout *container = new QVBoxLayout(this);
    container->setContentsMargins(10,10,10,10);
    container->addWidget(contentWidget);

    mainLayout->addSpacing(-280);
    mainLayout->addLayout(container);

    // 进入按钮
    enterButton = new QPushButton(this);
    enterButton->setFixedSize(80,80);
    enterButton->move(260,780);
    //connect(enterButton, &QPushButton::clicked, this, &DailyPromptWindow::onEnterClicked);
    /*
    mainLayout->addSpacing(-100);
    mainLayout->addWidget(enterButton, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(20);
    */


    // 初始隐藏所有元素
    {
    imageOpacityEffect = new QGraphicsOpacityEffect(imageLabel);
    imageOpacityEffect->setOpacity(0);
    imageLabel->setGraphicsEffect(imageOpacityEffect);

    titleOpacityEffect = new QGraphicsOpacityEffect(titleLabel);
    titleOpacityEffect->setOpacity(0);
    titleLabel->setGraphicsEffect(titleOpacityEffect);

    contentOpacityEffect = new QGraphicsOpacityEffect(contentWidget);
    contentOpacityEffect->setOpacity(0);
    contentWidget->setGraphicsEffect(contentOpacityEffect);

    buttonOpacityEffect = new QGraphicsOpacityEffect(enterButton);
    buttonOpacityEffect->setOpacity(0);
    enterButton->setGraphicsEffect(buttonOpacityEffect);
    }
}

void DailyPromptWindow::setupStyle(){
    setStyleSheet(R"(
        QWidget{
            padding: 0px;

        }
        )");
    titleLabel->setStyleSheet(R"(
            QLabel {
                font: bold 60px '微软雅黑';
                color: #FFFFFF;
                padding: 0px;
            }
        )");
    contentLabel->setStyleSheet(R"(
            QLabel {
                font: 20px '微软雅黑';
                color: #7f8c8d;
                line-height: 1.5;
                padding: 30px;
                background: none;
                border:none;
            }
        )");
    enterButton->setStyleSheet(R"(
            QPushButton {
                font: bold 14px '微软雅黑';
                color: white;
                background:none;
                border-image: url(":/images/enter.png") 0 0 0 0 stretch stretch;
            }QPushButton:hover{
                border-image: url(":/images/enter_hover.png") 0 0 0 0 stretch stretch;
            }QPushButton:pressed{
                border-image: url(":/images/enter_pressed.png") 0 0 0 0 stretch stretch;
            }
        )");


}

void DailyPromptWindow::setupConnection(){
    connect(this,&MyWidget::appeared,this,&DailyPromptWindow::promptAnimation);
}

void DailyPromptWindow::setupAnimation(){
    //图片淡入动画


    imageFadeIn = new QPropertyAnimation(imageOpacityEffect, "opacity");
    imageFadeIn->setDuration(800);
    imageFadeIn->setStartValue(0.0);
    imageFadeIn->setEndValue(1.0);
    imageFadeIn->setEasingCurve(QEasingCurve::InOutQuad);

    // 标题动画组
    titleSlide = new QPropertyAnimation(titleLabel, "pos");
    titleSlide->setDuration(1500);
    titleSlide->setEasingCurve(QEasingCurve::OutBack);

    titleFadeIn= new QPropertyAnimation(titleOpacityEffect,"opacity");
    titleFadeIn->setDuration(1500);
    titleFadeIn->setStartValue(0);
    titleFadeIn->setEndValue(1);

    titleGroup= new QParallelAnimationGroup(titleLabel);
    titleGroup->addAnimation(titleFadeIn);
    titleGroup->addAnimation(titleSlide);

    // 内容淡入动画
    contentFadeIn = new QPropertyAnimation(contentOpacityEffect, "opacity");
    contentFadeIn->setDuration(800);
    contentFadeIn->setStartValue(0.0);
    contentFadeIn->setEndValue(1.0);


    // 按钮动画组
    btnFadeIn = new QPropertyAnimation(buttonOpacityEffect,"opacity");
    btnFadeIn->setDuration(400);
    btnFadeIn->setStartValue(0);
    btnFadeIn->setEndValue(1);

    btnWait = new QPropertyAnimation(buttonOpacityEffect,"opacity");
    btnWait->setDuration(2000);

    btnGroup = new QSequentialAnimationGroup(enterButton);
    btnGroup->addAnimation(btnWait);
    btnGroup->addAnimation(btnFadeIn);

    // 开场动画组
    entranceGroup = new QParallelAnimationGroup(this);
    entranceGroup->addAnimation(imageFadeIn);
    entranceGroup->addAnimation(titleGroup);
    entranceGroup->addAnimation(contentFadeIn);
    entranceGroup->addAnimation(btnGroup);

    // 退出动画组
    exitGroup = new QParallelAnimationGroup(this);
    QPropertyAnimation *windowFade = new QPropertyAnimation(this, "windowOpacity");
    windowFade->setDuration(500);
    windowFade->setEndValue(0.0);
    exitGroup->addAnimation(windowFade);
    connect(exitGroup, &QParallelAnimationGroup::finished, this, &QWidget::close);
}

void DailyPromptWindow::promptAnimation(){

    titleSlide->setStartValue(QPoint(-titleLabel->width(), titleLabel->y()));
    titleSlide->setEndValue(titleLabel->pos());


    //titleOpacityEffect->setOpacity(1);

    entranceGroup->start();

}

void DailyPromptWindow::setContent(const QString& title, const QString& text, const QPixmap& image) {
    titleLabel->setText(title);
    titleLabel->adjustSize();
    contentLabel->setText(text);
    contentLabel->adjustSize();
    imageLabel->setPixmap(image.scaled(600, 500, Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
    imageLabel->adjustSize();
}
