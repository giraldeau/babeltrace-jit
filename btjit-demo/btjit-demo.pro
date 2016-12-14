QT += core
QT -= gui

TARGET = btjit-demo
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++14

TEMPLATE = app

SOURCES += main.cpp

DEFINES += TOPSRCDIR=\\\"\"$${top_srcdir}\"\\\"
