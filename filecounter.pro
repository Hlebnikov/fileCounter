#-------------------------------------------------
#
# Project created by QtCreator 2016-02-11T21:46:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = filecounter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qexifimageheader_original.cpp

HEADERS  += mainwindow.h \
    qexifimageheader_original.h

FORMS    += mainwindow.ui

include(plugin/qtxlsx/src/xlsx/qtxlsx.pri)
