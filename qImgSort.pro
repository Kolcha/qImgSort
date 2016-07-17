#-------------------------------------------------
#
# Project created by QtCreator 2011-02-19T22:37:45
#
#-------------------------------------------------

QT       += core gui widgets svg
CONFIG   += c++11

TARGET = qImgSort
TEMPLATE = app


SOURCES += src/main.cpp\
    src/main_window.cpp \
    src/scanner_thread.cpp \
    src/name_utils.cpp

HEADERS  += \
    src/main_window.h \
    src/scanner_thread.h \
    src/name_utils.h

FORMS    += \
    src/main_window.ui

TRANSLATIONS += \
    lng/qimgsort_ru.ts

win32:RC_FILE = res/qimgsort.rc
macx {
    ICON = res/qimgsort.icns
    QMAKE_INFO_PLIST = res/qimgsort.plist
}

RESOURCES += \
    qimgsort.qrc
