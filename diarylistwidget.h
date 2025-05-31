#ifndef DIARYLISTWIDGET_H
#define DIARYLISTWIDGET_H

#include <QWidget>
#include "Headers.h"
#include"diarylist.h"

class DiaryListEntry: public QWidget{
    Q_OBJECT
    QFrame *underLine;
    QLabel *diaryListIcon;
    QLabel *diaryName;
    QPushButton *bcgrdBtn;
    const int wid=390;
    const int hei=40;
public:
    DiaryListEntry(const DiaryList &DL,QWidget *parent = nullptr);
    void setupStyle();
    void setupConnection();
    void setChecked(bool);
signals:
    void checkUpdated();
    void beingPressed();
};

class DiaryListWidget : public QWidget
{
    Q_OBJECT
    QScrollArea *scrArea;
    QVector<DiaryListEntry*> dlEntry;
    DiaryListEntry *newEntry;
    const int wid=390;
    const int hei=40;
    QTimer *scrollBarTimer;
    bool eventFilter(QObject *watched, QEvent *event) override;
public:
    explicit DiaryListWidget(QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
    void buildDiaryLists(const QVector<DiaryList> &vec);
signals:
    void changeList(const int &num);
private slots:
    void hideScrollBar();
    void updateBtnState(const int &num);
};

#endif // DIARYLISTWIDGET_H
