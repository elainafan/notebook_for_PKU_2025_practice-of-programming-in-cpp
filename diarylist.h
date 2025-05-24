#ifndef DIARYLIST_H
#define DIARYLIST_H

#include"Headers.h"

class DiaryList
{
    QString name;
    int type;
    int colourType;
    //此处可考虑添加 vector<Diary> 存储容器，存储所有本中的日记（或者不提前存储，现查文件也可以）
public:
    DiaryList(const QString &nm,const int &tp,const int &ctp)
        :name(nm),type(tp),colourType(ctp)
    {}

    QString getName()const{return name;}
    int getType()const{return type;}
    int getColourType()const{return colourType;}
};

#endif // DIARYLIST_H
