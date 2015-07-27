#-------------------------------------------------
#
# Project created by QtCreator 2015-07-19T11:21:10
#
#-------------------------------------------------

QT       += testlib xml

QT       -= gui

TARGET = tst_scriptbanktest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


HEADERS += ../../scriptbank.hh \
    ../../configuration.hh \
    ../../scriptbankbuilder.hh \
    ../../interfaces/scriptbankinterface.hh \
    ../../interfaces/scriptlibrary.hh \
    ../../interfaces/scriptprioritylibrary.hh \
    ../../exceptions/unknownscript.hh \
    ../../confxmlhandler.hh


SOURCES += tst_scriptbanktest.cc \
    ../../scriptbank.cc \
    ../../scriptbankbuilder.cc \
    ../../exceptions/unknownscript.cc \
    ../../confxmlhandler.cc

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../
INCLUDEPATH += ../../interfaces
INCLUDEPATH += ../../exceptions
INCLUDEPATH += ../../../utils/messagegroup/
INCLUDEPATH += ../../../utils/messages/

DEPENDPATH += ../../
DEPENDPATH += ../../interfaces
DEPENDPATH += ../../exceptions

LIBS += -L../../../../build/lib/ -lmessagegroup
LIBS += -L../../../../build/lib/ -lmessages

