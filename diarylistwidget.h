#ifndef DIARYLISTWIDGET_H
#define DIARYLISTWIDGET_H

#include <QWidget>
#include "Headers.h"
#include"diarylist.h"
#include"FileOperation.h"
#include"newdiarylist.h"

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
    void requestDelete();
    void requestShowAll();
protected:
    void mousePressEvent(QMouseEvent *event) override;
};

class DiaryListWidget : public QWidget
{
    Q_OBJECT
    NewDiaryList *newdl=nullptr;
    QScrollArea *scrArea=nullptr;
    QVector<DiaryListEntry*> dlEntry;
    DiaryListEntry *newEntry=nullptr;
    const int wid=390;
    const int hei=40;
    QTimer *scrollBarTimer=nullptr;
    bool eventFilter(QObject *watched, QEvent *event) override;
    FileOperation *fileOperator=nullptr;
    QVBoxLayout *mainLayout=nullptr;
public:
    explicit DiaryListWidget(FileOperation *fileOpt, QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
    void buildDiaryLists(const QVector<DiaryList> &vec);
    void setChecked(const int &,bool);

signals:
    void changeList(const int &num);
    void newListAdded();
    void showAll(const int &num);
private slots:
    void hideScrollBar();
    void updateBtnState(const int &num);
    void addNewList(const DiaryList &);

};

#endif // DIARYLISTWIDGET_H
