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
           ../../ScriptLangWrapper/qtscriptwrapper.cc \
           ../../ScriptLangWrapper/scriptapiqobjectwrapper.cc \
    fakeapi.cc

HEADERS += ../../ScriptLangWrapper/qtscriptwrapper.hh \
           ../../ScriptLangWrapper/scriptapiqobjectwrapper.hh \
           ../../exceptions/scriptrunexceptions.hh \
           ../../ScriptAPI/scriptapi.hh \
    fakeapi.hh

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../exceptions \
               ../ScriptLangWrapper \
               ../ScriptAPI

DEPENDPATH += ../../exceptions \
              ../../ScriptLangWrapper \
              ../../ScriptAPI

INCLUDEPATH     += ../../externals/include/
INCLUDEPATH     += ../utils/messagegroup/
INCLUDEPATH     += ../utils/messages/
