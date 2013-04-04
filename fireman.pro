#-------------------------------------------------
#
# Project created by QtCreator 2013-04-02T22:44:02
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = fireman
TEMPLATE = app

include(./qextserialport-1.2rc/src/qextserialport.pri)

SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
