QT += core gui widgets


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notebookBackend
TEMPLATE = app



SOURCES += \
    diarydisplaywidget.cpp \
    newdiarylist.cpp \
    reminderwidget.cpp\
    AppWindow.cpp \
    FileOperation.cpp \
    application.cpp \
    calendar.cpp \
    dailypromptwindow.cpp \
    diary.cpp \
    diarydetailwindow.cpp \
    diarylist.cpp \
    diarylistwidget.cpp \
    gradientoverlay.cpp \
    guiderwindow.cpp \
    loginwindow.cpp \
    main.cpp \
    CryptoUtils.cpp \
    mywidget.cpp \
    qaesencryption.cpp\
    searchwidget.cpp \
    userinfowidget.cpp \
    markdowneditorwidget.cpp\

HEADERS += \
    diarydisplaywidget.h \
    newdiarylist.h \
    reminderwidget.h\
    AppWindow.h \
    FileOperation.h \
    Headers.h \
    CryptoUtils.h \
    application.h \
    calendar.h \
    dailypromptwindow.h \
    diary.h \
    diarydetailwindow.h \
    diarylist.h \
    diarylistwidget.h \
    gradientoverlay.h \
    guiderwindow.h \
    loginwindow.h \
    mywidget.h \
    qaesencryption.h\
    reminder.h \
    searchwidget.h \
    userinfowidget.h \
    markdowneditorwidget.h\

FORMS +=

# 发布版本优化
CONFIG += c++17
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

RESOURCES += \
    Resources.qrc

DISTFILES += \
    interfaceRequirement.md


