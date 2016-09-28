#-------------------------------------------------
#
# Project created by QtCreator 2016-05-10T21:34:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Base3
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    universebody.cpp \
    universecomposite.cpp \
    config.cpp \
    zodiac.cpp \
    universecomponentfactory.cpp \
    zoom.cpp \
    pan.cpp \
    timestep.cpp \
    snap.cpp \
    infopanels.cpp

HEADERS  += dialog.h \
    universecomponent.h \
    universecomposite.h \
    universebody.h \
    config.h \
    zodiac.h \
    universecomponentfactory.h \
    zoom.h \
    alter.h \
    pan.h \
    timestep.h \
    snap.h \
    infopanels.h

FORMS    += dialog.ui

OTHER_FILES +=
