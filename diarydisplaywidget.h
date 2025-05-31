#ifndef DIARYDISPLAYWIDGET_H
#define DIARYDISPLAYWIDGET_H

#include <QWidget>
#include "mywidget.h"
#include"Headers.h"
#include "diary.h"

#ifndef DIARY_HEI
#define DIARY_HEI 900
#define DIARY_WID 700
#endif
class DiaryWidget: public QWidget
{
    Q_OBJECT
    Diary diary;
    QLabel *diaryBlock;
    void paintEvent(QPaintEvent *)
    {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }
public:
    explicit DiaryWidget(Diary dia,QWidget *parent=nullptr):QWidget(parent),diary(dia){
        setupUI();
        setupStyle();
        setupConnection();
    }
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
    void leftClicked(DiaryWidget* widget);    // 左键点击信号
    void rightClicked(DiaryWidget* widget);   // 右键点击信号
    void diaryClicked(DiaryWidget* widget, Qt::MouseButton button); // 通用点击信号

protected:
    void mousePressEvent(QMouseEvent *event) override; // 重写鼠标点击事件
};

class DiaryDisplayWidget : public MyWidget
{
    Q_OBJECT
    QVector<Diary> diaryVec;
    QVector< DiaryWidget* > diaWidgVec;
    QPushButton *newDiary;
public:
    explicit DiaryDisplayWidget(QVector<Diary> dVec,QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
    void openDiary(Diary);
};

#endif // DIARYDISPLAYWIDGET_H
