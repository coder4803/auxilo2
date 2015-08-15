#-------------------------------------------------
#
# Project created by QtCreator 2015-06-12T22:40:07
#
#-------------------------------------------------

QT       += script scripttools testlib

QT       -= gui

TARGET = tst_qtscripttest
CONFIG   += console c++11
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_qtscripttest.cc \
           ../../plugins/qtscriptinterpreter/qtscriptinterpreter.cc \
           ../../plugins/qtscriptinterpreter/scriptapiadapter.cc \
    fakeapi.cc

HEADERS += ../../plugins/qtscriptinterpreter/qtscriptinterpreter.hh \
           ../../plugins/qtscriptinterpreter/scriptapiadapter.hh \
           ../../ScriptInterpreter/scriptrunexceptions.hh \
           ../../ScriptInterpreter/scriptapi.hh \
    fakeapi.hh

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../exceptions \
               ../../ScriptInterpreter\
               ../../plugins/qtscriptinterpreter

DEPENDPATH += ../../ScriptInterpreter

INCLUDEPATH     += ../../../../utils/messages/

LIBS += -L../../../../../build/lib/ -lmessages
