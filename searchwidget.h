#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>
#include "Headers.h"

class SearchWidget : public QWidget
{
    Q_OBJECT

    QLabel *searchIcon;
    QLineEdit *searchEdit;
public:
    explicit SearchWidget(QWidget *parent = nullptr);
    void setupUI();
    void setupStyle();
    void setupConnection();
signals:
};

#endif // SEARCHWIDGET_H
