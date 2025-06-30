#ifndef NEWDIARYLIST_H
#define NEWDIARYLIST_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QDialogButtonBox>
#include <QPixmap>
#include "diarylist.h"

class NewDiaryList : public QDialog
{
    Q_OBJECT

public:
    explicit NewDiaryList(QWidget *parent = nullptr);
    ~NewDiaryList();

    DiaryList getDiaryList() const;
    bool isAccepted() const;
signals:
    void diaryListCreated(const DiaryList &newList);

private:
    void setupUI();
    void setupConnections();
    void loadColorIcons();

    QLineEdit *nameEdit;
    QButtonGroup *typeGroup;
    QRadioButton *dailyRadio;
    QRadioButton *weeklyRadio;
    QRadioButton *monthlyRadio;
    QRadioButton *yearlyRadio;
    QComboBox *colorCombo;
    QDialogButtonBox *buttonBox;

    QString diaryName;
    QString diaryType;
    int colorIndex;
    bool accepted;

    static const int NUM_COLORS = 9; // 假设有10种颜色图标
};

#endif // NEWDIARYLIST_H
