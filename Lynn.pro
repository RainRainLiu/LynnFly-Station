#-------------------------------------------------
#
# Project created by QtCreator 2017-02-21T10:20:14
#
#-------------------------------------------------

QT       += core gui
QT       += serialport
QT      +=charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Lynn
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Componemts/myserialport.cpp \
    Componemts/crc32.cpp \
    UpdateFirmware/updatefirmware.cpp \
    Componemts/communicationFormat.cpp \
    UpdateFirmware/bootloadProcess.cpp \
    LineChart/linechart.cpp \
    LineChart/gesture.cpp \
    Componemts/myfunction.cpp

HEADERS  += mainwindow.h \
    Componemts/myserialport.h \
    Componemts/crc32.h \
    UpdateFirmware/updatefirmware.h \
    Componemts/communicationFormat.h \
    UpdateFirmware/bootloadProcess.h \
    LineChart/linechart.h \
    LineChart/gesture.h \
    Componemts/myfunction.h

FORMS    += mainwindow.ui \
    UpdateFirmware/updatefirmware.ui \
    LineChart/linechart.ui

RESOURCES += \
    ico.qrc
