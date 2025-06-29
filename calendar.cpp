#include "calendar.h"

Calendar::Calendar(QString tp,QWidget *parent)
    : calendarType(tp),QWidget{parent}
{
    setFixedSize(400,281);
    setupUI();
    setupStyle();
    setupConnection();
    updateWeekLine();
}

void Calendar::setupUI(){
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QWidget *weekContainer = new QWidget(this);
    QVBoxLayout *weekLayout = new QVBoxLayout(weekContainer);
    weekLayout->setContentsMargins(15, 90, 15, 3);
    weekLayout->setSpacing(0);
    for(int i=0; i<6; i++){
        weekLine[i] = new QWidget();
        weekLine[i]->setFixedSize(370, 26);
        weekLayout->addWidget(weekLine[i]);
    }
    weekContainer->setFixedSize(400,281);

    calendarWidget = new QCalendarWidget(this);
    calendarWidget->setGridVisible(false);
    calendarWidget->setFirstDayOfWeek(Qt::Monday);
    calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
    calendarWidget->setHorizontalHeaderFormat(QCalendarWidget::SingleLetterDayNames);
    mainLayout->addWidget(calendarWidget);
}

void Calendar::setupStyle(){
    setStyleSheet(R"(
        Calendar{
            background: none;
            border: none;
        }
        QCalendarWidget {
            background-color: transparent;
            color: #333333;
            font-size: 20px;
        }

        QCalendarWidget QToolButton#qt_calendar_prevmonth {
            qproperty-icon: url(:/images/prevCalendarPage.png);
            icon-size: 24px, 24px;
            background-color: transparent;
            border: none;
        }

        QCalendarWidget QToolButton#qt_calendar_nextmonth {
            qproperty-icon: url(:/images/nextCalendarPage.png);
            icon-size: 24px, 24px;
            background-color: transparent;
            border: none;
        }
        /*
        QCalendarWidget QToolButton#qt_calendar_prevmonth:hover,
        QCalendarWidget QToolButton#qt_calendar_nextmonth:hover {
            background-color: #e6e6e6;
        }*/

        QCalendarWidget QToolButton#qt_calendar_monthbutton {
            color: #FFFFFF;
            background-color: #7030A0;
            font-size: 20px;
            font-weight: bold;
            border: 0px solid #e0e0e0;
            border-radius: 4px;
            padding: 4px;
            margin: 4px;
        }
        QCalendarWidget QToolButton#qt_calendar_yearbutton {
            color: #FFFFFF;
            background-color: #7030A0;
            font-size: 20px;
            font-weight: bold;
            border: 0px solid #e0e0e0;
            border-radius: 4px;
            padding: 4px;
            margin: 4px;
        }
        QCalendarWidget QToolButton#qt_calendar_monthbutton:hover,
        QCalendarWidget QToolButton#qt_calendar_yearbutton:hover {
            background-color: #602090;
            border-color: #ccc;
        }
        QCalendarWidget QToolButton#qt_calendar_monthbutton:pressed,
        QCalendarWidget QToolButton#qt_calendar_yearbutton:pressed {
            background-color: #602090;
            border-color: #aaa;
        }
        QCalendarWidget QToolButton::menu-indicator {
            image: none;
            subcontrol-position: right center;
            subcontrol-origin: padding;
            padding-right: 6px;
        }

        QCalendarWidget QMenu {
            width: 150px;
            left: 20px;
            color: white;
            background-color: #888888;
        }
        QCalendarWidget QSpinBox {
            color: #333333;
            background-color: white;
            selection-background-color: rgb(136, 136, 136);
            selection-color: white;
        }
        QCalendarWidget QHeaderView::item {
            background-color: transparent;
        }
        QCalendarWidget QWidget#qt_calendar_navigationbar {
            background-color: transparent;
        }
        QCalendarWidget QTableView {
            background-color: transparent;
            selection-background-color: transparent;
            selection-color: white;
        }
        QCalendarWidget QTableView::item:selected {
            background-color: #7030A0;
            background-size: 14px, 14px;
            color: white;
            border-radius: 12px;
            margin: 1px;
            padding: 1px;
        }
        QCalendarWidget QTableView::item {
            background-color:transparent;
            margin: 1px;
            padding: 0px;
            padding-top: 4px;
        }
    )");

}

void Calendar::setupConnection(){

    connect(calendarWidget, &QCalendarWidget::selectionChanged,
            this, &Calendar::updateWeekLine);
    connect(calendarWidget, &QCalendarWidget::selectionChanged,
            this, &Calendar::dateUpdated);
    connect(calendarWidget, &QCalendarWidget::currentPageChanged,
            this, &Calendar::closeWeekLine);

}

QDate Calendar::getCurDate(){
    return calendarWidget->selectedDate();
}

QDate Calendar::getCurWeek(){
    QDate day=calendarWidget->selectedDate();
    return day.addDays(1-day.dayOfWeek());
}

QDate Calendar::getCurMonth(){
    QDate day = calendarWidget->selectedDate();
    return QDate(day.year(),day.month(),1);
}

QDate Calendar::getCurYear(){
    QDate day = calendarWidget->selectedDate();
    return QDate(day.year(),1,1);
}

void Calendar::closeWeekLine(){
    for(int i = 0; i < 6; i++) {
        weekLine[i]->setStyleSheet("border-radius:12px; background-color:transparent;");
    }
}

void Calendar::updateWeekLine(){
    //if(calendarType!="weekly")return;
    QDate selectedDate = calendarWidget->selectedDate();
    if (!selectedDate.isValid()) return;

    QDate firstOfMonth(selectedDate.year(), selectedDate.month(), 1);
    int dayOfWeek = firstOfMonth.dayOfWeek();

    int dateRow = ((selectedDate.day()+dayOfWeek-1-1)/7);
    if(dayOfWeek==1)dateRow++;
    closeWeekLine();
    if(dateRow >= 0 && dateRow < 6)
        weekLine[dateRow]->setStyleSheet("border-radius:12px; background-color:rgba(112,48,160,30);");

}
