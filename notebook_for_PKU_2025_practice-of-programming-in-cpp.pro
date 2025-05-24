QT += core gui widgets
#QT += widgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BackEnd
TEMPLATE = app

# 添加QAESEncryption
include(Qt-AES/QAESEncryption.pri)

SOURCES += \
    AppWindow.cpp \
    FileOperation.cpp \
    application.cpp \
    calendar.cpp \
    dailypromptwindow.cpp \
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
    reminder.cpp \
    searchwidget.cpp \
    widget.cpp

HEADERS += \
    AppWindow.h \
    FileOperation.h \
    Headers.h \
    CryptoUtils.h \
    application.h \
    calendar.h \
    dailypromptwindow.h \
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
    widget.h

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


