#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T13:53:41
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_sharedmmaptest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += tst_sharedmmaptest.cpp \
    sharedmmap.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    sharedmmap.h
