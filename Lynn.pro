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
    Communication/format.cpp \
    Communication/bootloadProcess.cpp \
    Componemts/crc32.cpp \
    UpdateFirmware/updatefirmware.cpp

HEADERS  += mainwindow.h \
    Componemts/myserialport.h \
    Communication/format.h \
    Communication/bootloadProcess.h \
    Componemts/crc32.h \
    UpdateFirmware/updatefirmware.h

FORMS    += mainwindow.ui \
    UpdateFirmware/updatefirmware.ui

RESOURCES += \
    ico.qrc
