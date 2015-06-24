#-------------------------------------------------
#
# Project created by QtCreator 2015-06-20T16:32:32
#
#-------------------------------------------------

QT += testlib

QT -= gui

TARGET = tst_signalreadertest

CONFIG += console c++11
CONFIG -= app_bundle

TEMPLATE = app


SOURCES += tst_signalreadertest.cc \
    prioritylibrarystub.cc \
    prioritysubjectstub.cc \
    ../../communication/signalreader.cc \
    ../../signal.cc \
    ../../signalqueue.cc \
    ../../exceptions/unknownscript.cc \
    testsender.cc

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    prioritylibrarystub.hh \
    prioritysubjectstub.hh \
    ../../interfaces/scriptprioritylibrary.hh \
    ../../interfaces/priorityupdatesubject.hh \
    ../../interfaces/priorityupdateobserver.hh \
    ../../communication/signalreader.hh \
    ../../signal.hh \
    ../../signalqueue.hh \
    ../../exceptions/unknownscript.hh \
    testsender.hh

INCLUDEPATH += ../../
INCLUDEPATH += ../../interfaces
INCLUDEPATH += ../../communication
INCLUDEPATH += ../../exceptions
INCLUDEPATH += ../../../../externals/include/
INCLUDEPATH += ../../../utils/messagegroup/
INCLUDEPATH += ../../../utils/messages/

DEPENDPATH += ../../
DEPENDPATH += ../../interfaces
DEPENDPATH += ../../communication
DEPENDPATH += ../../exceptions

LIBS += -L../../../../build/lib/ -lmessagegroup
LIBS += -L../../../../build/lib/ -lmessages
LIBS += -L../../../../externals/lib/ -lqamqp
