#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T19:27:48
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_scriptbanktest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += tst_scriptbanktest.cc \
    ../../badmessage.cc \
    ../../scriptbankbuilder.cc \
    ../../unknownscript.cc \
    ../../scriptbank.cc
DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../
DEPENDPATH += ../../

HEADERS += \
    ../../badmessage.hh \
    ../../scriptbankbuilder.hh \
    ../../scriptlibrary.hh \
    ../../scriptprioritylibrary.hh \
    ../../unknownscript.hh \
    ../../scriptbank.hh \
    ../../priorityupdatesubject.hh \
    ../../priorityupdateobserver.hh

RESOURCES +=
