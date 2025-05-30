QT += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = notebookBackend
TEMPLATE = app

SOURCES += \
    FileOperation.cpp \
    main.cpp \
    MainWindow.cpp \
    CryptoUtils.cpp \
    qaesencryption.cpp

HEADERS += \
    FileOperation.h \
    Headers.h \
    MainWindow.h \
    CryptoUtils.h \
    diarylist.h \
    qaesencryption.h

FORMS +=

# 发布版本优化
CONFIG += c++11
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
}
