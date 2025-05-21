#include "dailypromptwindow.h"

DailyPromptWindow::DailyPromptWindow(QWidget *parent)
    : QWidget{parent}
{
    setWindowTitle("Remember the day when...");
    resize(600,900);
}
