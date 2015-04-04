#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T00:15:47
#
#-------------------------------------------------

QT       += core gui
QT       += sql

CONFIG   += c++14

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GS_1
TEMPLATE = app

SOURCES +=\
    dblogin.cpp \
    sqlconsole.cpp \
    main.cpp \
    mainwindow.cpp \
    visitview.cpp

HEADERS  += \
    dblogin.h \
    globaldata.h \
    sqlconsole.h \
    mainwindow.h \
    visitview.h

FORMS    += \
    dblogin.ui \
    sqlconsole.ui \
    mainwindow.ui \
    visitview.ui
