#-------------------------------------------------
#
# Project created by QtCreator 2015-03-04T23:33:01
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_signaltest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += tst_signaltest.cc \
    ../../signal.cc \

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    ../../signal.hh \

INCLUDEPATH += ../../
DEPENDPATH += ../../
