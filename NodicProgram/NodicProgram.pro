#-------------------------------------------------
#
# Project created by QtCreator 2016-07-28T10:49:36
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NodicProgramAppV1.1
TEMPLATE = app
RC_FILE = nodic.rc



SOURCES += main.cpp\
        mainwindow.cpp \
    mythread.cpp

HEADERS  += mainwindow.h \
    mythread.h

FORMS    += mainwindow.ui
DESTDIR  += "E:\project\github\nodicprogrammingApp"

RESOURCES += \
    appprogram.qrc
