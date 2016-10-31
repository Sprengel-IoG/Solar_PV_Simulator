#-------------------------------------------------
#
# Project created by QtCreator 2016-09-17T13:49:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
CONFIG += c++11

TARGET = Solar_PV_v0
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    solardata.cpp \
    pvsimulation.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    solardata.h \
    pvsimulation.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
