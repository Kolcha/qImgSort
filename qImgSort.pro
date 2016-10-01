#
#   qImgSort - sort image files by resolution
#   Copyright (C) 2011-2016  Nick Korotysh <nick.korotysh@gmail.com>
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
