#-------------------------------------------------
#
# Project created by QtCreator 2011-02-19T22:37:45
#
#-------------------------------------------------

QT       += core gui widgets
CONFIG   += c++11

TARGET = qImgSort
TEMPLATE = app


SOURCES += main.cpp\
    main_window.cpp \
    scanner_thread.cpp \
    name_utils.cpp

HEADERS  += \
    main_window.h \
    scanner_thread.h \
    name_utils.h

FORMS    += \
    main_window.ui
