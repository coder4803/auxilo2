#-------------------------------------------------
#
# Project created by QtCreator 2015-03-10T23:01:24
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_signalqueuetest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_signalqueuetest.cc \
    ../../signalqueue.cc \
    ../../signal.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"


DEPENDPATH += ../../
INCLUDEPATH += ../../

HEADERS += \
    ../../signalqueue.hh \
    ../../signal.hh
