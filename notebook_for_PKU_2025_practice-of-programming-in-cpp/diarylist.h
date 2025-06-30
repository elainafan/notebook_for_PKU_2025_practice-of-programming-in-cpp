#ifndef DIARYLIST_H
#define DIARYLIST_H

#include"Headers.h"

class DiaryList
{
    QString name;
    QString calendarType;
    int colourType;
    //此处可考虑添加 vector<Diary> 存储容器，存储所有本中的日记（或者不提前存储，现查文件也可以）
public:
    DiaryList(const QString &nm,const QString &caltp,const int &ctp)
        :name(nm),calendarType(caltp),colourType(ctp)
    {}

    QString getName()const{return name;}
    QString getType()const{return calendarType;}
    int getColourType()const{return colourType;}
};

#endif // DIARYLIST_H
