#-------------------------------------------------
#
# Project created by QtCreator 2012-05-22T11:09:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = evo
TEMPLATE = app
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR  = build
RCC_DIR = build

SOURCES += main.cpp\
        mainwindow.cpp \
    world.cpp \
    animal.cpp \
    map.cpp

HEADERS  += mainwindow.h \
    world.h \
    animal.h \
    common.h \
    map.h

FORMS    += mainwindow.ui
