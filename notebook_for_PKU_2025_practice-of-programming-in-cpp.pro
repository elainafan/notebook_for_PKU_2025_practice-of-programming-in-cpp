QT += core gui widgets
QT += widgets webenginewidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BackEnd
TEMPLATE = app

# 添加QAESEncryption
include(Qt-AES/QAESEncryption.pri)

SOURCES += \
    FileOperation.cpp \
    application.cpp \
    dailypromptwindow.cpp \
    diarydetailwindow.cpp \
    gradientoverlay.cpp \
    guiderwindow.cpp \
    loginwindow.cpp \
    main.cpp \
    MainWindow.cpp \
    CryptoUtils.cpp \
    mywidget.cpp \
    qaesencryption.cpp\
    widget.cpp

HEADERS += \
    FileOperation.h \
    Headers.h \
    MainWindow.h \
    CryptoUtils.h \
    application.h \
    dailypromptwindow.h \
    diarydetailwindow.h \
    gradientoverlay.h \
    guiderwindow.h \
    loginwindow.h \
    mywidget.h \
    qaesencryption.h\
    widget.h

FORMS +=

# 发布版本优化
CONFIG += c++17
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

RESOURCES += \
    Resources.qrc


