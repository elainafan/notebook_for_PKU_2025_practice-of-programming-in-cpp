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
};

class DiaryListWidget : public QWidget
{
    Q_OBJECT
    QScrollArea *scrArea;
    DiaryListEntry *dlEntry[100];
    DiaryListEntry *newEntry;
    const int wid=390;
    const int hei=40;
public:
    explicit DiaryListWidget(QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
    void buildDiaryLists(const vector<DiaryList> &vec);
signals:

};

#endif // DIARYLISTWIDGET_H
