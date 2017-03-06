#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T10:20:14
#
#-------------------------------------------------

QT       += core gui
QT       += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lynn
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Componemts/myserialport.cpp \
    Communication/deceivemontor.cpp \
    Communication/format.cpp \
    Communication/drive.cpp

HEADERS  += mainwindow.h \
    Componemts/myserialport.h \
    Communication/deceivemontor.h \
    Communication/format.h \
    Communication/drive.h

FORMS    += mainwindow.ui

RESOURCES += \
    ico.qrc
